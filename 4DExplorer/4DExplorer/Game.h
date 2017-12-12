#pragma once
#include <Windows.h>
#include "Object4D.h"
#include "Vector4.h"
class Game
{
	public:
		Game(void);
		~Game(void);
		void Update();
		void Draw();
		void UpdateControl(UINT msg,WPARAM wParam,LPARAM lParam);		
		void Initialize(HWND _hWnd);
	private:		
		int MouseX;
		int MouseY;
		int LeftButton;		
		int isEnd;

		char Keys[150];

		int currentPlayer;
		int lastLeft;
		int lastX;
		int lastY;
		HWND hWnd;
		int isClicked(int x1,int y1,int x2,int y2);
		
		Object4D object;
		Vector4 min;
		Vector4 max;
		Vector4 cam;
		float rv[6];
};
