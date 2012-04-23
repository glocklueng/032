//-----------------------------------------------------------------------------
// Routines for the user interface when doing interactive things with prox
// cards; this is basically a command line thing, in one window, and then
// another window to do the graphs.
// Jonathan Westhues, Sept 2005
//-----------------------------------------------------------------------------
#include <windows.h>
#include <limits.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "prox.h"

#define oops() do { \
	char line[100]; \
	sprintf(line, "Internal error at line %d file '%s'", __LINE__, \
		__FILE__); \
	MessageBox(NULL, line, "Error", MB_ICONERROR); \
	exit(-1); \
} while(0)

void dbp(char *str, ...)
{
	va_list f;
	char buf[1024];
	va_start(f, str);
	vsprintf(buf, str, f);
	OutputDebugString(buf);
	OutputDebugString("\n");
}

int GraphBuffer[MAX_GRAPH_TRACE_LEN];
int GraphTraceLen;
int PlotGridX, PlotGridY;

HPEN GreyPenLite, GreyPen, GreenPen, WhitePen, YellowPen;
HBRUSH GreenBrush, YellowBrush;

static int GraphStart = 0;
static double GraphPixelsPerPoint = 1;

static int CursorAPos;
static int CursorBPos;
double CursorScaleFactor = 1.0;
static HPEN CursorAPen;
static HPEN CursorBPen;

static HWND CommandWindow;
static HWND GraphWindow;
static HWND ScrollbackEdit;
static HWND CommandEdit;

#define COMMAND_HISTORY_MAX 16
static char CommandHistory[COMMAND_HISTORY_MAX][256];
static int CommandHistoryPos = -1;
static int CommandHistoryNext;

static HFONT MyFixedFont;
#define FixedFont(x) SendMessage((x), WM_SETFONT, (WPARAM)MyFixedFont, TRUE)

void ExecCmd(char *cmd)
{
}

int CommandFinished;
int offset = 64;

static void ResizeCommandWindow(void)
{
	int w, h;
	RECT r;
	GetClientRect(CommandWindow, &r);
	w = r.right - r.left;
	h = r.bottom - r.top;
	MoveWindow(ScrollbackEdit, 10, 10, w - 20, h - 50, TRUE);
	MoveWindow(CommandEdit, 10, h - 29, w - 20, 22, TRUE);
}

void RepaintGraphWindow(void)
{
	InvalidateRect(GraphWindow, NULL, TRUE);
}

static LRESULT CALLBACK
	CommandWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_DESTROY:
		case WM_QUIT:
			exit(0);
			return 0;

		case WM_SIZE:
			ResizeCommandWindow();
			return 0;

		case WM_SETFOCUS:
			SetFocus(CommandEdit);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 1;
}

