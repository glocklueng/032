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
/// set def id so don't need this	ON_MESSAGE(DM_GETDEFID, OnGetDefID)
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

		if (pDlg->m_Simulate == false )
			pDlg->m_Serial.Write( userInput8.GetString() );

		userInput8.ReleaseBuffer();

	}
}


void CTextDump::OnBnClickedSaveText()
{
	int nNumItems = m_TextOutput.GetCount() ;

	if ( nNumItems == 0 ) {
		return;
	}

	CString filename = ::GetSaveFile( _T("Supported Files Types(*.txt)\0*.txt\0\0"),_T("Choose filename to save commands too"),_T("") );

	if (filename.GetLength() == 0 ) 
		return;

	if( filename.Find(L".txt") == -1 ) {
		filename.Append(L".txt");
	}
	
	CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex))
    {
        ex.ReportError();
        return;
    }

TRY
	    {
        for (int i = 0; i < nNumItems; i++)
        {
        
				CString temp;
				m_TextOutput.GetText(i,temp);
                File.WriteString(temp);
        }

        File.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to save grid list"));
        return ;
    }
    END_CATCH
}


void CTextDump::OnBnClickedClear()
{
	m_TextOutput.ResetContent();
}
