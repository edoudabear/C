typedef struct material {
	double diff_coeff;
	double color[3];
	double refr_coeff[3];
    double refractive_index;
	double refl_coeff[3];
	double spec_coeff[3];
	double specular_exponent;
} material;

material white = {
	.diff_coeff = 1.0, .color = {0.3, 0.3, 0.3},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.1, 0.1, 0.1},
	.spec_coeff = {0.0, 0.0, 0.0}, .specular_exponent = 1.0
};

material plastic_y = {
	.diff_coeff = 0.5, .color = {0.5, 0.5, 0.1},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.0, 0.0, 0.0},
	.spec_coeff = {0.7, 0.7, 0.7}, .specular_exponent = 80.0
};

material plastic_m = {
	.diff_coeff = 0.5, .color = {0.5, 0.2, 0.5},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.0, 0.0, 0.0},
	.spec_coeff = {0.7, 0.7, 0.7}, .specular_exponent = 80.0
};

material plastic_b = {
	.diff_coeff = 0.5, .color = {0.1, 0.2, 0.5},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.0, 0.0, 0.0},
	.spec_coeff = {0.7, 0.7, 0.7}, .specular_exponent = 80.0
};

material glass = {
	.diff_coeff = 0.0, .color = {0.0, 0.0, 0.0},
	.refr_coeff = {0.95, 0.95, 0.95}, .refractive_index = 1.4,
	.refl_coeff = {0.1, 0.1, 0.1},
	.spec_coeff = {0.5, 0.5, 0.5}, .specular_exponent = 150.0
};

material glass_y = {
	.diff_coeff = 0.0, .color = {0.0, 0.0, 0.0},
	.refr_coeff = {0.95, 0.85, 0.5}, .refractive_index = 1.4,
	.refl_coeff = {0.1, 0.1, 0.1},
	.spec_coeff = {0.5, 0.5, 0.5}, .specular_exponent = 150.0
};

material rubber_r = {
	.diff_coeff = 0.9, .color = {0.3, 0.1, 0.15},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.0, 0.0, 0.0},
	.spec_coeff = {0.1, 0.1, 0.1}, .specular_exponent = 10.0
};

material rubber_g = {
	.diff_coeff = 0.9, .color = {0.1, 0.2, 0.1},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.0, 0.0, 0.0},
	.spec_coeff = {0.1, 0.1, 0.1}, .specular_exponent = 10.0
};

material rubber_o = {
	.diff_coeff = 0.9, .color = {0.25, 0.15, 0.075},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.0, 0.0, 0.0},
	.spec_coeff = {0.1, 0.1, 0.1}, .specular_exponent = 10.0
};

material mirror = {
	.diff_coeff = 0.0, .color = {1.0, 1.0, 1.0},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.8, 0.8, 0.8},
	.spec_coeff = {0.1, 0.1, 0.1}, .specular_exponent = 1250.0
};

material metal = {
	.diff_coeff = 0.2, .color = {0.4, 0.2, 0.15},
	.refr_coeff = {0.0, 0.0, 0.0}, .refractive_index = 1.0,
	.refl_coeff = {0.6, 0.3, 0.23},
	.spec_coeff = {3.2, 1.6, 1.2}, .specular_exponent = 550.0
};
