#include "StdAfx.h"
#include "Game.h"



Game::Game(void)
{

}

void Game::Initialize(HWND _hWnd)
{	
	for(int i=0;i<150;i++)
		Keys[i]=0;
	min=Vector4(-200,-200,-200,-2);
	max=Vector4();
	cam=Vector4();
	hWnd=_hWnd;
	object=Object4D();
	int n;
	float x,y,z,w;
	int a,b;
	FILE *fp=fopen("model\\in.c4d","r");	
	FILE *fp1=fopen("model\\config.txt","r");	
	if(fp==NULL)
	{
		
		object.verts.push_back(Vector4(-1,-1,-1,-1));
	
		object.verts.push_back(Vector4(-1,-1,-1,-1));
		object.verts.push_back(Vector4(1,-1,-1,-1));
		object.verts.push_back(Vector4(1,1,-1,-1));
		object.verts.push_back(Vector4(-1,1,-1,-1));

		object.verts.push_back(Vector4(-1,-1,1,-1));
		object.verts.push_back(Vector4(1,-1,1,-1));
		object.verts.push_back(Vector4(1,1,1,-1));
		object.verts.push_back(Vector4(-1,1,1,-1));

	
	
		object.verts.push_back(Vector4(-1,-1,-1,1));
		object.verts.push_back(Vector4(1,-1,-1,1));
		object.verts.push_back(Vector4(1,1,-1,1));
		object.verts.push_back(Vector4(-1,1,-1,1));

		object.verts.push_back(Vector4(-1,-1,1,1));
		object.verts.push_back(Vector4(1,-1,1,1));
		object.verts.push_back(Vector4(1,1,1,1));
		object.verts.push_back(Vector4(-1,1,1,1));
	
		object.lines.push_back(pair<int,int>(1,2));
		object.lines.push_back(pair<int,int>(2,3));
		object.lines.push_back(pair<int,int>(3,4));
		object.lines.push_back(pair<int,int>(4,1));
	
		object.lines.push_back(pair<int,int>(5,6));
		object.lines.push_back(pair<int,int>(6,7));
		object.lines.push_back(pair<int,int>(7,8));
		object.lines.push_back(pair<int,int>(8,5));
		object.lines.push_back(pair<int,int>(5,1));
		object.lines.push_back(pair<int,int>(6,2));
		object.lines.push_back(pair<int,int>(7,3));
		object.lines.push_back(pair<int,int>(8,4));
	
		for(int i=9;i<12;i++)
		{
			object.lines.push_back(pair<int,int>(i,i+1));
			object.lines.push_back(pair<int,int>(i+4,i+5));
			object.lines.push_back(pair<int,int>(i,i+4));
		}
		object.lines.push_back(pair<int,int>(9,12));
		object.lines.push_back(pair<int,int>(13,16));
		object.lines.push_back(pair<int,int>(12,16));
		for(int i=1;i<=8;i++)
		{
			object.lines.push_back(pair<int,int>(i,i+8));
		}
		
		max.value[0]=200;
		max.value[1]=200;
		max.value[2]=200;
		max.value[3]=2;
		cam.value[0]=0;
		cam.value[1]=0;
		cam.value[2]=10;
		cam.value[3]=0;
		
	}
	else
	{
		for(int i=0;i<4;i++)
		{
			fscanf(fp,"%f%f",&min.value[i],&max.value[i]);
		}
		for(int i=0;i<3;i++)
		{
			fscanf(fp,"%f",&cam.value[i]);
		}
		fscanf(fp,"%d",&n);
		for(int i=0;i<n;i++)
		{
			fscanf(fp,"%f%f%f%f",&x,&y,&z,&w);
			object.verts.push_back(Vector4(x,y,z,w));
		}
		fscanf(fp,"%d",&n);
		for(int i=0;i<n;i++)
		{
			fscanf(fp,"%d%d",&a,&b);
			object.lines.push_back(pair<int,int>(a,b));
		}
		fclose(fp);
		fp=NULL;
	}
	if(fp1==NULL)
	{
		for(int i=0;i<6;i++)
		{
			rv[i]=0.004;
		}
	}
	else
	{
		for(int i=0;i<6;i++)
		{
			fscanf(fp1,"%f",&rv[i]);
		}
		fclose(fp1);
	}
	

	
}



void Game::UpdateControl(UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_MOUSEMOVE:
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);
			break;
		case WM_LBUTTONDOWN:
			LeftButton=1;
			break;
		case WM_LBUTTONUP:
			LeftButton=0;
			break;
		case WM_KEYDOWN:
			Keys[wParam]=1;
			break;
		case WM_KEYUP:
			Keys[wParam]=0;
			break;
		
	}
}

void Game::Update()
{
	int t;
	if(Keys[VK_CONTROL])
	{
		t=-1;
	}
	else
	{
		t=1;
	}
	if(Keys[VK_SHIFT])
		t*=3;
	for(int i=0;i<6;i++)
	{
		if(Keys[49+i])
		{
			object.Rotations[i]+=rv[i]*t;
		}
	}
	if(Keys[VK_UP])
	{
		cam.value[2]+=0.1f;
	}
	if(Keys[VK_DOWN])
	{
		cam.value[2]-=0.1f;
	}
	if(Keys[VK_LEFT])
	{
		cam.value[0]-=0.1f;
	}
	if(Keys[VK_RIGHT])
	{
		cam.value[0]+=0.1f;
	}
	if(Keys[87])
	{
		cam.value[1]+=0.1f;
	}
	if(Keys[83])
	{
		cam.value[1]-=0.1f;
	}
	for(int i=0;i<3;i++)
	{
		if(cam.value[i]>15)
			cam.value[i]=15;
		if(cam.value[i]<-15)
			cam.value[i]=-15;
	}
	lastLeft=LeftButton;
	lastX=MouseX;
	lastY=MouseY;
	
}

void Game::Draw()
{	
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Çå³ý»º³åÇø		
	object.Draw(min,max,cam);
	
}

Game::~Game(void)
{

}

int Game::isClicked(int x1,int y1,int x2,int y2)
{
	if(MouseX>=x1&&MouseX<=x2&&MouseY>=y1&&MouseY<=y2)
		return 1;
	return 0;
}

