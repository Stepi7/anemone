// Anemone.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Anemone.h"

#define MAX_LOADSTRING 100

// ���� ����:
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
_hWnds hWnds;
_Class Cl;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(
		__in HINSTANCE hInstance,
		__in_opt HINSTANCE hPrevInstance,
		__in_opt LPWSTR lpCmdLine,
		__in int nCmdShow
	)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ANEMONE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, false))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANEMONE));

	// ������Ʈ������ �������� ��� ������
	HKEY key;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;

	wchar_t *szPath = (wchar_t *)malloc(256*2);
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\ChangShin\\ezTrans", 0, KEY_READ, &key);

	if (RegQueryValueEx(key, L"FilePath", 0, &dwType, (LPBYTE)szPath, &dwSize) != ERROR_SUCCESS)
	{
		MessageBox(0, L"����Ʈ������ ��ġ���� �ʾҰų� ������Ʈ������ ����Ʈ���� ��θ� ã�� �� �����ϴ�.\r\n����Ʈ������ ��ġ�Ǿ� �־�� �Ƴ׸�� ������ �����մϴ�.\r\n����Ʈ������ ��ġ�Ǿ� �ִٸ� INI ������ ����Ʈ���� ��θ� ������ �ּ���.", 0, MB_ICONERROR);
		return false;
	}

	// �������� �ʱ�ȭ
	std::wstring szEnginePath = szPath;
	Cl.TransEngine = new CTransEngine();

	if (!Cl.TransEngine->Init(szEnginePath))
	{
		MessageBox(0, L"����Ʈ���� ���� �ʱ�ȭ�� �����߽��ϴ�.", 0, MB_ICONERROR);
		return false;
	}
	/*
	char buf[1024];
	char *n_str;

	// ���� ���������ϴ� ������ �־ ���� ����
	for (int k = 0; k < 3; k++)
	{
		n_str = (char *)TransEngine.J2K_TranslateMMNT(0, "�u��낵����΍��x�Q�l�ł��b���Ȃǂł��Ȃ��ł��傤���v");
		if (n_str[0] != 0x00) break;
	}

	if (n_str)
	{
		strcpy_s(buf, n_str);
		TransEngine.J2K_FreeMem(n_str);
		MessageBoxA(0, buf, 0, 0);
	}
	*/
	
	// ������ �ʱ�ȭ
	Cl.TextRenderer = new CTextRenderer();
	if (!Cl.TextRenderer->Init())
	{
		MessageBox(0, L"GDI+ ���� �ʱ�ȭ�� �����߽��ϴ�.\r\n�ý����� ���ҽ��� ������ ��� GDI+ �ʱ�ȭ�� ������ �� �ֽ��ϴ�.\r\n����ǰ� �ִ� �ٸ� ���α׷����� �����ϰ� �ٽ� �����ϼ���.", 0, MB_ICONERROR);
		return false;
	}

	// �ؽ�Ʈ ó�� Ŭ����
	Cl.TextProcess = new CTextProcess();

	// ������ ǥ��
	ShowWindow(hWnds.Main, true);

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete Cl.TransEngine;
	delete Cl.TextRenderer;
	delete Cl.TextProcess;

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANEMONE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hWnds.hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   int cx = GetSystemMetrics(SM_CXSCREEN);
   int cy = GetSystemMetrics(SM_CYSCREEN);

   int x = 300;
   int y = 200;

   hWnds.Main = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOPMOST, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      (cx-x)/2, (cy-y)/2, x, y, NULL, NULL, hInstance, NULL);

   if (!hWnds.Main)
   {
      return FALSE;
   }

   ShowWindow(hWnds.Main, nCmdShow);
   UpdateWindow(hWnds.Main);
   
   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
	{
	}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hWnds.hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		Cl.TextRenderer->Paint();
	}
		break;
	case WM_MOVING:
	case WM_SIZING:
	{
		RECT *prc = (RECT *)lParam;
		SetWindowPos(hWnd, NULL, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top, 0);
		Cl.TextRenderer->Paint();
	}
		break;
	case WM_DRAWCLIPBOARD:
	{
		Cl.TextProcess->OnDrawClipboard();
	}
		break;
	case WM_LBUTTONDOWN:
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;
	case WM_ERASEBKGND:
		return false;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO *mm = (MINMAXINFO *)lParam;

		mm->ptMinTrackSize.x = 90;
		mm->ptMinTrackSize.y = 90;
	}
		break;
	case WM_NCHITTEST:
	{
		POINT pt;
		RECT rc;
		GetClientRect(hWnd, &rc);
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		ScreenToClient(hWnd, &pt);

		int BORDERWIDTH = 30;

		/*top-left, top and top-right*/
		if (pt.y<BORDERWIDTH)
		{
			if (pt.x<BORDERWIDTH)
			{
				return HTTOPLEFT;
			}
			else if (pt.x>(rc.right - BORDERWIDTH))
			{
				return HTTOPRIGHT;
			}    return HTTOP;
		}
		/*bottom-left, bottom and bottom-right */
		if (pt.y>(rc.bottom - BORDERWIDTH))
		{
			if (pt.x<BORDERWIDTH)
			{
				return HTBOTTOMLEFT;
			}
			else if (pt.x>(rc.right - BORDERWIDTH))
			{
				return HTBOTTOMRIGHT;
			}    return HTBOTTOM;
		}  if (pt.x<BORDERWIDTH)
		{
			return HTLEFT;
		}  if (pt.x>(rc.right - BORDERWIDTH))
		{
			return HTRIGHT;
		}  return DefWindowProc(hWnd, message, wParam, lParam);
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
