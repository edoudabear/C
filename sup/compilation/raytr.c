#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "material.h"
#include "vect.h"

const double epsilon = 1e-3;
const double maxdist = 1e10;

typedef struct light {
    double pos[3];
    double color[3];
} light;

typedef struct sphere {
    double pos[3];
    double radius;
    material mat;
} sphere;

double clamp(const double val, const double mini, const double maxi) {
    // Clip value (val) in interval [mini, maxi] and return it
    
    return val<mini?mini:(val>maxi?maxi:val);
}

void color_clamp(double* col) {
    // Ensure color components (col) are in the [0,1] range
    
    double mini = 1.0, maxi = 0.0;
    for(int i=0; i<3; ++i)
        { col[i] = fmax(0.0, col[i]); mini = fmin(col[i], mini); maxi = fmax(col[i], maxi); }
    if (maxi > 1.0) {
        for(int i=0; i<3; ++i)
            { col[i] = fmin(1.0, mini+(col[i]-mini)/(maxi-mini)); }
    }
}

void vect_reflect(const double* const dir, const double* const n, double* const res) {
    // Compute reflexion of 3D directional vector (dir) on a surface of 3D normal (n)
    // Result stored in (res)
    
    vect_muladd(dir, -2.0*vect_pscal(dir, n), n, res);
}

void vect_refract(const double* const i, const double* const n,
                  const double r, double* const res) {
    // Compute refraction of 3D directional vector (dir) on a surface of 3D normal (n)
    // with ratio of refractive indexes (r = ni/nt)
    // Result stored in (res)
    
    double cosi = clamp(-vect_pscal(i, n), -1.0, 1.0);
    
    if (cosi < 0.0) { // ray in direction opposite to normal
        double rn[3];
        vect_mul(-1.0, n, rn);
        vect_refract(i, rn, 1.0/r, res);
        return;
    }
    
    double c = 1.0 - r*r*(1.0-cosi*cosi);
    if (c < 0) { vect_reflect(i, n, res); return; } // total reflexion
    
    vect_mul(r, i, res);			
    vect_imuladd(res, r*cosi - sqrt(c), n);
}

bool sphere_intersect(const double* const orig, const double* const dir,
                      const sphere* const s, double* dist_hit) {
    // Look for intersection of ray from (orig) with direction (dir)
    // with sphere (s), returns true if found, and store distance to 
    // intersection in (dist_hit), false otherwise
    
    double v[3];
    vect_sub(s->pos, orig, v);
    double pscal = vect_pscal(v, dir);
    double dist_sq = pscal*pscal + s->radius*s->radius - vect_pscal(v, v);
    if (dist_sq < 0) return false;
    
    // Possible intersection, compute distance to detect IEEE rounding issues
    double d1 = sqrt(dist_sq);
    *dist_hit = pscal - d1;
    if (*dist_hit < epsilon) { *dist_hit += 2*d1; }
    return (*dist_hit > epsilon);
}

bool scene_intersect(const double* const orig, const double* const dir,
                     const sphere* const spheres, const int nb_spheres,
                     double* const hit, double* const n, material* mat) {
    // Look for intersection of ray from (orig) with direction (dir)
    // with (nb_spheres) spheres stored in array (spheres), or with the
    // bottom plane, returns true if found, and store intersection position
    // in (hit), normal to surface in (n) and surface material in (mat),
    // false otherwise (dist_hit may be modified)
    
    double max_dist = maxdist;   // max distance to a sphere surface
    
    // Compute the closest collision with a sphere surface (if any)
    for(int i=0; i<nb_spheres; ++i) {
        double dist_hit;
        if (sphere_intersect(orig, dir, &spheres[i], &dist_hit) && dist_hit < max_dist) {
            max_dist = dist_hit;                   // new closest sphere surface
            vect_muladd(orig, dist_hit, dir, hit); // collision position on surface
            vect_sub(hit, spheres[i].pos, n);
            vect_normalize(n);                     // normal to sphere surface
            *mat = spheres[i].mat;                 // sphere material
        }
    }
    
    // Compute collision with the plane (if any)
    if (fabs(dir[1]) > epsilon) { // not parallel to plane
        double dist_hit = -orig[1]/dir[1];
        double impact[3];
        vect_muladd(orig, dist_hit, dir, impact);
        if (dist_hit > epsilon && dist_hit < max_dist
                && (impact[0]+0.5*impact[2]) > -14.0 && (impact[0]+0.5*impact[2]) < 14.0
                && (impact[2]-0.5*impact[0]) > -14.0 && (impact[2]-0.5*impact[0]) < 14.0) {
            vect_copy(impact, hit);
            n[0] = 0.0; n[1] = 1.0; n[2] = 0.0;
            *mat = ((int)(floor(hit[0]+0.5*hit[2]) + floor(hit[2]-0.5*hit[0]))&1)?mirror:white;
            max_dist = dist_hit;
        }
    }
    
    // Collision if max_dist has been reduced
    return max_dist < maxdist;
}

