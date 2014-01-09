/******************************************************************************
/ SnM_Util.cpp
/
/ Copyright (c) 2012-2013 Jeffos
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

///////////////////////////////////////////////////////////////////////////////
// File util
///////////////////////////////////////////////////////////////////////////////

// the API function file_exists() is a bit different, it returns false for folders
bool FileOrDirExists(const char* _fn)
{
	if (_fn && *_fn && *_fn!='.') // valid absolute path (1/2)?
	{
		if (const char* p = strrchr(_fn, PATH_SLASH_CHAR)) // valid absolute path (2/2)?
		{
			WDL_FastString fn;
			fn.Set(_fn, *(p+1)? 0 : (int)(p-_fn)); // // bug fix for directories, skip last PATH_SLASH_CHAR if needed
			struct stat s;
#ifdef _WIN32
			return (statUTF8(fn.Get(), &s) == 0);
#else
			return (stat(fn.Get(), &s) == 0);
#endif
		}
	}
	return false;
}


