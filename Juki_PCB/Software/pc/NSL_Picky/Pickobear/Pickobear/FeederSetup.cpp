// FeederSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "FeederSetup.h"
#include "afxdialogex.h"


// CFeederSetup dialog

IMPLEMENT_DYNAMIC(CFeederSetup, CDialog)

CFeederSetup::CFeederSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CFeederSetup::IDD, pParent)
{

}

CFeederSetup::~CFeederSetup()
{
}

void CFeederSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFeederSetup, CDialog)
END_MESSAGE_MAP()


// CFeederSetup message handlers
