/******************************************************************************
/ SnM_Dlg.cpp
/
/ Copyright (c) 2009-2013 Jeffos
/ http://www.standingwaterstudios.com/reaper
/
/ Permission is hereby granted, free of charge, to any person obtaining a copy
/ of this software and associated documentation files (the "Software"), to deal
/ in the Software without restriction, including without limitation the rights to
/ use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
/ of the Software, and to permit persons to whom the Software is furnished to
/ do so, subject to the following conditions:
/ 
/ The above copyright notice and this permission notice shall be included in all
/ copies or substantial portions of the Software.
/ 
/ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/ EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
/ OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
/ NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
/ HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/ WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/ FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/ OTHER DEALINGS IN THE SOFTWARE.
/
******************************************************************************/

#include "stdafx.h"

#ifdef _WIN32

static BOOL CALLBACK EnumRemoveXPStyles(HWND _hwnd, LPARAM _unused)
{
	// do not deal with list views & list boxes
	char className[64] = "";
	if (GetClassName(_hwnd, className, sizeof(className)) && 
		strcmp(className, WC_LISTVIEW) && 
		strcmp(className, WC_LISTBOX))
	{
		LONG style = GetWindowLong(_hwnd, GWL_STYLE);
		if ((style & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX ||
			(style & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON ||
			(style & BS_GROUPBOX) == BS_GROUPBOX)
		{
			RemoveXPStyle(_hwnd, 1);
		}
	}
	return TRUE;
}

#endif

WDL_DLGRET SNM_HookThemeColorsMessage(HWND _hwnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam, bool _wantColorEdit)
{
	if (SWS_THEMING)
	{
		switch(_uMsg)
		{
#ifdef _WIN32
			case WM_INITDIALOG :
				// remove XP style on some child ctrls (cannot be themed otherwise)
				EnumChildWindows(_hwnd, EnumRemoveXPStyles, 0);
				return 0;
#endif
			case WM_CTLCOLOREDIT:
				if (!_wantColorEdit) return 0;
			case WM_CTLCOLORSCROLLBAR: // not managed yet, just in case..
			case WM_CTLCOLORLISTBOX:
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORDLG:
			case WM_CTLCOLORSTATIC:
/* commented (custom implementations)
			case WM_DRAWITEM:
*/
				return SendMessage(GetMainHwnd(), _uMsg, _wParam, _lParam);
		}
	}
	return 0;
}
