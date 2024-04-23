// draw.cpp : Defines the entry point for the application.
//
#include <string.h>
#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <gdiplus.h>
#include <fstream>

#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;
int xt = 1;
int skip = 0;
int drawn = 0;
int tspeed = 10;
TCHAR text1[10];
TCHAR text2[10];
TCHAR text3[10];
TCHAR text4[10];
// buttons
HWND hWndmsg1;
HWND hWndmsg2;
HWND hwndButton;
HWND TextBox;
HWND TextBox2;
HWND TextBox3;
HWND TextBox4;
// sent data
int col = 0, amplituda = -30;
double kroki = 0;
double srednia = 0;
double czas_kroku = 0, czas_postoju = 0;
RECT drawArea1 = { 21, 51, 1402, 479 };
std::vector < double > a_x;
std::vector < double > a_y;
std::vector < double > a_z;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);
void steps()
{
	char buffer2[0xff];
	sprintf(buffer2, "Number of steps %f\n time of stop  %f s\n time of steps  %f s\n", kroki, czas_postoju, czas_kroku);
	MessageBoxA(hWndmsg1, buffer2, "Steps", MB_OK);
}
double srednia_kroczaca(int j)
{
	int i = 0;
	double suma = 0;
	for (; i < 25 && j < a_z.size(); i++)
	{
		suma += a_z[j];
		j++;
	}
	suma = suma / i;
	return suma;

}
void koniec_kroku(int& j, int& temp, int& temp2)
{
	for (int k = 0; j < a_z.size() - 6; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (abs(a_z[j + i] - a_z[j + i + 1]) < 0.005)
			{
				k++;
			}
		}
		if (k == 3)
		{
			czas_kroku = czas_kroku + j - temp;
			temp = 0;
			temp2 = j;
			kroki++;
			srednia = srednia_kroczaca(j);
			break;
		}
		else
		{
			k = 0;
		}
	}
}
void licz_kroki()
{
	int temp = 0, temp2 = 0;
	srednia = srednia_kroczaca(0);
	for (int j = 0; j < a_z.size(); j++)
	{
		if (a_z[j] - srednia > 0.2)
		{
			czas_postoju = czas_postoju + j - temp2;
			temp2 = 0;
			temp = j;
			koniec_kroku(j, temp, temp2);
		}
	}
	czas_kroku /= 25;
	czas_postoju /= 25;
}
void average()
{
	double avr = 0;
	for (int i = 0; i < a_x.size(); i++)
	{
		avr += sqrt(a_x[i] * a_x[i] + a_y[i] * a_y[i] + a_z[i] * a_z[i]);
	}

	avr *= 9.81;
	avr /= a_x.size();
	char buffer[0xff];
	sprintf(buffer, "the average is: %f\n", avr);
	MessageBoxA(hWndmsg2, buffer, "Average", MB_OK);
}

