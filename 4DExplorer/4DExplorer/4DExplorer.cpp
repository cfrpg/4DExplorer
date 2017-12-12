// 4DExplorer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "4DExplorer.h"
#include <gl\glut.h>
#include <Game.h>

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

struct WIN_MSGMAP_ENTRY { //��Ϣӳ���ṹ
	UINT nMessage; //��Ϣ��
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
	ON_MSG(WM_DESTROY, OnDestroy) //����������Ϣӳ��
	ON_MSG(WM_CREATE, OnCreate) //���ڴ�����Ϣӳ��
	ON_MSG(WM_PAINT, OnPaint) //�ػ���Ϣӳ��
	ON_MSG(WM_SIZE, OnSize) //���ڴ�С�仯��Ϣӳ��
	ON_MSG(WM_COMMAND, OnCommand) //�˵���Ϣӳ��
	ON_MSG(WM_MOUSEMOVE, OnControl) //����ƶ���Ϣӳ��
	ON_MSG(WM_LBUTTONDOWN,OnControl)
	ON_MSG(WM_LBUTTONUP,OnControl)
	ON_MSG(WM_KEYDOWN,OnControl)
	ON_MSG(WM_KEYUP,OnControl)
END_MESSAGE_MAP()

HGLRC hRC; //OpenGL��Ⱦ����
void InitScene(); //��ʼ��OpenGL��ʾ����
void ChangeSize(int w,int h); //OpenGL��������
 HWND hWnd;
 Game game;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR    lpCmdLine,int  nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;
	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY4DEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY4DEXPLORER));

	// ����Ϣѭ��:
	while (TRUE)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) 
		{ //������������Ϣ
			if (!GetMessage(&msg,NULL,0,0)) break; //WM_QUIT������Ϣѭ��
			//����ݼ�ת��Ϊ��Ӧ�Ĳ˵�����
			if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)){
				TranslateMessage(&msg); //�������¼�ת�����ַ���Ϣ
				DispatchMessage(&msg);//�ַ���Ϣ�����ڵ���Ϣ������ȥִ��
			}
		}
		OnIdle(); //����Ϣʱ������й���		
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WORD i;
	for(i=0;_messageEntries[i].nMessage; i++) 
	{
		//�����Ϣӳ������и���Ϣ������ö�Ӧ����Ϣ������
		if (message==_messageEntries[i].nMessage) 
		{
			if(message!=WM_DESTROY)//��WM_DESTROY��Ϣ�������������
				return (*_messageEntries[i].pfn)(hWnd,message,wParam,lParam);
			//�����WM_DESTROY��Ϣ���������أ���������
			(*_messageEntries[i].pfn)(hWnd,message,wParam,lParam);
			break;
		}
	}
	if (message==WM_DESTROY) 
	{ //WM_DESTROY����
		PostQuitMessage(0); //����WM_QUIT��Ϣ������Ӧ�ó���
		return 0; //������������Ϣ���뷵��0
	}
	return DefWindowProc(hWnd,message,wParam,lParam); //������Ϣ����Ĭ�ϴ���
}

