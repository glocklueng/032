
// BitsplitDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CBitSplitDlg dialog
class CBitSplitDlg : public CDialogEx
{
// Construction
public:
	CBitSplitDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BITSPLIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HBITMAP LoadBitmap(LPCTSTR szFilename) 
	{ 
		ASSERT(szFilename);
		HBITMAP hBitmap = NULL; 
		hBitmap = (HBITMAP)LoadImage(NULL, szFilename, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
		return hBitmap; 
	}
	int GetCBitmapWidth(const CImage & cbm)
	{
		return cbm.GetWidth();
	}

	int GetCBitmapHeight(const CImage & cbm)
	{
		return cbm.GetHeight();
	}

  int GetCBitmapWidth(const CBitmap & cbm)
   {
      BITMAP bm;
      cbm.GetObject(sizeof(BITMAP),&bm);
      return bm.bmWidth;
   }

   int GetCBitmapHeight(const CBitmap & cbm)
   {
      BITMAP bm;
      cbm.GetObject(sizeof(BITMAP),&bm);
      return bm.bmHeight;
   }
	

	BOOL CreateImage( CImage *newbitmap, CImage *source_bitmap, COLORREF low_level, COLORREF high_level);
	CString m_Filename;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void DrawPicture();
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_NumberLayers;
	afx_msg void OnBnClickedLoadbmp();
	afx_msg void OnBnClickedSaveImages();
	afx_msg void OnBnClickedExportDxf();
	CStatic m_ImageControl;
	CImage m_Bitmap;
	CEdit m_NumLevels;
	afx_msg void OnNMCustomdrawNumberOfLevels(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_BlackAndWhite;
};