void cast_ray(const double* const orig, const double* const dir, 
              const sphere* const spheres, const int nb_spheres,
              const light* const lights, const int nb_lights,
              int nbiter, double* const color) {
    // compute color for a ray originating from (orig) and traveling
    // in direction (dir), using informations on the scene
    // and store the computed color in (color)
    // Limit the number of refractions/reflexions to nb_iter 
    
    // Compute collision
    double hit[3] = {0.0, 0.0, 0.0};
    double n[3] = {0.0, 1.0, 0.0};
    material mat;
    if (nbiter<=0 || !scene_intersect(orig, dir, spheres, nb_spheres, hit, n, &mat)) {
        double bcolor[3] = {0.1, 0.3, 0.9}; // No collision, return background color
        vect_mul(1.0+dir[1], bcolor, color);
        return;
    }

    // Phong illumination contributions
    double refl_color[3] = {0.0, 0.0, 0.0};
    double refr_color[3] = {0.0, 0.0, 0.0};
    double diff_color[3] = {0.0, 0.0, 0.0};
    double spec_color[3] = {0.0, 0.0, 0.0};

    // Compute color from reflexion
    double refl_dir[3];
    if (vect_norm(mat.refl_coeff)>0.0) {
        vect_reflect(dir, n, refl_dir);
        vect_normalize(refl_dir);
        cast_ray(hit, refl_dir, spheres, nb_spheres, lights, nb_lights, nbiter-1, refl_color);
    }
    
    // Compute color from refraction
    double refr_dir[3];
    if (vect_norm(mat.refr_coeff)>0.0) {
        vect_refract(dir, n, 1.0/mat.refractive_index, refr_dir);
        vect_normalize(refr_dir);
        cast_ray(hit, refr_dir, spheres, nb_spheres, lights, nb_lights, nbiter-1, refr_color);
    }

    // Compute diffusion and specular colors using lights data
    for(int i=0; i<nb_lights; ++i) {
        // Compute unit vector towards light source
        double light_dir[3];
        vect_sub(lights[i].pos, hit, light_dir);
        vect_normalize(light_dir);
        
        // Check whether light source reachs the surface (for shadows)
        double shadow_hit[3], dummy_n[3];
        material dummy_mat;
        if (scene_intersect(hit, light_dir, spheres, nb_spheres, shadow_hit, dummy_n, &dummy_mat)) {
            double travel_light[3], travel[3];
            vect_sub(hit, lights[i].pos, travel);
            vect_sub(shadow_hit, lights[i].pos, travel_light);
            if (vect_norm(travel_light) < vect_norm(travel)) { continue; } // In shadow
        }
        
        // Add light contribution to diffuse color
        vect_imuladd(diff_color, mat.diff_coeff*fmax(0.0, vect_pscal(light_dir, n)), lights[i].color);
        
        // Add light contribution to specular color
        double refl_light_dir[3];
        vect_reflect(light_dir, n, refl_light_dir);
        double scolor[3];
        vect_cmul(mat.spec_coeff, lights[i].color, scolor);
        double k = pow(fmax(0.0, vect_pscal(refl_light_dir, dir)), mat.specular_exponent);
        vect_imuladd(spec_color, k, scolor);
    }
    
    // Compute final color from Phong illumination model
    vect_cmul(mat.color, diff_color, color);
    vect_iadd(color, spec_color);
    vect_icmuladd(color, mat.refl_coeff, refl_color);
    vect_icmuladd(color, mat.refr_coeff, refr_color);
}

int render_to_file(const char* const filename,
                   const sphere* const spheres, const int nb_spheres,
                   const light* const lights, const int nb_lights,
                   const int width, const int height, const double fov) {
    // Renter a scene and store the result in a file named (filename)
    // using scene data, image size and camera field of view (fov)
    // and return 0 on success, -1 on file error
    
    FILE* f = fopen(filename, "wb");
    if (!f) { return -1; } // Failure to open file
    
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    
    for (int j = 0; j<height; ++j) {
        for (int i = 0; i<width; ++i) {
            double orig[3] = { 0.0, 4.5, 20.0 }; // Position of camera
            double dir[3] = { (i+.5)-width/2., -(j+.5)+height/2., -height/(2.*tan(fov/2.)) };
            vect_normalize(dir); // direction of ray for pixel (i, j)
            double color[3];
            cast_ray(orig, dir, spheres, nb_spheres, lights, nb_lights, 3, color);
            color_clamp(color);
            fprintf(f, "%c%c%c", (char)(255*color[0]), (char)(255*color[1]), (char)(255*color[2]));
        }
    }

    fclose(f);
    return 0;
}

int main(int argc, char* argv[]) {
    sphere spheres[] = { { .pos={4.0, 4.0, 4.0}, .radius=2.0, .mat=plastic_y },
                         { .pos={5.0, 2.0, 9.0}, .radius=2.0, .mat=rubber_r },
                         { .pos={1.0, 3.5, 8.0}, .radius=2.0, .mat=glass }, 
                         { .pos={2.0, 1.5, 9.5}, .radius=1.0, .mat=glass_y }, 
                         { .pos={1.0, 4.5, 1.5}, .radius=1.0, .mat=rubber_o }, 
                         { .pos={-0.5, 1.0, 10.0}, .radius=1.0, .mat=rubber_o }, 
                         { .pos={-2.0, 3.0, 2.0}, .radius=2.0, .mat=rubber_g }, 
                         { .pos={-5.0, 2.5, 9.0}, .radius=2.0, .mat=plastic_b }, 
                         { .pos={8.0, 7.5, -3.0}, .radius=3.0, .mat=plastic_m }, 
                         { .pos={-7.0, 7.0, 2.0}, .radius=2.0, .mat=metal },
                         { .pos={-3.0, 7.0, -11.0}, .radius=7.0, .mat=metal },
                         { .pos={-3.0, 1.0, 5.5}, .radius=1.0, .mat=metal } };
    
    light lights[] = { { .pos={-15.0, 20.0, 55.0}, .color={1.8, 1.8, 1.3} },
                       { .pos={30.0, 40.0, 40.0}, .color={1.6, 1.5, 1.6} },
                       { .pos={20.0, 30.0, 10.0}, .color={1.3, 1.6, 1.6} } };

    return render_to_file("render.ppm",
                          spheres, sizeof(spheres)/sizeof(spheres[0]),
                          lights, sizeof(lights)/sizeof(lights[0]),
                          1024, 768, 1.0);
}
