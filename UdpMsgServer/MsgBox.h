#pragma once
#include <Windows.h>
#include <string>
# include <tchar.h>
using namespace std;

void OutDebugMsg(LPSTR msg, ...);

# define IDC_MSGSTATIC 1000

class MsgBox
{
public:
	MsgBox();
	~MsgBox();

public:
	BOOL Create(LPTSTR lpClsName, HINSTANCE hInstance, HWND hParent = NULL);
	BOOL ShowWindow(LPSTR msg);
	LRESULT MsgLoopStart();

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HWND m_edtMsg;
	LPTSTR m_lpClsName;
	
public:
	static LRESULT CALLBACK WndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI OnCtlColorStatic(HWND hWnd, WPARAM wParam, LPARAM lParam);
};

