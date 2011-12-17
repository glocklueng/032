// EditComponent.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "EditComponent.h"
#include "afxdialogex.h"


// EditComponent dialog

IMPLEMENT_DYNAMIC(EditComponent, CDialogEx)

EditComponent::EditComponent(CWnd* pParent /*=NULL*/)
	: CDialogEx(EditComponent::IDD, pParent)
	, m_Name(_T(""))
	, m_Value(_T(""))
	, m_Type(_T(""))
	, m_Feeder(_T(""))
{

}

EditComponent::~EditComponent()
{
}

void EditComponent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FEEDER_NAME, m_Name);
	DDX_Text(pDX, IDC_FEEDER_NAME2, m_Value);
	DDX_Text(pDX, IDC_XPOS, entry.x);
	DDX_Text(pDX, IDC_YPOS, entry.y);
	DDX_Text(pDX, IDC_LX, m_Feeder);
	DDX_Text(pDX, IDC_LY,entry.side);
	DDX_Text(pDX, IDC_FCOUNTX, m_Type);
	DDX_Text(pDX, IDC_ROT, entry.rot);
	DDV_MinMaxLong(pDX, entry.rot, 0, 360);
}


BEGIN_MESSAGE_MAP(EditComponent, CDialogEx)
END_MESSAGE_MAP()


// EditComponent message handlers
