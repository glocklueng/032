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
	DDX_Control(pDX, IDC_FEEDER_GRID_EDIT, m_FeederGrid);
}

BEGIN_MESSAGE_MAP(CFeederSetup, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_FEEDER_GRID_EDIT, OnGridEndEdit)
END_MESSAGE_MAP()


BOOL  CFeederSetup::OnInitDialog(){
	CDialog::OnInitDialog();

	ResetFeederGrid();
	RebuildList();

	SetDefID(IDC_FEEDER_GRID_EDIT);
	return false;
}

// CFeederSetup message handlers
void CFeederSetup::ResetFeederGrid( void ) 
{
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );

	CRect rect;	
	CWnd *wnd = GetDlgItem(IDC_FEEDER_GRID_EDIT);
	if(wnd ) {
		wnd->GetClientRect( rect) ;
	}

	int nInterval =( rect.Width() / 10 );

	m_FeederGrid.DeleteAllItems();

	m_FeederGrid.SetRowCount( 1 );
	m_FeederGrid.SetColumnCount( 10 );
	m_FeederGrid.SetListMode( true );

	m_FeederGrid.SetItemText(0,0,_T("Name"));
	m_FeederGrid.SetItemText(0,1,_T("X"));
	m_FeederGrid.SetItemText(0,2,_T("Y"));
	m_FeederGrid.SetItemText(0,3,_T("Rot"));
	m_FeederGrid.SetItemText(0,4,_T("LX"));
	m_FeederGrid.SetItemText(0,5,_T("LY"));
	m_FeederGrid.SetItemText(0,6,_T("CX"));
	m_FeederGrid.SetItemText(0,7,_T("CY"));
	m_FeederGrid.SetItemText(0,8,_T("T"));
	m_FeederGrid.SetItemText(0,9,_T("CNT"));

	m_FeederGrid.SetColumnWidth(0,(int)(nInterval));
	m_FeederGrid.SetColumnWidth(1,(int)(nInterval));
	m_FeederGrid.SetColumnWidth(2,(int)(nInterval));
	m_FeederGrid.SetColumnWidth(3,(int)(nInterval));

	m_FeederGrid.SetColumnWidth(4,nInterval);
	m_FeederGrid.SetColumnWidth(5,nInterval);

	m_FeederGrid.SetColumnWidth(6,nInterval);
	m_FeederGrid.SetColumnWidth(7,nInterval);
	m_FeederGrid.SetColumnWidth(8,(int)(nInterval));
	m_FeederGrid.SetColumnWidth(9,(int)(nInterval));

	m_FeederGrid.SetGridLines( GVL_BOTH );

	m_FeederGrid.SetFixedColumnSelection(TRUE);
	m_FeederGrid.SetFixedRowSelection(TRUE);

	m_FeederGrid.SetFixedRowCount( 1 );

	m_FeederGrid.SetTrackFocusCell( true );
	m_FeederGrid.SetFrameFocusCell( true );

	m_FeederGrid.SetFixedRowSelection( true );

	m_FeederGrid.SetGridLines( GVL_BOTH );

	for (int row = m_FeederGrid.GetFixedRowCount(); row < m_FeederGrid.GetRowCount(); row++)
	{
		m_FeederGrid.SetCellType(row,2, RUNTIME_CLASS(CGridCellNumeric));

		CGridCellNumeric *pCell = (CGridCellNumeric *)m_FeederGrid.GetCell(row,2);

		if( pCell ){

			pCell->SetFlags(NT_INTEGER);
			pCell->SetNumber( 0 );
		}

		m_FeederGrid.SetRowHeight(row,20);
	}

	for (int row = m_FeederGrid.GetFixedRowCount(); row < m_FeederGrid.GetRowCount(); row++)
	{
		m_FeederGrid.SetCellType(row,1, RUNTIME_CLASS(CGridCellNumeric));

		CGridCellNumeric *pCell = (CGridCellNumeric *)m_FeederGrid.GetCell(row,1);

		if( pCell ){

			pCell->SetFlags(NT_INTEGER);
			pCell->SetNumber( 0 );
		}

		m_FeederGrid.SetRowHeight(row,20);
	}
}

void CFeederSetup::AddRow( CListCtrl_FeederList::FeederDatabase *entry ) 
{
	int i ;

	ASSERT( entry );

	i =  m_FeederGrid.GetRowCount() + 1;
	m_FeederGrid.SetRowCount(  i );

	i--;

	m_FeederGrid.SetItemText(i ,0 ,CString(entry->label));
	SetValue( i,1, entry->x );
	SetValue( i,2, entry->y );
	SetValue( i,3, entry->rot );
	SetValue( i,4, entry->lx );
	SetValue( i,5, entry->lx );
	SetValue( i,6, entry->countx );
	SetValue( i,7, entry->county );
	SetValue( i,8, entry->tool );
	SetValue( i,9, entry->componentIndex );

	m_FeederGrid.SetRowHeight(i ,20);
}

