#pragma once

class Matrix4
{
public:
	Matrix4(void);
	~Matrix4(void);
	float value[4][4];
	static Matrix4 RotateXY(float a);
	static Matrix4 RotateXZ(float a);
	static Matrix4 RotateYZ(float a);
	static Matrix4 RotateXW(float a);
	static Matrix4 RotateYW(float a);
	static Matrix4 RotateZW(float a);

	void Identity();
};
