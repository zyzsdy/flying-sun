#include "stdafx.h"

STRUVER s = { 0 };
GAMELOGIC g = { 0 };
extern ObjectList o;

int GetRand(int n){
	return rand() % n;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd){
	//初始化全局变量
	wsprintf(s.szAppName, L"FlyingSun");
	wsprintf(s.szTitle, L"Flying Sun 1.0");
	srand((unsigned)GetTickCount());

	//建立主窗口
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	//wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hCursor = LoadCursorFromFile(L"Hand.cur");
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = s.szAppName;

	if (!RegisterClassEx(&wndClass)){
		MessageBox(NULL, L"无法打开本程序。", L"ERROR", MB_ICONERROR);
		return -1;
	}

	HWND hwnd = CreateWindow(s.szAppName, s.szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH + 16, WINDOW_HEIGHT + 39, true);
	//MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);
	//资源初始化
	if (!GameInit(hwnd)){
		MessageBox(hwnd, L"资源初始化失败", L"错误", MB_ICONERROR);
		return -1;
	}

	//消息处理部分
	MSG msg = { 0 };
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			s.tNow = GetTickCount();
			if (s.tNow - s.tPre >= 10)
				GamePaint(hwnd);
			if (s.tNow - s.tPreCtrl >= 1){
				GameControl(hwnd);
				EnemyBuild(hwnd);
			}
		}
	}
	UnregisterClass(s.szAppName, wndClass.hInstance);
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT paintStruct;

	switch (message){
	case WM_PAINT:
		BeginPaint(hwnd, &paintStruct);
		GamePaint(hwnd);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_MOUSEMOVE:
		s.iMouseX = LOWORD(lParam);
		s.iMouseY = HIWORD(lParam);
		if (g.page==PG_TITLE||g.page==PG_GAMEOVER){
			BOOL bPS = FALSE;
			if (s.iFocuMouseButton == 0) bPS = TRUE;
			s.iFocuMouseButton = IsMouseHintButton(s.iMouseX, s.iMouseY);
			if (s.iFocuMouseButton&&bPS)
				if(s.bSound) s.Sound->GameSoundplay(s.Sound->pSound_Mouseover);
		}
		break;
	case WM_LBUTTONDOWN:
		if (g.page == PG_TITLE || g.page == PG_GAMEOVER){
			s.bMouseKeydown = TRUE;
			if (s.iFocuMouseButton) if (s.bSound)
				s.Sound->GameSoundplay(s.Sound->pSound_Mouseclick);
		}
		break;
	case WM_LBUTTONUP:
		if (g.page == PG_TITLE || g.page == PG_GAMEOVER){
			s.bMouseKeydown = FALSE;
			TitleButtonDo(s.iFocuMouseButton, hwnd);
		}
		break;
	case WM_KEYDOWN:
		switch (g.page){
		case PG_TITLE:
			switch (wParam){
			case VK_RETURN:
				InitPage_Game(hwnd);
				break;
			}
			break;
		case PG_GAMERUNNING:
			switch (wParam){
			case VK_UP:
				g.ctlUP = TRUE;
				DiChange(hwnd, CHD_UP);
				break;
			case VK_LEFT:
				g.ctlLEFT = TRUE;
				DiChange(hwnd, CHD_LEFT);
				break;
			case VK_DOWN:
				g.ctlDOWN = TRUE;
				DiChange(hwnd, CHD_DOWN);
				break;
			case VK_RIGHT:
				g.ctlRIGHT = TRUE;
				DiChange(hwnd, CHD_RIGHT);
				break;
			case VK_SHIFT:
				if (g.ctlFIRE == FALSE){
					CreateObject(TP_MYBULLET);
					g.ctlFIRE = TRUE;
				}
				break;
			}
			break;
		case PG_GAMEOVER:
			switch (wParam){
			case VK_RETURN:
				InitPage_Title(hwnd);
				break;
			}
			break;
		}//end switch g.page
		//全局快捷键
		switch (wParam){
		case VK_F3:
			s.bShowInfo = !s.bShowInfo;
			break;
		case VK_F5:
			s.bFMode = !s.bFMode;
			break;
		case VK_F4:
			g.heart += 1;
			break;
		}
		break;
	case WM_KEYUP:
		switch (g.page){
		case PG_GAMERUNNING:
			switch (wParam){
			case VK_UP:
				g.ctlUP = FALSE;
				break;
			case VK_LEFT:
				g.ctlLEFT = FALSE;
				break;
			case VK_DOWN:
				g.ctlDOWN = FALSE;
				break;
			case VK_RIGHT:
				g.ctlRIGHT = FALSE;
				break;
			case VK_SHIFT:
				g.ctlFIRE = FALSE;
				break;
			}
			break;
		}
		break;
	case WM_DESTROY:
		GameCleanUp(hwnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}