// DeleteFolder.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

deque<wstring> g_dequeFolder;
CKCritSec g_lockDequeFolder;

deque<wstring> g_dequeFile;
CKCritSec g_lockDequeFile;

void RecursiveDeleteFile(const wstring& strTarget)
{
	WIN32_FIND_DATA fd;
	wchar_t szTempFileFind[512] = { 0 };
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
	swprintf_s(szTempFileFind, L"%s\\*.*", strTarget.c_str());
	HANDLE hFind = FindFirstFile(szTempFileFind, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return ;

	while(FindNextFileW(hFind, &fd))
	{
		if (_tcsicmp(fd.cFileName,L".")==0)
		{
			continue;						
		}
		else if (_tcsicmp(fd.cFileName,L"..")==0)
		{
			continue;
		}
		else
		{
			if ( (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)//����Ŀ¼
			{
				wstring strFile = strTarget + L"\\";
				strFile += fd.cFileName;

				DeleteFile(strFile.c_str());
			}
			else
			{
				wstring strFolder = strTarget + L"\\";
				strFolder += fd.cFileName;				

				RecursiveDeleteFile(strFolder);
			}
		}

	}//while
	FindClose(hFind);

	RemoveDirectoryW(strTarget.c_str());
}

wstring GetHeaderFolder()
{
	CKAutoLock theLock(&g_lockDequeFolder);
	if (g_dequeFolder.size()>0)
	{
		wstring strFolder = g_dequeFolder.front().c_str();
		g_dequeFolder.pop_front();
		return strFolder;
	}
	
	return L"";
}
wstring GetTailFolder()
{
	CKAutoLock theLock(&g_lockDequeFolder);
	if (g_dequeFolder.size() >0)
	{
		wstring strFolder = g_dequeFolder.back().c_str();
		g_dequeFolder.pop_back();
		return strFolder;
	}

	return L"";
}

DWORD WINAPI DeleteHeaderFolderThreadFunc(LPVOID pParam)
{
	wstring strFile;
	while (true)
	{
		strFile = GetHeaderFolder();
		if (strFile.empty())
		{
			fprintf(stdout,"����Ϊ�գ�ɾ�����\r\n");
			break;
		}
		else
		{
			RecursiveDeleteFile(strFile);
			fprintf(stdout, "Folder header delete %s\r\n",_A(strFile));
		}
	}

	return 0;
}
DWORD WINAPI DeleteTailFolderThreadFunc(LPVOID pParam)
{
	wstring strFile;
	while (true)
	{
		strFile = GetTailFolder();
		if (strFile.empty())
		{
			fprintf(stdout,"����Ϊ�գ�ɾ�����\r\n");
			break;
		}
		else
		{
			RecursiveDeleteFile(strFile);
			fprintf(stdout, "Folder tail delete %s\r\n",_A(strFile));
		}
	}
	return 0;
}

wstring GetHeaderFile()
{
	CKAutoLock theLock(&g_lockDequeFile);
	if (g_dequeFile.size()>0)
	{
		wstring strFile = g_dequeFile.front().c_str();
		g_dequeFile.pop_front();
		return strFile;
	}

	return L"";
}
wstring GetTailFile()
{
	CKAutoLock theLock(&g_lockDequeFile);
	if (g_dequeFile.size() >0)
	{
		wstring strFile = g_dequeFile.back().c_str();
		g_dequeFile.pop_back();
		return strFile;
	}

	return L"";
}
DWORD WINAPI DeleteHeaderFileThreadFunc(LPVOID pParam)
{
	wstring strFile;
	while (true)
	{
		strFile = GetHeaderFile();
		if (strFile.empty())
		{
			fprintf(stdout,"����Ϊ�գ�ɾ�����\r\n");
			break;
		}
		else
		{
			DeleteFile(strFile.c_str());
			fprintf(stdout, "header delete %s\r\n",_A(strFile));
		}
	}

	return 0;
}
DWORD WINAPI DeleteTailFileThreadFunc(LPVOID pParam)
{
	wstring strFile;
	while (true)
	{
		strFile = GetTailFile();
		if (strFile.empty())
		{
			fprintf(stdout,"����Ϊ�գ�ɾ�����\r\n");
			break;
		}
		else
		{
			DeleteFile(strFile.c_str());
			fprintf(stdout, "tail delete %s\r\n",_A(strFile));
		}
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr,"param error.\r\n");
		return -1;
	}

	wstring strTarget = argv[1];

	fprintf(stdout,"��ʼ����%sĿ¼�µ��ļ����ļ���,�����ĵȴ�\r\n",_A(strTarget));

	WIN32_FIND_DATA fd;
	wchar_t szTempFileFind[512] = { 0 };
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
	swprintf_s(szTempFileFind, L"%s\\*.*", strTarget.c_str());
	HANDLE hFind = FindFirstFile(szTempFileFind, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	
	while(FindNextFileW(hFind, &fd))
	{
		if (_tcsicmp(fd.cFileName,L".")==0)
		{
			continue;						
		}
		else if (_tcsicmp(fd.cFileName,L"..")==0)
		{
			continue;
		}
		else
		{
			if ( (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)//�ļ�
			{
				wstring strFile = strTarget + L"\\";
				strFile += fd.cFileName;				

				g_dequeFile.push_back(strFile);
			}
			else	//Ŀ¼
			{
				wstring strFolder = strTarget + L"\\";
				strFolder += fd.cFileName;				

				g_dequeFolder.push_back(strFolder);
			}
		}
	}//while
	FindClose(hFind);
	
	fprintf(stdout,"�� %d���ļ��У�%d���ļ� Ҫɾ��,����Y(y)����\r\n",g_dequeFolder.size(),g_dequeFile.size());
	char chKey = getchar();
	if (chKey=='Y' || chKey=='y')
	{
		fprintf(stdout,"��ʼִ��ɾ��\r\n");
	}
	else
	{
		fprintf(stdout,"����%c�������˳�\r\n",chKey);
		return 0;
	}

	unsigned int dwThrdId = 0;
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))DeleteHeaderFolderThreadFunc,0,0,&dwThrdId);
	HANDLE hThread2 = (HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))DeleteTailFolderThreadFunc,0,0,&dwThrdId);

	HANDLE hThread3 = (HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))DeleteHeaderFileThreadFunc,0,0,&dwThrdId);
	HANDLE hThread4 = (HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))DeleteTailFileThreadFunc,0,0,&dwThrdId);
	
	WaitForSingleObject(hThread1,INFINITE);
	WaitForSingleObject(hThread2,INFINITE);
	WaitForSingleObject(hThread3,INFINITE);
	WaitForSingleObject(hThread4,INFINITE);
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);

	fprintf(stdout,"ִ����ϣ��˳�\r\n");
	return 0;
}