static void PaintGraph(HDC hdc)
{
	RECT r;
	HBRUSH brush;
	HPEN pen;
	char str[250];
	int yMin = INT_MAX;
	int yMax = INT_MIN;
	int yMean = 0;
	int startMax = 0;
	int absYMax = 1;
	int n = 0, i = 0;

	brush = GreenBrush;
	pen = GreenPen;

	GetClientRect(GraphWindow, &r);
	int zeroHeight = (r.top + r.bottom) >> 1;

	// plot X and Y grid lines
	if ((PlotGridX > 0) && ((PlotGridX * GraphPixelsPerPoint) > 1)) {
		for(i = offset; i < r.right; i += (int)(PlotGridX * GraphPixelsPerPoint)) {
			SelectObject(hdc, GreyPenLite);
			MoveToEx(hdc, r.left + i, r.top, NULL);
			LineTo(hdc, r.left + i, r.bottom);
		}
  }

	if ((PlotGridY > 0) && ((PlotGridY * GraphPixelsPerPoint) > 1)){
		for(i = 0; i < ((r.top + r.bottom)>>1); i += (int)(PlotGridY * GraphPixelsPerPoint)) {
			SelectObject(hdc, GreyPenLite);
			MoveToEx(hdc, r.left, zeroHeight + i, NULL);
			LineTo(hdc, r.right, zeroHeight + i);
			MoveToEx(hdc, r.left, zeroHeight - i, NULL);
			LineTo(hdc, r.right, zeroHeight - i);
		}
  }

	// print vertical separator white line on the left of the window
	SelectObject(hdc, WhitePen);
	MoveToEx(hdc, r.left + offset, r.top, NULL);
	LineTo(hdc, r.left + offset, r.bottom);

	// print horizontal grey zero axis line
	SelectObject(hdc, GreyPen);
	MoveToEx(hdc, r.left, zeroHeight, NULL);
	LineTo(hdc, r.right, zeroHeight);

	startMax = (GraphTraceLen - (int)((r.right - r.left - offset) / GraphPixelsPerPoint));
	// check boundaries
	if(startMax < 0) startMax = 0;
	if(GraphStart > startMax) GraphStart = startMax;
	if(GraphStart < 0) GraphStart = 0;


	SelectObject(hdc, pen);

	// go over the portion of the graph to be displayed and find the largest
	// absolute value which will be used to auto scale the graph when displayed
	for(i = GraphStart; ; i++) {
		if(i >= GraphTraceLen) {
			break;
		}
		if(fabs((double)GraphBuffer[i]) > absYMax) {
			absYMax = (int)fabs((double)GraphBuffer[i]);
		}
		int x = offset + (int)((i - GraphStart)*GraphPixelsPerPoint);
		if(x > r.right) {
			break;
		}
	}

	absYMax = (int)(absYMax*1.2 + 1);
	SelectObject(hdc, MyFixedFont);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(0, 0, 0));

	// number of points that will be plotted
	double span = (int)((r.right - r.left) / GraphPixelsPerPoint);

	// one label every offset pixels, let us say
	int labels = (r.right - r.left - offset) / offset;
	if(labels <= 0) labels = 1;
	// round to nearest power of 2
	int pointsPerLabel = (int)(log(span / labels)/log(2.0));
	if(pointsPerLabel <= 0) pointsPerLabel = 1;
	pointsPerLabel = (int)pow(2.0,pointsPerLabel);

	// go over the graph and plot samples and labels
	for(i = GraphStart; ; i++) {
		if(i >= GraphTraceLen) {
			break;
		}
		int x = offset + (int)((i - GraphStart)*GraphPixelsPerPoint);
		if(x > r.right + GraphPixelsPerPoint) {
			break;
		}

		int y = GraphBuffer[i];
		if(y < yMin) yMin = y;
		if(y > yMax) yMax = y;
		yMean += y;
		n++;

		y = (y * (r.top - r.bottom) / (2*absYMax)) + zeroHeight;
		if(i == GraphStart) {
			MoveToEx(hdc, x, y, NULL);
		} else {
			LineTo(hdc, x, y);
		}

		if(GraphPixelsPerPoint > 10) {
			RECT f;
			f.left = x - 3;
			f.top = y - 3;
			f.right = x + 3;
			f.bottom = y + 3;
			FillRect(hdc, &f, brush);
		}

		// plot labels
		if(((i - GraphStart) % pointsPerLabel == 0) && i != GraphStart) {
			SelectObject(hdc, WhitePen);
			MoveToEx(hdc, x, zeroHeight - 8, NULL);
			LineTo(hdc, x, zeroHeight + 8);

			sprintf(str, "+%d", i);
			SIZE size;
			GetTextExtentPoint32(hdc, str, strlen(str), &size);
			TextOut(hdc, x - size.cx, zeroHeight + 8, str, strlen(str));

			SelectObject(hdc, pen);
			MoveToEx(hdc, x, y, NULL);
		}

		// plot measurement cursors
		if(i == CursorAPos || i == CursorBPos) {
			if(i == CursorAPos) {
				SelectObject(hdc, CursorAPen);
			} else {
				SelectObject(hdc, CursorBPen);
			}
			MoveToEx(hdc, x, r.top, NULL);
			LineTo(hdc, x, r.bottom);

			SelectObject(hdc, pen);
			MoveToEx(hdc, x, y, NULL);
		}
	}

	if(n != 0) {
		yMean /= n;
	}

	// print misc information at bottom of graph window
	sprintf(str, "@%d   max=%d min=%d mean=%d n=%d/%d    dt=%d [%.3f] zoom=%.3f CursorA=%d [%d] CursorB=%d [%d]",
		GraphStart, yMax, yMin, yMean, n, GraphTraceLen,
		CursorBPos - CursorAPos, (CursorBPos - CursorAPos)/CursorScaleFactor, GraphPixelsPerPoint,
		CursorAPos, GraphBuffer[CursorAPos], CursorBPos, GraphBuffer[CursorBPos]);
	TextOut(hdc, 50, r.bottom - 20, str, strlen(str));
}

