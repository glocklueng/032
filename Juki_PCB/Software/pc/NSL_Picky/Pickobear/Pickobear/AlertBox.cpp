// AlertBox.cpp : implementation file
//

#include "stdafx.h"
#include "AlertBox.h"
#include "afxdialogex.h"


// CAlertBox dialog

IMPLEMENT_DYNAMIC(CAlertBox, CDialogEx)

CAlertBox::CAlertBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlertBox::IDD, pParent)
{

}

CAlertBox::~CAlertBox()
{
}

void CAlertBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALERT_TEXT, m_AlertString);
}


BEGIN_MESSAGE_MAP(CAlertBox, CDialogEx)
END_MESSAGE_MAP()


// CAlertBox message handlers
