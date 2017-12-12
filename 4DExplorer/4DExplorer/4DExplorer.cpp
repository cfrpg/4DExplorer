// 4DExplorer.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "4DExplorer.h"
#include <gl\glut.h>
#include <Game.h>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

struct WIN_MSGMAP_ENTRY { //消息映射表结构
	UINT nMessage; //消息码
	LRESULT (*pfn)(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
};
#define BEGIN_MESSAGE_MAP() const struct WIN_MSGMAP_ENTRY _messageEntries[]={
#define ON_MSG(nMessage,pfn) { nMessage, pfn},
#define END_MESSAGE_MAP() {0, NULL} };
void    OnIdle(void);
LRESULT OnDestroy(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
LRESULT OnCreate(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
LRESULT OnPaint(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
LRESULT OnSize(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
LRESULT OnCommand(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
LRESULT OnControl(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

BEGIN_MESSAGE_MAP()
	ON_MSG(WM_DESTROY, OnDestroy) //窗口销毁消息映射
	ON_MSG(WM_CREATE, OnCreate) //窗口创建消息映射
	ON_MSG(WM_PAINT, OnPaint) //重绘消息映射
	ON_MSG(WM_SIZE, OnSize) //窗口大小变化消息映射
	ON_MSG(WM_COMMAND, OnCommand) //菜单消息映射
	ON_MSG(WM_MOUSEMOVE, OnControl) //鼠标移动消息映射
	ON_MSG(WM_LBUTTONDOWN,OnControl)
	ON_MSG(WM_LBUTTONUP,OnControl)
	ON_MSG(WM_KEYDOWN,OnControl)
	ON_MSG(WM_KEYUP,OnControl)
END_MESSAGE_MAP()

HGLRC hRC; //OpenGL渲染场境
void InitScene(); //初始化OpenGL显示场景
void ChangeSize(int w,int h); //OpenGL视区缩放
 HWND hWnd;
 Game game;

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR    lpCmdLine,int  nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;
	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY4DEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY4DEXPLORER));

	// 主消息循环:
	while (TRUE)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) 
		{ //检测队列有无消息
			if (!GetMessage(&msg,NULL,0,0)) break; //WM_QUIT结束消息循环
			//将快捷键转换为相应的菜单命令
			if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)){
				TranslateMessage(&msg); //将键盘事件转化成字符消息
				DispatchMessage(&msg);//分发消息到窗口的消息处理函数去执行
			}
		}
		OnIdle(); //无消息时处理空闲过程		
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY4DEXPLORER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY4DEXPLORER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WORD i;
	for(i=0;_messageEntries[i].nMessage; i++) 
	{
		//如果消息映射表中有该消息，则调用对应的消息处理函数
		if (message==_messageEntries[i].nMessage) 
		{
			if(message!=WM_DESTROY)//非WM_DESTROY消息处理后立即返回
				return (*_messageEntries[i].pfn)(hWnd,message,wParam,lParam);
			//如果是WM_DESTROY消息处理但不返回，继续后处理
			(*_messageEntries[i].pfn)(hWnd,message,wParam,lParam);
			break;
		}
	}
	if (message==WM_DESTROY) 
	{ //WM_DESTROY后处理
		PostQuitMessage(0); //发出WM_QUIT消息，结束应用程序
		return 0; //处理了这条消息必须返回0
	}
	return DefWindowProc(hWnd,message,wParam,lParam); //其他消息调用默认处理
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
LRESULT OnDestroy(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}
LRESULT OnCreate(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	//窗口创建消息处理
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;
	HDC hdc;
	//定义像素格式
	ZeroMemory(&pfd,sizeof(pfd));//全部数据设为0
	pfd.nSize		=sizeof(pfd);
	pfd.nVersion		=1;
	pfd.dwFlags		=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType	=PFD_TYPE_RGBA;
	pfd.cColorBits	=24;
	pfd.cDepthBits	=16;
	pfd.iLayerType	=PFD_MAIN_PLANE;
	hdc=GetDC(hWnd); //客户区DC
	nPixelFormat=ChoosePixelFormat(hdc,&pfd); //检索DC匹配的像素格式
	SetPixelFormat(hdc,nPixelFormat,&pfd); //设置像素格式
	hRC=wglCreateContext(hdc); //创建OpenGL渲染场境
	wglMakeCurrent(hdc,hRC); //设置当前态
	InitScene(); //初始化OpenGL显示场景
	wglMakeCurrent(0,0); //取消当前态
	ReleaseDC(hWnd,hdc); //释放DC
	
	return 0;
}
LRESULT OnPaint(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	//重绘消息处理
	PAINTSTRUCT ps;
	HDC hdc;
	hdc=BeginPaint(hWnd,&ps); //客户区准备绘图
	wglMakeCurrent(hdc,hRC); //设置当前态
	game.Draw(); //显示OpenGL场景
	SwapBuffers(hdc); //交换缓冲区
	wglMakeCurrent(0,0); //取消当前态
	EndPaint(hWnd,&ps); //客户区绘图结束
	return 0; //处理了这条消息必须返回0
}
LRESULT OnSize(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	int cx, cy;
	cx = LOWORD(lParam), cy = HIWORD(lParam); //宽度值和高度值
	if(cx==0 || cy==0) return 0;
	hdc=GetDC(hWnd); //客户区DC
	wglMakeCurrent(hdc,hRC); //设置当前态
	ChangeSize(cx,cy); //OpenGL视区缩放
	wglMakeCurrent(0,0); //取消当前态
	ReleaseDC(hWnd,hdc); //释放DC
	return 0; //处理了这条消息必须返回0
}
LRESULT OnCommand(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	int wmId, wmEvent;
	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
		// 分析菜单选择:
	switch (wmId)
	{
		case IDM_ABOUT:
			MessageBoxA(hWnd,"About:\n4DExplorer ver 1.0\ncfrpg@mail.nwpu.edu.cn","4DExplorer",MB_OK);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT OnControl(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	game.UpdateControl(message,wParam,lParam);
	return 0;
}
void OnIdle()
{
	//应用程序空闲过程处理
	game.Update();//更新画面
	HDC hdc;
	//if (!bAnimate) return ;
	//yRotation += 1.0;
	hdc=GetDC(hWnd); //客户区DC
	if (hdc) {
		wglMakeCurrent(hdc,hRC); //设置当前态
		game.Draw(); //显示OpenGL场景
		SwapBuffers(hdc); //交换缓冲区
		wglMakeCurrent(0,0); //取消当前态
	}
	ReleaseDC(hWnd,hdc); //释放DC
}
#define AP 0.1f
#define DP 0.5f
#define SP 0.2f
void InitScene()
{
	//初始化OpenGL显示场景，设置灯光，材质属性
	GLfloat	ambientProperties[]={AP,AP,AP,1.0f};
	GLfloat	diffuseProperties[]={DP,DP,DP,1.0f};
	GLfloat	specularProperties[]={SP,SP,SP,1.0f};
	glPolygonMode(GL_FRONT,GL_FILL); //设置多边形正面为轮廓线
	glPolygonMode(GL_BACK,GL_FILL); //设置多边形背面为轮廓线
	glShadeModel(GL_FLAT); //设置平直明暗处理模型
	glEnable(GL_NORMALIZE); //设置单位法线
	glClearDepth(1.0); //深度缓冲清除值
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientProperties); //设置环境光参数
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseProperties); //设置散射光参数
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularProperties); //设置镜面光参数
	
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1.0); //设置光照模型
	//glEnable(GL_LIGHT0); //使用光源
	//glEnable(GL_LIGHTING); //允许光照
	gluOrtho2D(0,700,550,0);
	game.Initialize(hWnd);
}
void ChangeSize(int w,int h)
{	//OpenGL视区缩放
	GLfloat fAspect;
	fAspect = (h==0) ? 1.0f : (GLfloat)w/h;
	glViewport(0,0,w,h); //定义视区
	glMatrixMode(GL_PROJECTION); //指定投影矩阵定义修剪空间
	glLoadIdentity(); //加载单位矩阵
	gluPerspective(30,fAspect,1,15.0); //定义透视投影矩阵
	glMatrixMode(GL_MODELVIEW); //指定模型视图矩阵
	glLoadIdentity(); //加载单位矩阵
	glDrawBuffer(GL_BACK); //使用后台缓冲绘图
	glEnable(GL_DEPTH_TEST); //启用深度测试
}