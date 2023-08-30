double vect_pscal(const double* const arg1, const double* const arg2);

double vect_norm(const double* const v);

void vect_normalize(double* const v);

void vect_copy(const double* const v, double* const res);

void vect_add(const double* const arg1, const double* const arg2, double* const res);
void vect_sub(const double* const arg1, const double* const arg2, double* const res);
void vect_mul(const double coeff, const double* const v, double* const res);
void vect_cmul(const double* const arg1, const double* const arg2, double* const res);
void vect_muladd(const double* const arg1, const double coeff, const double* const arg2, double* const res);

void vect_imul(double* const v, const double coeff);
void vect_icmul(double* const arg1, const double* const arg2);
void vect_iadd(double* const arg1, const double* const arg2);

void vect_imuladd(double* const arg1, const double coeff, const double* const arg2);
void vect_icmuladd(double* const arg1, const double* const coeffs, const double* const arg2);
