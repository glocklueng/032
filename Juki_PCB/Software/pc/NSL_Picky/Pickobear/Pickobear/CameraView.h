#pragma once


// CCameraView dialog

class CCameraView : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraView)

public:
	CCameraView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCameraView();
		
	COpenGLControl m_CameraWindow;

// Dialog Data
	enum { IDD = IDD_POPUP_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
