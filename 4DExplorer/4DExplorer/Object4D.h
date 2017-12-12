#pragma once
#include "Matrix4.h"
#include "Vector4.h"
#include <vector>

using namespace std;

class Object4D
{
public:
	Object4D(void);
	vector<Vector4> verts;
	vector<pair<int,int>> lines;
	void Draw(Vector4 min,Vector4 max,Vector4 cam);
	void Transform(Matrix4 mat);
	void Translation(float x,float y,float z,float w);
	float Rotations[6];
	~Object4D(void);
};