static LRESULT CALLBACK
	GraphWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_DESTROY:
		case WM_QUIT:
			GraphWindow = NULL;
			return DefWindowProc(hwnd, msg, wParam, lParam);

		case WM_SIZE:
			RepaintGraphWindow();
			return 0;

		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			if(GraphStart < 0) {
				GraphStart = 0;
			}
			// This draws the trace.
			PaintGraph(hdc);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_KEYDOWN:
			switch(wParam) {
				case VK_DOWN:
					if(GraphPixelsPerPoint <= 8) {
						GraphPixelsPerPoint *= 2;
					}
					break;

				case VK_UP:
					if(GraphPixelsPerPoint >= 0.01) {
						GraphPixelsPerPoint /= 2;
					}
					break;

				case VK_RIGHT:
					if(GraphPixelsPerPoint < 16) {
						GraphStart += (int)(16 / GraphPixelsPerPoint);
					} else {
						GraphStart++;
					}
					break;

				case VK_LEFT:
					if(GraphPixelsPerPoint < 16) {
						GraphStart -= (int)(16 / GraphPixelsPerPoint);
					} else {
						GraphStart--;
					}
					break;

				default:
					goto nopaint;
			}
			RepaintGraphWindow();
nopaint:
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN: {
			int x = LOWORD(lParam);
			x -= offset;
			x = (int)(x / GraphPixelsPerPoint);
			x += GraphStart;

			if(msg == WM_LBUTTONDOWN) {
				CursorAPos = x;
			} else {
				CursorBPos = x;
			}
			RepaintGraphWindow();
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 1;
}

void PrintToScrollback(char *fmt, ...)
{
	va_list f;
	char str[1024];
	strcpy(str, "\r\n");
	va_start(f, fmt);
	vsprintf(str+2, fmt, f);

	static char TextBuf[1024*32];
	SendMessage(ScrollbackEdit, WM_GETTEXT, (WPARAM)sizeof(TextBuf),
		(LPARAM)TextBuf);

	if(strlen(TextBuf) + strlen(str) + 1 <= sizeof(TextBuf)) {
		strcat(TextBuf, str);
	} else {
		lstrcpyn(TextBuf, str, sizeof(TextBuf));
	}

	SendMessage(ScrollbackEdit, WM_SETTEXT, 0, (LPARAM)TextBuf);
	SendMessage(ScrollbackEdit, EM_LINESCROLL, 0, (LPARAM)INT_MAX);
}

void ShowGraphWindow(void)
{
	if(GraphWindow) return;

	GraphWindow = CreateWindowEx(0, "Graph", "graphed",
		WS_OVERLAPPED | WS_BORDER | WS_MINIMIZEBOX | WS_SYSMENU |
		WS_SIZEBOX | WS_VISIBLE, 200, 150, 600, 500, NULL, NULL, NULL,
		NULL);
	if(!GraphWindow) oops();
}

void HideGraphWindow(void)
{
	if(GraphWindow) {
		DestroyWindow(GraphWindow);
		GraphWindow = NULL;
	}
}

static void SetCommandEditTo(char *str)
{
	SendMessage(CommandEdit, WM_SETTEXT, 0, (LPARAM)str);
	SendMessage(CommandEdit, EM_SETSEL, strlen(str), strlen(str));
}

