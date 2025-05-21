#define _USE_MATH_DEFINES // To get M_PI defined
#include "algebra.h"

Vector CrossProduct(Vector a, Vector b) {
	Vector v = { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
	return v;
}

float DotProduct(Vector a, Vector b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector Subtract(Vector a, Vector b) {
	Vector v = { a.x-b.x, a.y-b.y, a.z-b.z };
	return v;
}    

Vector Add(Vector a, Vector b) {
	Vector v = { a.x+b.x, a.y+b.y, a.z+b.z };
	return v;
}    

float Length(Vector a) {
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vector Normalize(Vector a) {
	float len = Length(a);
	Vector v = { a.x/len, a.y/len, a.z/len };
	return v;
}

Vector ScalarVecMul(float t, Vector a) {
	Vector b = { t*a.x, t*a.y, t*a.z };
	return b;
}

HomVector MatVecMul(Matrix a, Vector b) {
	HomVector h;
	h.x = b.x*a.e[0] + b.y*a.e[4] + b.z*a.e[8] + a.e[12];
	h.y = b.x*a.e[1] + b.y*a.e[5] + b.z*a.e[9] + a.e[13];
	h.z = b.x*a.e[2] + b.y*a.e[6] + b.z*a.e[10] + a.e[14];
	h.w = b.x*a.e[3] + b.y*a.e[7] + b.z*a.e[11] + a.e[15];
	return h;
}

Vector Homogenize(HomVector h) {
	Vector a;
	if (h.w == 0.0) {
		fprintf(stderr, "Homogenize: w = 0\n");
		a.x = a.y = a.z = 9999999;
		return a;
	}
	a.x = h.x / h.w;
	a.y = h.y / h.w;
	a.z = h.z / h.w;
	return a;
}

Matrix MatMatMul(Matrix a, Matrix b) {
	Matrix c;
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			c.e[j*4+i] = 0.0;
			for (k = 0; k < 4; k++)
				c.e[j*4+i] += a.e[k*4+i] * b.e[j*4+k];
		}
	}
	return c;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    return MatMatMul(a, b);
}


Matrix Scale(float x, float y, float z) {
    Matrix S;
    S.e[0] = x;		S.e[4] = 0.f;	S.e[8]  = 0.f;	S.e[12] = 0.f;
    S.e[1] = 0.f;	S.e[5] = y;		S.e[9]  = 0.f;	S.e[13] = 0.f;
    S.e[2] = 0.f;	S.e[6] = 0.f;	S.e[10] = z;	S.e[14] = 0.f;
    S.e[3] = 0.f;	S.e[7] = 0.f;	S.e[11] = 0.f;	S.e[15] = 1.f;
    return S;
}

Matrix Translate(float x, float y, float z) {
    Matrix T;
    T.e[0] = 1.f;	T.e[4] = 0.f;	T.e[8]  = 0.f;	T.e[12] = x;
    T.e[1] = 0.f;	T.e[5] = 1.f;	T.e[9]  = 0.f;	T.e[13] = y;
    T.e[2] = 0.f;	T.e[6] = 0.f;	T.e[10] = 1.f;	T.e[14] = z;
    T.e[3] = 0.f;	T.e[7] = 0.f;	T.e[11] = 0.f;	T.e[15] = 1.f;
    return T;
}

Matrix RotateX(float rad) {
    Matrix R;
    R.e[0] = 1.f;	R.e[4] = 0.f;		R.e[8]  = 0.f;			R.e[12] = 0.f;
    R.e[1] = 0.f;	R.e[5] = cos(rad);	R.e[9]  = -sin(rad);	R.e[13] = 0.f;
    R.e[2] = 0.f;	R.e[6] = sin(rad);	R.e[10] = cos(rad);		R.e[14] = 0.f;
    R.e[3] = 0.f;	R.e[7] = 0.f;		R.e[11] = 0.f;			R.e[15] = 1.f;
    return R;
}

Matrix RotateY(float rad) {
    Matrix R;
    R.e[0] = cos(rad);	R.e[4] = 0.f;	R.e[8]  = sin(rad);	R.e[12] = 0.f;
    R.e[1] = 0.f;		R.e[5] = 1.0f;	R.e[9]  = 0.f;		R.e[13] = 0.f;
    R.e[2] = -sin(rad);	R.e[6] = 0.f;	R.e[10] = cos(rad);	R.e[14] = 0.f;
    R.e[3] = 0.f;		R.e[7] = 0.f;	R.e[11] = 0.f;		R.e[15] = 1.f;
    return R;
}

Matrix RotateZ(float rad) {
    Matrix R;
    R.e[0] = cos(rad);	R.e[4] = -sin(rad);	R.e[8]  = 0.f;	R.e[12] = 0.f;
    R.e[1] = sin(rad);	R.e[5] = cos(rad);	R.e[9]  = 0.f;	R.e[13] = 0.f;
    R.e[2] = 0.f;		R.e[6] = 0.f;		R.e[10] = 1.f;	R.e[14] = 0.f;
    R.e[3] = 0.f;		R.e[7] = 0.f;		R.e[11] = 0.f;	R.e[15] = 1.f;
    return R;
}

// Matrix Rotate(float radX, float radY, float radZ){
// 	// RotX * (RotY * RotZ)
// 	return MatMatMul(RotateX(radX), MatMatMul(RotateY(radY), RotateZ(radZ)));
// }

void PrintVector(char const *name, Vector a) {
	printf("%s: %6.5lf %6.5lf %6.5lf\n", name, a.x, a.y, a.z);
}

void PrintHomVector(char const *name, HomVector a) {
	printf("%s: %6.5lf %6.5lf %6.5lf %6.5lf\n", name, a.x, a.y, a.z, a.w);
}

void PrintMatrix(char const *name, Matrix a) { 
	int i,j;

	printf("%s:\n", name);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%6.5lf ", a.e[j*4+i]);
		}
		printf("\n");
	}
}

Vector SurfaceNormal(Vector a, Vector b, Vector c)
{
	return {0.80078125f,0.34765625f, 0.1796875f};
}