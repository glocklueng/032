// CameraView.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "CameraView.h"
#include "afxdialogex.h"


// CCameraView dialog

IMPLEMENT_DYNAMIC(CCameraView, CDialogEx)

CCameraView::CCameraView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraView::IDD, pParent)
{

}

CCameraView::~CCameraView()
{
}

void CCameraView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraView, CDialogEx)
END_MESSAGE_MAP()


// CCameraView message handlers

/**
 * OnInitDialog - Does initialise camera
 *
 * @param none used
 * @return 
 */
BOOL CCameraView::OnInitDialog()
{
	CRect rect,recta;

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_CAMERA)->GetWindowRect(rect);
	GetDlgItem(IDC_CAMERA)->GetWindowRect(recta);
	
	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_CameraWindow.oglCreate( rect, recta, this, 1);

	m_CameraWindow.SetTimer(1, CAMERA_DEFAULT_UPDATE_RATE_MS, 0);

	return true;
}
