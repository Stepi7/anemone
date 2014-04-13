// ���� ���� �Լ��� �־�δ� ���̺귯�� ����
#include "stdafx.h"
#include "Library.h"

void GetLoadPath(std::wstring &dir)
{
	wchar_t p[255];
	GetModuleFileNameW(hInst, p, 255);
	std::wstring f = p;
	dir = f.substr(0, f.rfind(L'\\'));
}

// ������Ʈ������ ����Ʈ���� ��θ� ���ɴϴ�
bool GetEZTPathFromREG(std::wstring &dir)
{
	wchar_t szPath[255];
	HKEY key;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;

	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\ChangShin\\ezTrans", 0, KEY_READ, &key);

	if (RegQueryValueEx(key, L"FilePath", 0, &dwType, (LPBYTE)szPath, &dwSize) != ERROR_SUCCESS) return false;

	dir = szPath;
	return true;
}

bool ReadINI_Str(const wchar_t *key, const wchar_t *section, wchar_t *buf, wchar_t *file)
{
	int n = GetPrivateProfileString(section, key, NULL, buf, 255, file);
	if (n == 0) {
		//WriteLog(L"������ �ҷ����� | ���ǵ��� ���� �μ� | %ls\n", key);
		return false;
	}
	//WriteLog(L"%ls = \"%ls\"\n", key, buf);
	return true;
}


bool WriteINI_Str(const wchar_t *key, const wchar_t *section, wchar_t *buf, wchar_t *file)
{
	int n = WritePrivateProfileString(section, key, buf, file);
	if (n == 0) {
		//WriteLog(L"������ �ҷ����� | ���ǵ��� ���� �μ� | %ls\n", key);
		return false;
	}
	//WriteLog(L"%ls = \"%ls\"\n", key, buf);
	return true;
}
