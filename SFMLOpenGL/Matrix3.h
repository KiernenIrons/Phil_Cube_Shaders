#pragma once
#include <math.h>
#include <iostream>
#include "Vector3.h"

class Matrix3
{// The class has nine variables, 3 rows and 3 columns
public:

	Matrix3();

	Matrix3(Vector3 Row1, Vector3 Row2, Vector3 Row3);

	Matrix3(double _A11, double _A12, double _A13, double _A21, double _A22, double _A23, double _A31, double _A32, double _A33);

	Matrix3 operator*(Matrix3 M1);
	Vector3 Row(int i);
	Vector3 Column(int i);
	Matrix3 Inverse(Matrix3 M1);
	Matrix3 Rotation(int _angle);
	static Matrix3 Translate(float dx, float dy);
	static Matrix3 Scale(float dx, float dy);
	Vector3 operator*(Vector3 V1);
	Matrix3 Transpose(Matrix3 M1);

	Matrix3 operator+(Matrix3 M1);

	Matrix3 operator-(Matrix3 M1);

	static Matrix3 RotationX(float _angle);

	static Matrix3 RotationY(float _angle);

	static Matrix3 RotationZ(float _angle);

	static Matrix3 Scale3D(double dx);

	Matrix3 operator*(double x);

	double A11;
	double A12;
	double A13;
	double A21;
	double A22;
	double A23;
	double A31;
	double A32;
	double A33;
};