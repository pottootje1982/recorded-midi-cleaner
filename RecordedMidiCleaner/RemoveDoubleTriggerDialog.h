#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

// CRemoveDoubleTriggerDialog dialog

class CRemoveDoubleTriggerDialog : public CDialog
{
	DECLARE_DYNAMIC(CRemoveDoubleTriggerDialog)

public:
	CRemoveDoubleTriggerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemoveDoubleTriggerDialog();

	// Dialog Data
	enum { IDD = IDD_REMOVE_DOUBLE_TRIGGER_DIALOG };

private:
	void JoinNotes(bool preview);
	void UpdateSelectedNotes();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CButton joinLeadingNotesButton;
	CButton joinTrailingNotesButton;
	CSliderCtrl minNoteLengthSlider;
	int dialog;
	CString minNoteLengthStr;
	CString selectedNotesStr;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPreviewButton();
	afx_msg void OnEnChangeMinNoteLengthEdit();
	afx_msg void OnNMReleasedcaptureMinNoteLengthSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSelectShortNotesButton();
	afx_msg void OnTRBNThumbPosChangingMinNoteLengthSlider(NMHDR *pNMHDR, LRESULT *pResult);
};
