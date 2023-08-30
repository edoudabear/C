#include <math.h>
#include "vect.h"

double vect_pscal(const double* const arg1, const double* const arg2) {
	double res = 0.0; // <- tenter de retirer l'initialisation ici !
	for (int i=0; i<3; ++i) {
		res += arg1[i]*arg2[i];
	}
	return res;
}

double vect_norm(const double* const v) {
	return sqrt(vect_pscal(v, v));
}

void vect_normalize(double* const v) {
	double len=vect_norm(v);
	if (len == 0.0) {
		return;
	}
	for (int i=0; i<3; ++i) {
		v[i] /= len;
	}
}

void vect_copy(const double* const v, double* const res) {
	for (int i=0; i<3; ++i) { res[i] = v[i]; }
}

void vect_add(const double* const arg1, const double* const arg2, double* const res) {
	for (int i=0; i<3; ++i) { res[i] = arg1[i]+arg2[i]; }
}

void vect_sub(const double* const arg1, const double* const arg2, double* const res) {
	for (int i=0; i<3; ++i) { res[i] = arg1[i]-arg2[i]; }
}

void vect_mul(const double coeff, const double* const v, double* const res) {
	for (int i=0; i<3; ++i) { res[i] = coeff*v[i]; }
}

void vect_cmul(const double* const arg1, const double* const arg2, double* const res) {
	for (int i=0; i<3; ++i) { res[i] = arg1[i]*arg2[i]; }
}

void vect_muladd(const double* const arg1, const double coeff, const double* const arg2, double* const res) {
	for (int i=0; i<3; ++i) { res[i] = arg1[i] + coeff*arg2[i]; }
}

void vect_imul(double* const v, const double coeff) {
	for (int i=0; i<3; ++i) { v[i] *= coeff; }
}

void vect_icmul(double* const arg1, const double* const arg2) {
	for (int i=0; i<3; ++i) { arg1[i] *= arg2[i]; }
}

void vect_iadd(double* const arg1, const double* const arg2) {
	for (int i=0; i<3; ++i) { arg1[i] += arg2[i]; }
}

void vect_imuladd(double* const arg1, const double coeff, const double* const arg2) {
	for (int i=0; i<3; ++i) { arg1[i] += coeff*arg2[i]; }
}

void vect_icmuladd(double* const arg1, const double* const coeffs, const double* const arg2) {
	for (int i=0; i<3; ++i) { arg1[i] += coeffs[i]*arg2[i]; }
}