void CFeederSetup::SetValue(int row, int col, double value ) 
{
	m_FeederGrid.SetCellType( row, col, RUNTIME_CLASS(CGridCellNumeric));
	CGridCellNumeric *pCell = (CGridCellNumeric *)m_FeederGrid.GetCell( row, col );

	if( pCell ){
		pCell->SetFlags( CGridCellNumeric::Integer );
		pCell->SetNumber( (int)value );
	}
}
void CFeederSetup::RebuildList ( void ) 
{		
	CString temp;
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );
	CListCtrl_FeederList &m_FeederList(pDlg->m_FeederList);

	// remove any and all items from List
	//pDlg->m_FeederList.DeleteAllItems();
	ResetFeederGrid();

	// for all items loaded
	for ( unsigned int i = 0 ; i < pDlg->m_FeederList.m_Count ; i ++ ) {

		// fetch entry 
		pDlg->m_FeederList.entry = &pDlg->m_FeederList.m_Database.at( i  ) ;
		AddRow( pDlg->m_FeederList.entry ) ;

	}
}
 void CFeederSetup::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
 {
	CPickobearDlg *pDlg = (CPickobearDlg*)AfxGetApp()->m_pMainWnd;
	ASSERT( pDlg );	
	CListCtrl_FeederList &m_FeederList(pDlg->m_FeederList);

	 NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	 // AcceptChange is a fictional routine that should return TRUE
	 // if you want to accept the new value for the cell.
	 BOOL bAcceptChange = TRUE; //AcceptChange(pItem->iRow, pItem->iColumn);

	 CStringA str;
	 CListCtrl_FeederList::FeederDatabase feeder = m_FeederList.at ( pItem->iRow-1 );

	 CGridCellNumeric *pCell = (CGridCellNumeric *)m_FeederGrid.GetCell(pItem->iRow, pItem->iColumn);

	 _RPT1(_CRT_WARN, "Editing cell %s\r\n",feeder.label);

	 switch (  pItem->iColumn ) {
		 case 0:

			 str = UTF16toUTF8( m_FeederGrid.GetCell(pItem->iRow, pItem->iColumn)->GetText() );

			 strcpy_s( m_FeederList.at ( pItem->iRow-1 ).label, 256, str );
			 break;
		 case 1: 
			 if ( !( pCell->GetNumber() < 0  ||pCell->GetNumber() > MAX_X_TABLE ) && m_FeederList.at ( pItem->iRow-1 ).x != (long)pCell->GetNumber() )
				 m_FeederList.at ( pItem->iRow-1 ).x = (long)pCell->GetNumber();
			 else 
				 bAcceptChange =  FALSE;
			 break;

		 case 2: 
			 if ( !( pCell->GetNumber() < 0  || pCell->GetNumber() > MAX_Y_TABLE )  &&  m_FeederList.at ( pItem->iRow-1 ).y != (long)pCell->GetNumber() )
				 m_FeederList.at ( pItem->iRow-1 ).y = (long)pCell->GetNumber();
			 else 
				 bAcceptChange =  FALSE;
			 break;
		 case 3: 
			 if ( !( pCell->GetNumber() < 0  || pCell->GetNumber() > 360 ) && m_FeederList.at ( pItem->iRow-1 ).rot != (short) pCell->GetNumber())
				 m_FeederList.at ( pItem->iRow-1 ).rot =(short) pCell->GetNumber();
			 else 
				 bAcceptChange =  FALSE;
			 break;

		 case 4:
			if( m_FeederList.at ( pItem->iRow-1 ).lx != (long)pCell->GetNumber() ) 
				m_FeederList.at ( pItem->iRow-1 ).lx = (long)pCell->GetNumber();
			 else 
				 bAcceptChange =  FALSE;
			 break;

		 case 5: 
			if( m_FeederList.at ( pItem->iRow-1 ).ly != (long)pCell->GetNumber() ) 
			 m_FeederList.at ( pItem->iRow-1 ).ly = (long)pCell->GetNumber();
			 else 
				 bAcceptChange =  FALSE;
			 break;

		 case 6: 
			 if( m_FeederList.at ( pItem->iRow-1 ).countx != (unsigned long)pCell->GetNumber() )
				m_FeederList.at ( pItem->iRow-1 ).countx = (unsigned long)pCell->GetNumber();
			 else 
				 bAcceptChange =  FALSE;
			 break;

		 case 7: 
			 if( m_FeederList.at ( pItem->iRow-1 ).county != (unsigned long)pCell->GetNumber() ) 
				 m_FeederList.at ( pItem->iRow-1 ).county = (unsigned long)pCell->GetNumber();
			 break;

		 case 8: 
			 if( m_FeederList.at( pItem->iRow-1 ).tool != (unsigned char )pCell->GetNumber() )
				m_FeederList.at ( pItem->iRow-1 ).tool = (unsigned char )pCell->GetNumber();
			 break;

		 case 9: 
			 if( m_FeederList.at ( pItem->iRow-1 ).componentIndex != (unsigned char )pCell->GetNumber() )
				 m_FeederList.at ( pItem->iRow-1 ).componentIndex = (unsigned char )pCell->GetNumber();
			 break;
	 }

	 if( bAcceptChange ) {
	//	 m_FeederGrid.AutoSize();
		 m_FeederGrid.Refresh();

		 m_FeederList.RebuildList();

	 }

	 *pResult = (bAcceptChange)? 0 : -1;
 }