// �����ڡ������Ϣ�������
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
	//���ڴ�����Ϣ����
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;
	HDC hdc;
	//�������ظ�ʽ
	ZeroMemory(&pfd,sizeof(pfd));//ȫ��������Ϊ0
	pfd.nSize		=sizeof(pfd);
	pfd.nVersion		=1;
	pfd.dwFlags		=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType	=PFD_TYPE_RGBA;
	pfd.cColorBits	=24;
	pfd.cDepthBits	=16;
	pfd.iLayerType	=PFD_MAIN_PLANE;
	hdc=GetDC(hWnd); //�ͻ���DC
	nPixelFormat=ChoosePixelFormat(hdc,&pfd); //����DCƥ������ظ�ʽ
	SetPixelFormat(hdc,nPixelFormat,&pfd); //�������ظ�ʽ
	hRC=wglCreateContext(hdc); //����OpenGL��Ⱦ����
	wglMakeCurrent(hdc,hRC); //���õ�ǰ̬
	InitScene(); //��ʼ��OpenGL��ʾ����
	wglMakeCurrent(0,0); //ȡ����ǰ̬
	ReleaseDC(hWnd,hdc); //�ͷ�DC
	
	return 0;
}
LRESULT OnPaint(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	//�ػ���Ϣ����
	PAINTSTRUCT ps;
	HDC hdc;
	hdc=BeginPaint(hWnd,&ps); //�ͻ���׼����ͼ
	wglMakeCurrent(hdc,hRC); //���õ�ǰ̬
	game.Draw(); //��ʾOpenGL����
	SwapBuffers(hdc); //����������
	wglMakeCurrent(0,0); //ȡ����ǰ̬
	EndPaint(hWnd,&ps); //�ͻ�����ͼ����
	return 0; //������������Ϣ���뷵��0
}
LRESULT OnSize(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	int cx, cy;
	cx = LOWORD(lParam), cy = HIWORD(lParam); //���ֵ�͸߶�ֵ
	if(cx==0 || cy==0) return 0;
	hdc=GetDC(hWnd); //�ͻ���DC
	wglMakeCurrent(hdc,hRC); //���õ�ǰ̬
	ChangeSize(cx,cy); //OpenGL��������
	wglMakeCurrent(0,0); //ȡ����ǰ̬
	ReleaseDC(hWnd,hdc); //�ͷ�DC
	return 0; //������������Ϣ���뷵��0
}
LRESULT OnCommand(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	int wmId, wmEvent;
	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
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
	//Ӧ�ó�����й��̴���
	game.Update();//���»���
	HDC hdc;
	//if (!bAnimate) return ;
	//yRotation += 1.0;
	hdc=GetDC(hWnd); //�ͻ���DC
	if (hdc) {
		wglMakeCurrent(hdc,hRC); //���õ�ǰ̬
		game.Draw(); //��ʾOpenGL����
		SwapBuffers(hdc); //����������
		wglMakeCurrent(0,0); //ȡ����ǰ̬
	}
	ReleaseDC(hWnd,hdc); //�ͷ�DC
}
#define AP 0.1f
#define DP 0.5f
#define SP 0.2f
void InitScene()
{
	//��ʼ��OpenGL��ʾ���������õƹ⣬��������
	GLfloat	ambientProperties[]={AP,AP,AP,1.0f};
	GLfloat	diffuseProperties[]={DP,DP,DP,1.0f};
	GLfloat	specularProperties[]={SP,SP,SP,1.0f};
	glPolygonMode(GL_FRONT,GL_FILL); //���ö��������Ϊ������
	glPolygonMode(GL_BACK,GL_FILL); //���ö���α���Ϊ������
	glShadeModel(GL_FLAT); //����ƽֱ��������ģ��
	glEnable(GL_NORMALIZE); //���õ�λ����
	glClearDepth(1.0); //��Ȼ������ֵ
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientProperties); //���û��������
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseProperties); //����ɢ������
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularProperties); //���þ�������
	
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1.0); //���ù���ģ��
	//glEnable(GL_LIGHT0); //ʹ�ù�Դ
	//glEnable(GL_LIGHTING); //�������
	gluOrtho2D(0,700,550,0);
	game.Initialize(hWnd);
}
void ChangeSize(int w,int h)
{	//OpenGL��������
	GLfloat fAspect;
	fAspect = (h==0) ? 1.0f : (GLfloat)w/h;
	glViewport(0,0,w,h); //��������
	glMatrixMode(GL_PROJECTION); //ָ��ͶӰ�������޼��ռ�
	glLoadIdentity(); //���ص�λ����
	gluPerspective(30,fAspect,1,15.0); //����͸��ͶӰ����
	glMatrixMode(GL_MODELVIEW); //ָ��ģ����ͼ����
	glLoadIdentity(); //���ص�λ����
	glDrawBuffer(GL_BACK); //ʹ�ú�̨�����ͼ
	glEnable(GL_DEPTH_TEST); //������Ȳ���
}