# define _WINSOCK_DEPRECATED_NO_WARNINGS
# pragma comment(lib, "ws2_32.lib")
# include <WinSock2.h>
# include <iostream>
# include <process.h>
# include "MsgBox.h"

void MsgBoxThread(void *arg);
void OutDebugMsg(LPSTR msg, ...);

# define MAX_BUF 1024
SOCKET g_sMain = INVALID_SOCKET;
u_short g_usPort = 12345;
LPTSTR g_lpClsName = TEXT("WndMsgCls");
HINSTANCE g_hInstance = NULL;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	//init socket-------------------------------------------------------------------------
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA WSAData;
	int nRet = WSAStartup(wVersionRequested, &WSAData);
	if (nRet != 0)
	{
		OutDebugMsg("WSAStartup Error:%d", WSAGetLastError());
		return 0;
	}

	//init WndClass----------------------------------------------------------------------
	g_hInstance = hInstance;

	WNDCLASS wnd;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	wnd.hInstance = g_hInstance;
	wnd.lpfnWndProc = MsgBox::WndMsgProc;
	wnd.lpszClassName = g_lpClsName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW | CS_VREDRAW;//重要！！要不然背景绘制会出问题

	if (!RegisterClass(&wnd))
	{
		OutDebugMsg("RegisterClass Error:%d", GetLastError());
		return 0;
	}

	//create socket------------------------------------------------------------------
	g_sMain = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (g_sMain == INVALID_SOCKET)
	{
		OutDebugMsg("socket Error:%d", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(SOCKADDR_IN));
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(g_usPort);

	int bRet = 0;
	bRet = bind(g_sMain, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
	if (bRet == SOCKET_ERROR)
	{
		OutDebugMsg("bind Error:%d", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	//recv msg -----------------------------------------------------------------------
	char strBuf[MAX_BUF] = { 0 };
	int addrSize = sizeof(SOCKADDR);
	while (TRUE)
	{
		OutDebugMsg("服务端等待接受消息。");
		//ZeroMemory(strBuf, MAX_BUF);
		nRet  = recvfrom(g_sMain, strBuf, MAX_BUF, 0, (SOCKADDR*)&sockAddr, &addrSize);
		if (nRet > 0)
		{
			_beginthread(MsgBoxThread, 0, strBuf);
		}
		else
		{
			OutDebugMsg("服务端接受消息 <= 0, Error:%d。", WSAGetLastError());
		}
	}

	WSACleanup();
	return 0;
}

void MsgBoxThread(void *arg)
{
	char strBuf[MAX_BUF] = { 0 };
	strcpy_s(strBuf, (char *)arg);

	MsgBox msg;
	msg.Create(g_lpClsName, g_hInstance);
	msg.ShowWindow(strBuf);
	msg.MsgLoopStart();
	OutDebugMsg("Window Close");
	_endthread();
}

void OutDebugMsg(LPSTR msg, ...)
{

	CHAR szBuffer[1024];
	ZeroMemory(szBuffer, 1024);
	int nWritten = 0;
	va_list args;
	va_start(args, msg);//获取msg后面的参数列表
	nWritten = vsnprintf_s(szBuffer, 1024, msg, args);
	va_end(args);//释放

	strcat_s(szBuffer, "[DbgMsg]");

# ifdef _DEBUG
	OutputDebugStringA(szBuffer);
# else
	OutputDebugStringA(szBuffer);
# endif
}
