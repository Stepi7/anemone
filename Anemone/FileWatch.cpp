#include "stdafx.h"
#include "FileWatch.h"

CFileWatch *CFileWatch::m_pThis = NULL;
std::vector<std::wstring> fileList;

CFileWatch::CFileWatch()
{
	SECURITY_ATTRIBUTES ThreadAttributes;
	ThreadAttributes.bInheritHandle = false;
	ThreadAttributes.lpSecurityDescriptor = NULL;
	ThreadAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);

	hWatchThread = CreateThread(&ThreadAttributes, 0, FileChangeNotifyThread, NULL, 0, NULL);
	if (hWatchThread == NULL)
	{
		MessageBox(0, L"������ ���� �۾��� �����߽��ϴ�.", 0, MB_ICONERROR);
	}
}

CFileWatch::~CFileWatch()
{
	TerminateThread(hWatchThread, 0);
}

DWORD CFileWatch::_FileChangeNotifyThread(LPVOID lpParam)
{

	HWND hwnd = (HWND)lpParam;
	UINT m_nTimerID;

	std::wstring Path;
	GetLoadPath(Path);

	HANDLE hDir = CreateFile(Path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
	CONST DWORD cbBuffer = 1024 * 1024;
	BYTE* pBuffer = (PBYTE)malloc(cbBuffer);
	BOOL bWatchSubtree = FALSE;
	DWORD dwNotifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION;
	DWORD bytesReturned;

	m_nTimerID = timeSetEvent(500, 0, (LPTIMECALLBACK)FileChangeNotifyProc, 0, TIME_PERIODIC);

	wchar_t temp[MAX_PATH] = { 0 };
	for (;;)
	{
		FILE_NOTIFY_INFORMATION* pfni;
		BOOL fOk = ReadDirectoryChangesW(hDir, pBuffer, cbBuffer,
			bWatchSubtree, dwNotifyFilter, &bytesReturned, 0, 0);
		if (!fOk)
		{
			DWORD dwLastError = GetLastError();
			break;
		}

		pfni = (FILE_NOTIFY_INFORMATION*)pBuffer;

		do {
			memcpy(temp, pfni->FileName, pfni->FileNameLength);
			temp[pfni->FileNameLength / 2] = 0;
			std::wstring filename(temp);
			transform(filename.begin(), filename.end(), filename.begin(), tolower);

			for (unsigned int i = 0; i <= fileList.size(); i++)
			{
				if (i == fileList.size())
				{
					fileList.push_back(filename.c_str());
					break;
				}
				else if (fileList[i].compare(filename.c_str()) == 0) break;
			}

			pfni = (FILE_NOTIFY_INFORMATION*)((PBYTE)pfni + pfni->NextEntryOffset);
		} while (pfni->NextEntryOffset > 0);
	}

	return 0;
}

MMRESULT CFileWatch::_FileChangeNotifyProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2)
{
	bool c_config;
	bool c_anedic;

	c_config = false;
	c_anedic = false;

	if (fileList.size() == 0) return 0;

	//WriteLog(L"1�ʿ� �ѹ� �̺�Ʈ\n");
	for (unsigned int i = 0; i<fileList.size(); i++)
	{
		if (fileList[i].compare(L"anemone.ini") == 0)
		{
			c_config = true;
		}

		if (fileList[i].compare(L"anedic.txt") == 0)
		{
			c_anedic = true;
		}
	}
	fileList.clear();

	// ���� ��尡 �۵����϶� ���Ϻ����
	if (c_config == true)
	{
		//MessageBox(0, L"����", L"anemone.ini", 0);
		Cl.Config->LoadConfig();

		PostMessage(hWnds.Main, WM_PAINT, 0, 0);
		//PostMessage(hSettingWnd, WM_USER, UM_REFRESH_SETTING, 0);
	}

	if (c_anedic == true)
	{
		// �Ƴ׸�� ���� �ٽ��б�
		//PostMessage(hWnds.Main, UM_ReloadUserDict, 0, 0);
	}

	return 0;
}
