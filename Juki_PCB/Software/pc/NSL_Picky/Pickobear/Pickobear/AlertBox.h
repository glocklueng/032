#pragma once
#include "afxwin.h"

class CPickobearDlg;

// CAlertBox dialog

class CAlertBox : public CDialogEx
{
	DECLARE_DYNAMIC(CAlertBox)

public:
	CAlertBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAlertBox();
	 
	void CenterDialog(void)
   {
      CPoint   Point;
      CRect    DialogRect;
      CRect    ParentRect;
      int      nWidth;
      int      nHeight;
      CWnd     *DesktopWindow = NULL;
      CWnd     *MainWindow;

      // Get the size of the dialog box.
      GetWindowRect(DialogRect);

      // Get the main window.
      MainWindow = AfxGetApp()->m_pMainWnd;

      // Determine if the main window exists. This can be useful when
      // the application creates the dialog box before it creates the
      // main window. If it does exist, retrieve its size to center
      // the dialog box with respect to the main window.
      if (MainWindow != NULL)
         MainWindow->GetClientRect(ParentRect);
      // If the main window does not exist, center with respect to
      // the desktop window.
      else
      {
         DesktopWindow = GetDesktopWindow();
         DesktopWindow->GetWindowRect(ParentRect);
      }

      // Calculate the height and width for MoveWindow().
      nWidth = DialogRect.Width();
      nHeight = DialogRect.Height();

      // Find the center point and convert to screen coordinates.
      Point.x = ParentRect.Width() / 2;
      Point.y = ParentRect.Height() / 2;
      if (MainWindow)
         MainWindow->ClientToScreen(&Point);
      else
         DesktopWindow->ClientToScreen(&Point);

      // Calculate the new X, Y starting point.
      Point.x -= nWidth / 2;
      Point.y -= nHeight / 2;

      // Move the window.
      MoveWindow(Point.x, Point.y, nWidth, nHeight, FALSE);
   }	
	CPickobearDlg *pDlg;

// Dialog Data
	enum { IDD = IDD_ALERT_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_AlertString;
};