void x_t()
{
	int i = 0;
	int num = 0;
	while (text1[i] != 0)
	{
		num = (text1[i] - '0') + (num * 10);
		i++;
	}
	xt = num;
}
void amp()
{
	int i = 0;
	int num = 0;
	while (text2[i] != 0)
	{
		num = (text2[i] - '0') + (num * 10);
		i++;
	}
	amplituda = (-1) * num;
}
void data_skip()
{
	int i = 0;
	int num = 0;
	while (text3[i] != 0)
	{
		num = (text3[i] - '0') + (num * 10);
		i++;
	}
	skip = num;
}
void speed()
{
	int i = 0;
	int num = 0;
	while (text4[i] != 0)
	{
		num = (text4[i] - '0') + (num * 10);
		i++;
	}
	tspeed = num;
}
void drawG(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen_blue(Color(255, 0, 0, 255));
	double g1, g2;
	int border = 21;
	for (int i = 1 + skip; i < a_x.size(); i++)
	{
		g1 = amplituda * sqrt(a_x[i - 1] * a_x[i - 1] + a_y[i - 1] * a_y[i - 1] + a_z[i - 1] * a_z[i - 1]) + 270;
		g2 = amplituda * sqrt(a_x[i] * a_x[i] + a_y[i] * a_y[i] + a_z[i] * a_z[i]) + 270;
		graphics.DrawLine(&pen_blue, border, g1, border + xt, g2);
		border += xt;
	}
	drawn = 1;
	//graphics.DrawLine(&pen_blue, 21, 9.81 * 13 - 30, a_x.size(), 9.81 * (-4) +140);
}
void drawX(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen_red(Color(255, 255, 0, 0));
	double x1, x2;
	int border = 21;
	for (int i = 1 + skip; i < a_x.size(); i++)
	{
		x1 = (a_x[i - 1]) * amplituda + 270;
		x2 = (a_x[i]) * amplituda + 270;
		graphics.DrawLine(&pen_red, border, x1, border + xt, x2);
		border += xt;
	}
	drawn = 2;
	//graphics.DrawLine(&pen_blue, 21, 200, a_x.size(), 200);
}
void drawY(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen_green(Color(255, 0, 255, 0));
	double y1, y2;
	int border = 21;
	for (int i = 1 + skip; i < a_y.size(); i++)
	{
		y1 = (a_y[i - 1]) * amplituda + 270;
		y2 = (a_y[i]) * amplituda + 270;
		graphics.DrawLine(&pen_green, border, y1, border + xt, y2);

		border += xt;
	}
	drawn = 3;
	//graphics.DrawLine(&pen_blue, 21, 315, a_y.size(),315);
}
void drawZ(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen_blue(Color(255, 0, 0, 255));
	double z1, z2;
	int border = 21;
	for (int i = 1 + skip; i < a_y.size(); i++)
	{
		z1 = (a_z[i - 1]) * amplituda + 270;
		z2 = (a_z[i]) * amplituda + 270;
		graphics.DrawLine(&pen_blue, border, z1, border + xt, z2);

		border += xt;
	}
	drawn = 4;
}
void drawAll(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen_red(Color(255, 255, 0, 0)), pen_green(Color(255, 0, 255, 0)), pen_blue(Color(255, 0, 0, 255));
	//Pen pen2(Color(255, 25 * col, 0, 255));
	double z1, z2, y1, y2, x1, x2, g1, g2;
	int border = 21;

	for (int i = 1 + skip; i < a_y.size() - 1; i++)
	{
		z1 = (a_z[i - 1]) * amplituda + 270;
		z2 = (a_z[i]) * amplituda + 270;
		y1 = (a_y[i - 1]) * amplituda + 270;
		y2 = (a_y[i]) * amplituda + 270;
		x1 = (a_x[i - 1]) * amplituda + 270;
		x2 = (a_x[i]) * amplituda + 270;
		g1 = amplituda * sqrt(a_x[i - 1] * a_x[i - 1] + a_y[i - 1] * a_y[i - 1] + a_z[i - 1] * a_z[i - 1]) + 270;
		g2 = amplituda * sqrt(a_x[i] * a_x[i] + a_y[i] * a_y[i] + a_z[i] * a_z[i]) + 270;
		//g2 = -40 * sqrt(sqrt(a_x[i] * a_x[i] + a_y[i] * a_y[i]) * sqrt(a_x[i] * a_x[i] + a_y[i] * a_y[i]) + a_z[i] * a_z[i]);
		//graphics.DrawLine(&pen_blue, border, g1, border + 1, g2);
		graphics.DrawLine(&pen_red, border, x1, border + xt, x2);
		graphics.DrawLine(&pen_green, border, y1, border + xt, y2);
		graphics.DrawLine(&pen_blue, border, z1, border + xt, z2);
		border += xt;
	}
	drawn = 5;
}
void drawRectangles(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen3(Color(255, 0, 0, 0));
	graphics.DrawRectangle(&pen3, 20, 50, 1383, 500);

}
void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, int draw)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	if (draw == 1)
	{
		drawG(hdc);
	}
	else if (draw == 2)
	{
		drawX(hdc);
	}
	else if (draw == 3)
	{
		drawY(hdc);
	}
	else if (draw == 4)
	{
		drawZ(hdc);
	}
	else if (draw == 5)
	{
		drawAll(hdc);
	}

	EndPaint(hWnd, &ps);
}
void inputData()
{

	double temp;
	std::fstream plik;
	plik.open("outputRobotForwardA02.log", std::ios::in);
	while (!plik.eof())
	{
		for (int i = 0; i < 3; i++)
			plik >> temp; //niepotrzebne dane
		plik >> temp;
		a_x.push_back(temp);
		plik >> temp;
		a_y.push_back(temp);
		plik >> temp;
		a_z.push_back(temp);
		for (int i = 0; i < 6; i++)
			plik >> temp; //niepotrzebne dane
	}
	plik.close();
}
int OnCreate(HWND window)
{
	inputData();
	return 0;
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Draw g"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		0, 0,                                  // the left and top co-ordinates
		90, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Draw X"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 0,                                  // the left and top co-ordinates
		90, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Draw Y"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		200, 0,                                  // the left and top co-ordinates
		90, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Draw Z"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		300, 0,                                  // the left and top co-ordinates
		90, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON5,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Draw All"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		400, 0,                                  // the left and top co-ordinates
		90, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON3,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Show g"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1280, 0,                                  // the left and top co-ordinates
		90, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON10,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Steps"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1380, 0,                                  // the left and top co-ordinates
		90, 40,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON11,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);

	// create button and store the handle                                                       

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer ON"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		500, 0, 90, 40, hWnd, (HMENU)ID_RBUTTON1, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer OFF"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		600, 0, 90, 40, hWnd, (HMENU)ID_RBUTTON2, GetModuleHandle(NULL), NULL);

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1:
			repaintWindow(hWnd, hdc, ps, &drawArea1, 1);
			break;
		case ID_BUTTON2:
			repaintWindow(hWnd, hdc, ps, &drawArea1, 2);
			break;
		case ID_BUTTON3:
			repaintWindow(hWnd, hdc, ps, &drawArea1, 5);
			break;
		case ID_BUTTON4:
			repaintWindow(hWnd, hdc, ps, &drawArea1, 3);
			break;
		case ID_BUTTON5:
			repaintWindow(hWnd, hdc, ps, &drawArea1, 4);
			break;
		case ID_BUTTON6:
			GetWindowText(TextBox, text1, 2);
			x_t();
			break;
		case ID_BUTTON7:
			GetWindowText(TextBox2, text2, 5);
			amp();
			break;
		case ID_BUTTON8:
			GetWindowText(TextBox3, text3, 5);
			data_skip();
			break;
		case ID_BUTTON9:
			GetWindowText(TextBox4, text4, 10);
			speed();
			break;
		case ID_BUTTON10:
			average();
			break;
		case ID_BUTTON11:
			licz_kroki();
			steps();
			kroki = 0;
			czas_kroku = 0;
			czas_postoju = 0;
			break;
		case ID_RBUTTON1:
			SetTimer(hWnd, TMR_1, 25, 0);
			break;
		case ID_RBUTTON2:
			KillTimer(hWnd, TMR_1);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE: {
		TextBox = CreateWindow(TEXT("EDIT"), TEXT("1"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			700, 10, 60, 20,
			hWnd, (HMENU)NULL, NULL, NULL);

		CreateWindow(TEXT("BUTTON"), TEXT("x TIME"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			760, 0, 80, 40,
			hWnd, (HMENU)ID_BUTTON6, NULL, NULL);

		TextBox2 = CreateWindow(TEXT("EDIT"), TEXT("30"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			840, 10, 60, 20,
			hWnd, (HMENU)NULL, NULL, NULL);

		CreateWindow(TEXT("BUTTON"), TEXT("AMP"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			900, 0, 80, 40,
			hWnd, (HMENU)ID_BUTTON7, NULL, NULL);

		TextBox3 = CreateWindow(TEXT("EDIT"), TEXT("0"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			980, 10, 60, 20,
			hWnd, (HMENU)NULL, NULL, NULL);

		CreateWindow(TEXT("BUTTON"), TEXT("SKIP"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			1040, 0, 80, 40,
			hWnd, (HMENU)ID_BUTTON8, NULL, NULL);


		TextBox4 = CreateWindow(TEXT("EDIT"), TEXT("10"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			1120, 10, 60, 20,
			hWnd, (HMENU)NULL, NULL, NULL);

		CreateWindow(TEXT("BUTTON"), TEXT("T SPEED"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			1180, 0, 80, 40,
			hWnd, (HMENU)ID_BUTTON9, NULL, NULL);
		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		drawRectangles(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			switch (drawn)
			{
			case 1:
			{
				skip += tspeed;
				repaintWindow(hWnd, hdc, ps, &drawArea1, 1);
				break;
			}
			case 2:
			{
				skip += tspeed;
				repaintWindow(hWnd, hdc, ps, &drawArea1, 2);
				break;
			}
			case 3:
			{
				skip += tspeed;
				repaintWindow(hWnd, hdc, ps, &drawArea1, 3);
				break;
			}
			case 4:
			{
				skip += tspeed;
				repaintWindow(hWnd, hdc, ps, &drawArea1, 4);
				break;
			}
			case 5:
			{
				skip += tspeed;
				repaintWindow(hWnd, hdc, ps, &drawArea1, 5);
				break;
			}
			}
			if (skip > a_x.size() - 600)
				skip = 0;
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
