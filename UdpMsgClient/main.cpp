# define _WINSOCK_DEPRECATED_NO_WARNINGS
# pragma comment(lib, "ws2_32.lib")
# include <iostream>
# include <WinSock2.h>
# include <vector>
# include <string>
using namespace std;

BOOL ResolString(vector<string> &vtSerIp, string strIp);
void OutDebugMsg(LPSTR msg, ...);

# define MAX_BUF 1024
SOCKET g_sMain = INVALID_SOCKET;
u_short g_usPort = 12345;
vector<string> g_vtAllIp;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	ResolString(g_vtAllIp, lpCmdLine);

	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA WSAData;
	int nRet = 0;
	nRet = WSAStartup(wVersionRequested, &WSAData);
	if (nRet != 0)
	{
		return 0;
	}
	
	g_sMain = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (g_sMain == INVALID_SOCKET)
	{
		return 0;
	}

	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(SOCKADDR_IN));
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(g_usPort);

	char strBuf[MAX_BUF] = {0};
	strcpy_s(strBuf, g_vtAllIp[g_vtAllIp.size() - 1].c_str());
	g_vtAllIp.pop_back();

	int addrSize = sizeof(SOCKADDR);
	vector<string>::iterator vtIter = g_vtAllIp.begin();
	while (vtIter != g_vtAllIp.end())
	{
		sockAddr.sin_addr.S_un.S_addr = inet_addr((*vtIter).c_str());

		nRet = sendto(g_sMain, strBuf, sizeof(strBuf), 0, (SOCKADDR*)&sockAddr, addrSize);
		if (nRet > 0)
		{
			OutDebugMsg("向服务端发送消息成功。IP:%s", inet_ntoa(sockAddr.sin_addr));
		}
		vtIter++;
	}

	WSACleanup();
	return 0;
}

BOOL ResolString(vector<string> &vtSerIp, string strIp)
{
	char *strBuf = (char *)malloc(strIp.length() + 1);
	strcpy_s(strBuf, strIp.length() + 1, strIp.c_str());
	char *lpStrPos = NULL;
	char *lpStr = NULL;
	lpStr = strtok_s(strBuf, ";", &lpStrPos);
	while (lpStr != NULL)
	{
		vtSerIp.push_back(lpStr);
		lpStr = strtok_s(NULL, ";", &lpStrPos);
	}

	free(strBuf);
	return TRUE;
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
