# define _WINSOCK_DEPRECATED_NO_WARNINGS
# pragma comment(lib, "ws2_32.lib")
# include <iostream>
# include <WinSock2.h>

using namespace std;

SOCKET g_sMain = INVALID_SOCKET;
u_short g_usPort = 12345;
char *g_serIp = "192.168.3.110";

void OutDebugMsg(LPSTR msg, ...);

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

	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(SOCKADDR_IN));
	sockAddr.sin_addr.S_un.S_addr = inet_addr(g_serIp);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(g_usPort);

	char strMsgBuf[1024];

	while (true)
	{
		OutDebugMsg("开始链接服务端！");
		g_sMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (g_sMain == INVALID_SOCKET)
		{
			return 0;
		}

		int nRet = 0;

		do{
			nRet = connect(g_sMain, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
			Sleep(2000);
		} while (nRet == SOCKET_ERROR);

		ZeroMemory(strMsgBuf, 1024);
		while (true)
		{
			OutDebugMsg("准备接受消息！");
			nRet = recv(g_sMain, strMsgBuf, 1024, 0);
			if (nRet > 0)
			{
				MessageBoxA(NULL, strMsgBuf, "来自服务端的消息", MB_OK);
			}
			else
			{
				closesocket(g_sMain);
				break;
			}
		}
	}

	WSACleanup();
	return 0;
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
