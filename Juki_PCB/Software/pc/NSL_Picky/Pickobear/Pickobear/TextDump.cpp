// TextDump.cpp : implementation file
//

#include "stdafx.h"
#include "Pickobear.h"
#include "TextDump.h"
#include "afxdialogex.h"


// CTextDump dialog

IMPLEMENT_DYNAMIC(CTextDump, CDialogEx)

CTextDump::CTextDump(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTextDump::IDD, pParent)
{

}

CTextDump::~CTextDump()
{
}

void CTextDump::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMMAND_INPUT, m_InputBuffer);
	DDX_Control(pDX, IDC_TEXTOUT, m_TextOutput);
}


BEGIN_MESSAGE_MAP(CTextDump, CDialogEx)
	ON_BN_CLICKED(IDC_SEND, &CTextDump::OnBnClickedSend)
	ON_BN_CLICKED(IDC_SAVE_TEXT, &CTextDump::OnBnClickedSaveText)
	ON_BN_CLICKED(IDC_CLEAR, &CTextDump::OnBnClickedClear)
END_MESSAGE_MAP()


// CTextDump message handlers


void CTextDump::OnBnClickedSend()
{
	UpdateData();

	if( m_InputBuffer.GetLength() )  {

		Print( m_InputBuffer );
		CStringA userInput8( UTF16toUTF8( m_InputBuffer ) );

		// add a return
		userInput8 += "\r\n";

		pDlg->m_Serial.Write( userInput8.GetString() );

		userInput8.ReleaseBuffer();

	}
}


void CTextDump::OnBnClickedSaveText()
{
}


void CTextDump::OnBnClickedClear()
{
	m_TextOutput.ResetContent();
}
