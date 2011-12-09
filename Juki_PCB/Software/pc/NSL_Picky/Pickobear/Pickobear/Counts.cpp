// Counts.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "Counts.h"
#include "afxdialogex.h"


// CCounts dialog

IMPLEMENT_DYNAMIC(CCounts, CDialogEx)

CCounts::CCounts(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCounts::IDD, pParent)
	, m_CountX(0)
	, m_CountY(0)
{

}
BOOL CCounts::OnInitDialog() 
{
   BOOL b = CDialogEx::OnInitDialog();

   ( ( CWnd* ) GetDlgItem( IDC_COUNTX ) )->SetFocus() ;

   // must return false if focus set elsewhere
   return FALSE;
}
CCounts::~CCounts()
{
}

void CCounts::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COUNTX, m_CountX);
	DDV_MinMaxLong(pDX, m_CountX, 1, 1000);
	DDX_Text(pDX, IDC_COUNTY, m_CountY);
	DDV_MinMaxLong(pDX, m_CountY, 1, 1000);
}


BEGIN_MESSAGE_MAP(CCounts, CDialogEx)
END_MESSAGE_MAP()


// CCounts message handlers