void ShowGui()
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);

	wc.style			= CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)CommandWindowProc;
	wc.hInstance		= NULL;
	wc.hbrBackground	= (HBRUSH)(COLOR_BTNSHADOW);
	wc.lpszClassName	= "Command";
	wc.lpszMenuName		= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);

	if(!RegisterClassEx(&wc)) oops();

	wc.lpszClassName	= "Graph";
	wc.lpfnWndProc		= (WNDPROC)GraphWindowProc;
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);

	if(!RegisterClassEx(&wc)) oops();

	CommandWindow = CreateWindowEx(0, "Command", "prox",
		WS_OVERLAPPED | WS_BORDER | WS_MINIMIZEBOX | WS_SYSMENU |
		WS_SIZEBOX | WS_VISIBLE, 20, 20, 500, 400, NULL, NULL, NULL,
		NULL);
	if(!CommandWindow) oops();

	ScrollbackEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | ES_MULTILINE |
		ES_AUTOVSCROLL | WS_VSCROLL, 0, 0, 0, 0, CommandWindow, NULL,
		NULL, NULL);

	CommandEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
		WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VISIBLE |
		ES_AUTOHSCROLL, 0, 0, 0, 0, CommandWindow, NULL, NULL, NULL);

	MyFixedFont = CreateFont(14, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FF_DONTCARE, "Lucida Console");
	if(!MyFixedFont)
		MyFixedFont = (HFONT)GetStockObject(SYSTEM_FONT);

	FixedFont(ScrollbackEdit);
	FixedFont(CommandEdit);

	ResizeCommandWindow();
	SetFocus(CommandEdit);

	PrintToScrollback(">> Started prox, built " __DATE__ " " __TIME__);
	PrintToScrollback(">> Connected to device");

	GreyPenLite = CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
	GreyPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	GreenPen = CreatePen(PS_SOLID, 1, RGB(100, 255, 100));
	YellowPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	GreenBrush = CreateSolidBrush(RGB(100, 255, 100));
	YellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	WhitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	CursorAPen = CreatePen(PS_DASH, 1, RGB(255, 255, 0));
	CursorBPen = CreatePen(PS_DASH, 1, RGB(255, 0, 255));

	MSG msg;
	for(;;) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN) {
				char got[1024];
				SendMessage(CommandEdit, WM_GETTEXT, (WPARAM)sizeof(got),
					(LPARAM)got);

				if(strcmp(got, "cls")==0) {
					SendMessage(ScrollbackEdit, WM_SETTEXT, 0, (LPARAM)"");
				} else {
					CommandReceived(got);
				}
				SendMessage(CommandEdit, WM_SETTEXT, 0, (LPARAM)"");

				// Insert it into the command history, unless it is
				// identical to the previous command in the history.
				int prev = CommandHistoryNext - 1;
				if(prev < 0) prev += COMMAND_HISTORY_MAX;
				if(strcmp(CommandHistory[prev], got) != 0) {
					strcpy(CommandHistory[CommandHistoryNext], got);
					CommandHistoryNext++;
					if(CommandHistoryNext == COMMAND_HISTORY_MAX) {
						CommandHistoryNext = 0;
					}
				}
				CommandHistoryPos = -1;
			} else if(msg.message == WM_KEYDOWN && msg.wParam == VK_UP &&
				msg.hwnd == CommandEdit)
			{
				if(CommandHistoryPos == -1) {
					CommandHistoryPos = CommandHistoryNext;
				}
				CommandHistoryPos--;
				if(CommandHistoryPos < 0) {
					CommandHistoryPos = COMMAND_HISTORY_MAX-1;
				}
				SetCommandEditTo(CommandHistory[CommandHistoryPos]);
			} else if(msg.message == WM_KEYDOWN && msg.wParam == VK_DOWN &&
				msg.hwnd == CommandEdit)
			{
				CommandHistoryPos++;
				if(CommandHistoryPos >= COMMAND_HISTORY_MAX) {
					CommandHistoryPos = 0;
				}
				SetCommandEditTo(CommandHistory[CommandHistoryPos]);
			} else if(msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE &&
				msg.hwnd == CommandEdit)
			{
				SendMessage(CommandEdit, WM_SETTEXT, 0, (LPARAM)"");
			} else {
				if(msg.message == WM_KEYDOWN) {
					CommandHistoryPos = -1;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (!offline)
		{
			UsbCommand c;
			if(ReceiveCommandPoll(&c))
				UsbCommandReceived(&c);
		}

		Sleep(10);
	}
}
