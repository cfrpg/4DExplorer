#include "StdAfx.h"
#include "Vector4.h"




Vector4::Vector4(void)
{
}

Vector4::Vector4(float x,float y,float z,float w)
{
	value[0]=x;
	value[1]=y;
	value[2]=z;
	value[3]=w;
}

void Vector4::Transform(Matrix4 mat)
{
	float temp[4];
	for(int i=0;i<4;i++)
	{
		temp[i]=value[i];
		value[i]=0;
	}
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			value[i]+=temp[j]*mat.value[i][j];
		}
	}
}

void Vector4::Project(float n,float f ,float *res)
{
	float t=(f-value[3])/(f-n);
	res[0]=value[0]*t;
	res[1]=value[1]*t;
	res[2]=value[2]*t;
}



Vector4::~Vector4(void)
{

}
