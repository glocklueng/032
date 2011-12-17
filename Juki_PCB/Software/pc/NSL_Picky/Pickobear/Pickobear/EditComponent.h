#pragma once


// EditComponent dialog

class EditComponent : public CDialogEx
{
	DECLARE_DYNAMIC(EditComponent)

public:
	EditComponent(CWnd* pParent = NULL);   // standard constructor
	virtual ~EditComponent();
		
	CListCtrl_Components::CompDatabase entry;

// Dialog Data
	enum { IDD = IDD_COMPONENT_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	CString m_Value;
	CString m_Type;
	CString m_Feeder;
};
