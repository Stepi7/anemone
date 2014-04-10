// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <ios>
#include <tchar.h>

#include "TransEngine.h"
#include "TextRenderer.h"
#include "TextProcess.h"

// GDIPlus
#include <GdiPlus.h>
#pragma comment(lib,"GdiPlus.lib")
using namespace Gdiplus;

// STL
#include <string>

// hWnd ����
struct _hWnds
{
	HINSTANCE hInst;
	HWND Main;
	HWND Setting;
	HWND View;
	HWND Clip;
};
extern _hWnds hWnds;

// Ŭ���� ����
struct _Class
{
	CTextRenderer *TextRenderer;
	CTransEngine *TransEngine;
	CTextProcess *TextProcess;
};
extern _Class Cl;

struct render_attr
{
	FontFamily *Font;
	int Size_A;
	int Size_B;
	int Size_C;
	int Size_S;
};

static render_attr *rdName;
static render_attr *rdOriginal;
static render_attr *rdTrans;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
INT IsWritableMemory(LPVOID pMemoryAddr);
INT IsReadableMemory(LPVOID pMemoryAddr);