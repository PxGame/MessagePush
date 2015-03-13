# define _WINSOCK_DEPRECATED_NO_WARNINGS
# pragma comment(lib, "ws2_32.lib")
# include <iostream>
# include <WinSock2.h>
# include <vector>
# include <process.h>
# include <Ws2tcpip.h>

using namespace std;

SOCKET g_sMain = INVALID_SOCKET;
u_short g_usPort = 12345;
vector<SOCKET> g_allLinkClient;

void OutDebugMsg(LPSTR msg, ...);
void AcceptThread(void *arg);

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA WSAData;
	int nRet = WSAStartup(wVersionRequested, &WSAData);
	if (nRet != 0)
	{
		return 0;
	}

	g_sMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_sMain == INVALID_SOCKET)
	{
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
		return 0;
	}

	bRet = listen(g_sMain, SOMAXCONN);
	if (bRet == SOCKET_ERROR)
	{
		return 0;
	}

	_beginthread(AcceptThread, 0, NULL);

	MessageBoxA(NULL, "确定，发送消息！", "提示", MB_OK);
	char msg[1024];
	ZeroMemory(msg, 1024);
	strcpy_s(msg, "猜猜我是谁？");


	OutDebugMsg("开始发送数据！");
	vector<SOCKET>::iterator sockIter = g_allLinkClient.begin();
	while (sockIter != g_allLinkClient.end())
	{
		send(*sockIter, msg, 1024, 0);

		sockIter++;
	}

	WSACleanup();
	return 0;
}

void AcceptThread(void *arg)
{
	OutDebugMsg("服务端Accept线程开启。");
	while (true)
	{
		SOCKET clientSock;
		SOCKADDR_IN sockAddr;
		int length = sizeof(SOCKADDR_IN);
		ZeroMemory(&sockAddr, length);
		clientSock = accept(g_sMain, (SOCKADDR *)&sockAddr, &length);
		if (clientSock != INVALID_SOCKET)
		{
			g_allLinkClient.push_back(clientSock);
			OutDebugMsg("[ClientLink]IP:%s PORT:%d", inet_ntoa(sockAddr.sin_addr), ntohs(sockAddr.sin_port));
		}
		else
		{
			//error
		}
	}
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
