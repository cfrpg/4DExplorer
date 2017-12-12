#pragma once
#include <Matrix4.h>

class Vector4
{
public:
	Vector4(void);	
	Vector4(float x,float y,float z,float w);
	~Vector4(void);
	float value[4];
	
	void Transform(Matrix4 mat);
	void Project(float near,float far,float* res);
};

