// RemoveDoubleTriggerDialog.cpp : implementation file
//

#include "preheader.h"
#include "RemoveDoubleTriggerDialog.h"
#include "MidiCommands.h"

// CRemoveDoubleTriggerDialog dialog

IMPLEMENT_DYNAMIC(CRemoveDoubleTriggerDialog, CDialog)

CRemoveDoubleTriggerDialog::CRemoveDoubleTriggerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoveDoubleTriggerDialog::IDD, pParent)
	, dialog(0)
	, minNoteLengthStr(_T(""))
	, selectedNotesStr(_T(""))
{
}

CRemoveDoubleTriggerDialog::~CRemoveDoubleTriggerDialog()
{
}

BOOL CRemoveDoubleTriggerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	minNoteLengthStr = "250";

	joinLeadingNotesButton.SetCheck(BST_CHECKED);
	joinTrailingNotesButton.SetCheck(BST_CHECKED);

	minNoteLengthSlider.SetRange(0, 1000);
	minNoteLengthSlider.SetTicFreq(10);
	
	UpdateSelectedNotes();

	UpdateData(FALSE);

	OnEnChangeMinNoteLengthEdit();

	return TRUE;
}

void CRemoveDoubleTriggerDialog::UpdateSelectedNotes()
{
	char selectedNotesArray[128];
	itoa(CountSelectedNotes(), selectedNotesArray, 10);
	selectedNotesStr = selectedNotesArray;
	UpdateData(FALSE);
}

void CRemoveDoubleTriggerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_JOIN_LEADING_NOTES_CHECK, joinLeadingNotesButton);
	DDX_Control(pDX, IDC_JOIN_TRAILING_NOTES_CHECK, joinTrailingNotesButton);
	DDX_Control(pDX, IDC_MIN_NOTE_LENGTH_SLIDER, minNoteLengthSlider);
	DDX_Text(pDX, IDC_MIN_NOTE_LENGTH_EDIT, minNoteLengthStr);
	DDX_Text(pDX, IDC_NR_NOTES_SELECTED, selectedNotesStr);
}


BEGIN_MESSAGE_MAP(CRemoveDoubleTriggerDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CRemoveDoubleTriggerDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PREVIEW_BUTTON, &CRemoveDoubleTriggerDialog::OnBnClickedPreviewButton)
	ON_EN_CHANGE(IDC_MIN_NOTE_LENGTH_EDIT, &CRemoveDoubleTriggerDialog::OnEnChangeMinNoteLengthEdit)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_MIN_NOTE_LENGTH_SLIDER, &CRemoveDoubleTriggerDialog::OnNMReleasedcaptureMinNoteLengthSlider)
	ON_BN_CLICKED(IDC_SELECT_SHORT_NOTES_BUTTON, &CRemoveDoubleTriggerDialog::OnBnClickedSelectShortNotesButton)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_MIN_NOTE_LENGTH_SLIDER, &CRemoveDoubleTriggerDialog::OnTRBNThumbPosChangingMinNoteLengthSlider)
END_MESSAGE_MAP()


// CRemoveDoubleTriggerDialog message handlers

void CRemoveDoubleTriggerDialog::OnBnClickedOk()
{
	JoinNotes(false);
	OnOK();
}

void CRemoveDoubleTriggerDialog::OnBnClickedPreviewButton()
{
	UnselectAllNotes();
	JoinNotes(true);
	char noteCountArray[128];
	itoa(CountSelectedNotes(), noteCountArray, 10);
	selectedNotesStr = noteCountArray;
	UpdateData();
}

void CRemoveDoubleTriggerDialog::JoinNotes(bool preview)
{
	bool joinLeadingNotes = joinLeadingNotesButton.GetCheck() == BST_CHECKED;
	bool joinTrailingNotes = joinTrailingNotesButton.GetCheck() == BST_CHECKED;
	char noteLengthStr[256];
	GetDlgItemText(m_hWnd, IDC_MIN_NOTE_LENGTH_EDIT, noteLengthStr, 256);
	int noteLength = atoi(noteLengthStr);
	if (joinLeadingNotes)
		RemoveDoubleTriggers(noteLength, JOIN_LEADING, preview);
	if (joinTrailingNotes)
		RemoveDoubleTriggers(noteLength, JOIN_TRAILING, preview);
	UpdateSelectedNotes();
}

void CRemoveDoubleTriggerDialog::OnEnChangeMinNoteLengthEdit()
{
	char minNoteLengthStr[256];
	GetDlgItemText(m_hWnd, IDC_MIN_NOTE_LENGTH_EDIT, minNoteLengthStr, sizeof(minNoteLengthStr));
	minNoteLengthSlider.SetPos(atoi(minNoteLengthStr));
	UpdateData();
}

void CRemoveDoubleTriggerDialog::OnNMReleasedcaptureMinNoteLengthSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	char minNoteLengthStr[128];
	itoa(minNoteLengthSlider.GetPos(), minNoteLengthStr, 10);
	SetDlgItemText(m_hWnd, IDC_MIN_NOTE_LENGTH_EDIT, minNoteLengthStr);
	*pResult = 0;
	UpdateData();
}


void CRemoveDoubleTriggerDialog::OnBnClickedSelectShortNotesButton()
{
	int noteLength = atoi(minNoteLengthStr);
	SelectShortNotes(noteLength);
	UpdateSelectedNotes();
}

void CRemoveDoubleTriggerDialog::OnTRBNThumbPosChangingMinNoteLengthSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
