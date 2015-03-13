#include "MsgBox.h"

MsgBox::MsgBox()
{
}

MsgBox::~MsgBox()
{
}

BOOL  MsgBox::Create(LPTSTR lpClsName, HINSTANCE hInstance, HWND hParent)
{
	m_lpClsName = lpClsName;
	m_hInst = hInstance;

	int scrX = GetSystemMetrics(SM_CXSCREEN);
	int scrY = GetSystemMetrics(SM_CYSCREEN);

	RECT rtClient = { 0, 0, 600, 400 };
	DWORD wStyle = WS_POPUP;
	//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
	if (!AdjustWindowRect(&rtClient, wStyle, FALSE))
	{
		return false;
	}

	int rtWidth = rtClient.right - rtClient.left;
	int rtHeight = rtClient.bottom - rtClient.top;	

	POINT point;
	point.x = (scrX - rtWidth) / 2;
	point.y = (scrY - rtHeight) / 2;

	m_hWnd = CreateWindow(
		m_lpClsName,
		TEXT("MsgBox"),
		wStyle,
		point.x, point.y,
		rtWidth, rtHeight,
		hParent,
		NULL,
		m_hInst,
		NULL);

	if (m_hWnd == NULL)
	{
		return FALSE;
	}

	GetClientRect(m_hWnd, &rtClient);

	int marge = 50;
	point.x = rtClient.left + marge;
	point.y = rtClient.top + marge;
	rtWidth = rtClient.right - rtClient.left - 2 * marge;
	rtHeight = rtClient.bottom - rtClient.top - 2 * marge;

	m_edtMsg = CreateWindow(
		TEXT("STATIC"),
		NULL,
		SS_CENTER | WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		point.x, point.y,
		rtWidth, rtHeight,
		m_hWnd,
		(HMENU)IDC_MSGSTATIC,
		m_hInst,
		NULL);
	if (m_edtMsg == NULL)
	{
		OutDebugMsg("CreateWindow m_edtMsg == NULL Error:%d", GetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL MsgBox::ShowWindow(LPSTR msg)
{
	SetWindowTextA(m_edtMsg, msg);

	BOOL bRet = FALSE;
	bRet = ::UpdateWindow(m_hWnd);
	if (bRet == FALSE)
	{
		return FALSE;
	}

	SetWindowPos(m_hWnd, HWND_TOPMOST, 
		0, 0, 0, 0, 
		SWP_NOSIZE | SWP_NOMOVE);

	bRet = ::AnimateWindow(
		m_hWnd,
		2000,
		AW_CENTER | AW_BLEND
		);
	if (bRet == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

LRESULT MsgBox::MsgLoopStart()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT MsgBox::WndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return OnCreate(hWnd, wParam, lParam);
	case WM_CLOSE:
		return OnClose(hWnd, wParam, lParam);
	case WM_DESTROY:
		return OnDestroy(hWnd, wParam, lParam);
	case WM_ERASEBKGND:
		return OnEraseBkgnd(hWnd, wParam, lParam);
	case WM_PAINT:
		return OnPaint(hWnd, wParam, lParam);
	case WM_DRAWITEM:
		return OnDrawItem(hWnd, wParam, lParam);
	case WM_COMMAND:
		return OnCommand(hWnd, wParam, lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd, wParam, lParam);
	case WM_CTLCOLORSTATIC:
		return OnCtlColorStatic(hWnd, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT MsgBox::OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);
	return S_OK;
}

LRESULT MsgBox::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_CREATE, wParam, lParam);
}

LRESULT MsgBox::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(FALSE);
	return DefWindowProc(hWnd, WM_DESTROY, wParam, lParam);
}

LRESULT MsgBox::OnEraseBkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;
	HDC hBitdc = CreateCompatibleDC(hdc);

	RECT wndRect;
	GetClientRect(hWnd, &wndRect);

	HBITMAP hBitMap = NULL;
	hBitMap = (HBITMAP)LoadImage(NULL, TEXT("MsgBoxBkg.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitMap != NULL)
	{
		SelectObject(hBitdc, hBitMap);
		BitBlt(hdc, wndRect.left, wndRect.top, wndRect.right, wndRect.bottom,
		hBitdc, 0, 0, SRCCOPY);
		DeleteDC(hBitdc);
	}
	else
	{
		OutDebugMsg("LoadBitmap is NULL, MsgBoxBkg.bmp(600*400), Error:%d", GetLastError());
		DeleteDC(hBitdc); 
		return DefWindowProc(hWnd, WM_ERASEBKGND, wParam, lParam);
	}
	
	return S_OK;
}

LRESULT MsgBox::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_PAINT, wParam, lParam);
}

LRESULT MsgBox::OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_DRAWITEM, wParam, lParam);
}

LRESULT MsgBox::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UINT uId = LOWORD(wParam);
	UINT uEvent = HIWORD(wParam);

	switch (uId)
	{
	default:
		return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
	}

	return S_OK;
}

LRESULT MsgBox::OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);
	return S_OK;
}

LRESULT MsgBox::OnCtlColorStatic(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int nId = GetDlgCtrlID((HWND)lParam);
	int n = 0;
	LOGFONT logft;
	HFONT hFont;
	switch (nId)
	{
	case 0:
		n = GetLastError();
		break;
	case IDC_MSGSTATIC:
		ZeroMemory(&logft, sizeof(logft));
		logft.lfWidth = 10;							//字体宽度
		logft.lfHeight = 17;							//字体高度
		logft.lfWeight = FW_BLACK;						//字体粗细
		logft.lfItalic = FALSE;						//是否斜体
		logft.lfUnderline = FALSE;						//是否有下划线
		logft.lfStrikeOut = FALSE;						//是否有删除线
		logft.lfCharSet = ANSI_CHARSET;					//字符集
		logft.lfOutPrecision = OUT_DEFAULT_PRECIS;			//输出精度
		logft.lfClipPrecision = CLIP_DEFAULT_PRECIS;			//剪切精度
		logft.lfQuality = DEFAULT_QUALITY;				//输出质量
		logft.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;	//字体的间距和家族
		wcscpy_s(logft.lfFaceName, 32, L"黑体");					//字体类型
		hFont = CreateFontIndirect(&logft);
		SelectObject((HDC)wParam, hFont);
		//SendMessage((HWND)lParam, WM_SETFONT, (WPARAM)hFont, 0);
		SetTextColor((HDC)wParam, RGB(255, 0, 255));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LRESULT)((HBRUSH)GetStockObject(NULL_BRUSH));
	default:
		break;
	}
	return DefWindowProc(hWnd, WM_CTLCOLORSTATIC, wParam, lParam);
}
