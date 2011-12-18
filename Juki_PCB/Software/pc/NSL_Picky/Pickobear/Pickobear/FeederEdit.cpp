// FeederEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "FeederEdit.h"
#include "afxdialogex.h"


// FeederEdit dialog

IMPLEMENT_DYNAMIC(FeederEdit, CDialogEx)

FeederEdit::FeederEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(FeederEdit::IDD, pParent)
	, m_FeederName(_T(""))
{

}

FeederEdit::~FeederEdit()
{
}

void FeederEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FEEDER_NAME, m_FeederName);
	DDX_Text(pDX, IDC_XPOS, entry.x);
	DDX_Text(pDX, IDC_YPOS, entry.y);
	DDX_Text(pDX, IDC_LX, entry.lx);
	DDX_Text(pDX, IDC_LY, entry.ly);
	DDX_Text(pDX, IDC_ROT, entry.rot);
	DDX_Text(pDX, IDC_FCOUNTX, entry.countx);
	DDX_Text(pDX, IDC_FCOUNTY, entry.county);
	DDX_Text(pDX, IDC_TOOLBOX, entry.tool);
	DDX_Text(pDX, IDC_COUNT_EDIT, entry.componentIndex );
}


BEGIN_MESSAGE_MAP(FeederEdit, CDialogEx)
	ON_CBN_SELCHANGE(IDC_TOOLBOX, &FeederEdit::OnCbnSelchangeToolbox)
END_MESSAGE_MAP()


// FeederEdit message handlers


void FeederEdit::OnCbnSelchangeToolbox()
{

}
