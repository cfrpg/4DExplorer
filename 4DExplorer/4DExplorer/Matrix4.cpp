#include "StdAfx.h"
#include "Matrix4.h"
#include <cmath>


Matrix4::Matrix4(void)
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			value[i][j]=0;

}

void Matrix4::Identity()
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			value[i][j]=(i==j?1:0);
}

Matrix4 Matrix4::RotateXY(float a)
{
	Matrix4 m;
	m.Identity();
	m.value[0][0]=cos(a);
	m.value[0][1]=sin(a);
	m.value[1][0]=-sin(a);
	m.value[1][1]=cos(a);
	return m;
}
Matrix4 Matrix4::RotateXZ(float a)
{
	Matrix4 m;
	m.Identity();
	m.value[0][0]=cos(a);
	m.value[0][2]=sin(a);
	m.value[2][0]=-sin(a);
	m.value[2][2]=cos(a);
	return m;
}
Matrix4 Matrix4::RotateYZ(float a)
{
	Matrix4 m;
	m.Identity();
	m.value[1][1]=cos(a);
	m.value[1][2]=sin(a);
	m.value[2][1]=-sin(a);
	m.value[2][2]=cos(a);
	return m;
}
Matrix4 Matrix4::RotateXW(float a)
{
	Matrix4 m;
	m.Identity();
	m.value[0][0]=cos(a);
	m.value[0][3]=sin(a);
	m.value[3][0]=-sin(a);
	m.value[3][3]=cos(a);
	return m;
}
Matrix4 Matrix4::RotateYW(float a)
{
	Matrix4 m;
	m.Identity();
	m.value[1][1]=cos(a);
	m.value[1][3]=sin(a);
	m.value[3][1]=-sin(a);
	m.value[3][3]=cos(a);
	return m;
}
Matrix4 Matrix4::RotateZW(float a)
{
	Matrix4 m;
	m.Identity();
	m.value[2][2]=cos(a);
	m.value[2][3]=sin(a);
	m.value[3][2]=-sin(a);
	m.value[3][3]=cos(a);
	return m;
}


Matrix4::~Matrix4(void)
{
}
