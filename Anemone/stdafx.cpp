// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// Anemone.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

// TODO: �ʿ��� �߰� �����
// �� ������ �ƴ� STDAFX.H���� �����մϴ�.

/*
IsWritableMemory : �־��� �޸� �ּ��� ���°� ���Ⱑ������ üũ�ϴ� �Լ�
Param :
LPVOID  pMemoryAddr : �˻��ϰ��� �ϴ� �޸��� �ּ�
Return Value :
ERROR_SUCCESS : ���ڷ� �־��� �ּ��� �޸𸮰� Read/Write ������ ���
��Ÿ �� : Read/Write ������ �޸𸮰� �ƴϸ� �ش� �ּ��� Protect Mode ��
��Ÿ���� 0�� �ƴ� ���� ������.
http://msdn.microsoft.com/en-us/library/aa915370.aspx ����
*/
INT IsWritableMemory(LPVOID pMemoryAddr)
{
	MEMORY_BASIC_INFORMATION    MemInfo = { 0, };
	SIZE_T  nResult = 0;

	nResult = VirtualQuery(pMemoryAddr, &MemInfo, sizeof(MemInfo));

	if (nResult == 0) // Ŀ�� ������ ��� VirtualQuery ��ü�� Fail��.  
	{
		return -1;
	}
	else if (MemInfo.Protect & (PAGE_EXECUTE_READWRITE | PAGE_READWRITE))
	{
		return  ERROR_SUCCESS;
	}
	else
	{
		return  MemInfo.Protect;
	}
}


/*
IsReadableMemory : �־��� �޸� �ּ��� ���°� ����(Read)�������� üũ�ϴ� �Լ�
Param :
LPVOID  pMemoryAddr : �˻��ϰ��� �ϴ� �޸��� �ּ�
Return Value :
ERROR_SUCCESS : ���ڷ� �־��� �ּ��� �޸𸮰� Read ������ ���
��Ÿ �� : Read ������ �޸𸮰� �ƴϸ� �ش� �ּ��� State ��
��Ÿ���� 0�� �ƴ� ���� ������.
http://msdn.microsoft.com/en-us/library/aa915370.aspx ����
*/
INT IsReadableMemory(LPVOID pMemoryAddr)
{
	MEMORY_BASIC_INFORMATION    MemInfo = { 0, };
	SIZE_T  nResult = 0;

	nResult = VirtualQuery(pMemoryAddr, &MemInfo, sizeof(MemInfo));

	if (nResult == 0) // Ŀ�� ������ ��� VirtualQuery ��ü�� Fail��.  
	{
		return -1;
	}
	else if (MemInfo.State & MEM_COMMIT)
	{
		return  ERROR_SUCCESS;
	}
	else
	{
		return  MemInfo.State;
	}
}