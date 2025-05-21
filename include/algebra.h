#ifndef _ALGEBRA_H_
#define _ALGEBRA_H_
#include <math.h>
#include <stdio.h>
typedef struct { float x, y, z; } Vector;
typedef struct { float x, y, z, w; } HomVector;

/* Column-major order are used for the matrices here to be compatible with OpenGL.
** The indices used to access elements in the matrices are shown below.
**  _                _
** |                  |
** |   0   4   8  12  |
** |                  |
** |   1   5   9  13  |
** |                  |
** |   2   6  10  14  |
** |                  |
** |   3   7  11  15  |
** |_                _|
*/
typedef struct matrix { float e[16]; } Matrix;

Vector Add(Vector a, Vector b);
Vector Subtract(Vector a, Vector b);
Vector CrossProduct(Vector a, Vector b);
float DotProduct(Vector a, Vector b);
float Length(Vector a);
Vector Normalize(Vector a);
Vector ScalarVecMul(float t, Vector a);
HomVector MatVecMul(Matrix a, Vector b);
Vector Homogenize(HomVector a);
Matrix MatMatMul(Matrix a, Matrix b);

Matrix operator*(const Matrix& a, const Matrix& b);
Matrix Scale(float x, float y, float z);
Matrix Translate(float x, float y, float z);
Matrix RotateX(float rad);
Matrix RotateY(float rad);
Matrix RotateZ(float rad);

void PrintMatrix(char const* name, Matrix m);
void PrintVector(char const* name, Vector v);
void PrintHomVector(char const* name, HomVector h);

Vector SurfaceNormal(Vector a, Vector b, Vector c);
#endif

