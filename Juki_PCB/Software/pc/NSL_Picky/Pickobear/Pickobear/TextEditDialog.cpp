// TextEditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "TextEditDialog.h"
#include "afxdialogex.h"


// CTextEditDialog dialog

IMPLEMENT_DYNAMIC(CTextEditDialog, CDialogEx)

CTextEditDialog::CTextEditDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTextEditDialog::IDD, pParent)
	, m_Text(_T(""))
{

}

CTextEditDialog::~CTextEditDialog()
{
}

void CTextEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXTBOX, m_Text);
}


BEGIN_MESSAGE_MAP(CTextEditDialog, CDialogEx)
END_MESSAGE_MAP()


// CTextEditDialog message handlers
