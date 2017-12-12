#include "StdAfx.h"
#include "Object4D.h"


Object4D::Object4D(void)
{
	for(int i=0;i<6;i++)
		Rotations[i]=0;
}

void Object4D::Translation(float x,float y,float z,float w)
{
	for(int i=0;i<verts.size();i++)
	{
		verts[i].value[0]+=x;
		verts[i].value[1]+=y;
		verts[i].value[2]+=z;
		verts[i].value[3]+=w;
	}
}

void Object4D::Draw(Vector4 min,Vector4 max,Vector4 cam)
{
	float vct[3];
	Vector4 vctt;
	glDisable(GL_LIGHTING);
	glPointSize(5.0); 
	glLineWidth(1.5);
	glEnable(GL_LINE_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	
	glFrustum(min.value[0], max.value[0],min.value[1], max.value[1],min.value[2], max.value[2]);
	glLoadIdentity();

	gluLookAt(cam.value[0],cam.value[1],cam.value[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	glBegin(GL_LINES);
	
	for(int i=0;i<lines.size();i++)
	{
		vctt=Vector4(verts[lines[i].first]);
		
		vctt.Transform(Matrix4::RotateXY(Rotations[0]));
		vctt.Transform(Matrix4::RotateXZ(Rotations[1]));
		vctt.Transform(Matrix4::RotateYZ(Rotations[2]));
		vctt.Transform(Matrix4::RotateXW(Rotations[3]));
		vctt.Transform(Matrix4::RotateYW(Rotations[4]));
		vctt.Transform(Matrix4::RotateZW(Rotations[5]));
		vctt.Project(min.value[3],max.value[3],vct);
		glVertex3fv(vct);
		vctt=Vector4(verts[lines[i].second]);
		
		vctt.Transform(Matrix4::RotateXY(Rotations[0]));
		vctt.Transform(Matrix4::RotateXZ(Rotations[1]));
		vctt.Transform(Matrix4::RotateYZ(Rotations[2]));
		vctt.Transform(Matrix4::RotateXW(Rotations[3]));
		vctt.Transform(Matrix4::RotateYW(Rotations[4]));
		vctt.Transform(Matrix4::RotateZW(Rotations[5]));
		vctt.Project(min.value[3],max.value[3],vct);
		glVertex3fv(vct);
	}	
	glEnd();
	glPopMatrix();

}

void Object4D::Transform(Matrix4 mat)
{
	for(int i=0;i<verts.size();i++)
	{
		verts[i].Transform(mat);
	}
}

Object4D::~Object4D(void)
{

}
