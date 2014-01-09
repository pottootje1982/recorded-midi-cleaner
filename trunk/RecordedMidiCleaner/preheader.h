// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once
#include <afx.h>
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define STRICT
#ifdef _WIN32
#else
#include "../WDL/swell/swell.h"
#include <sys/time.h>
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <float.h>
#include <sys/stat.h>

// stl
#include <sstream>
#include <fstream>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <bitset>
#include <iomanip>
#include <string>
#include <list>
#include <map>
#include <set>
#include <numeric>
#include <ctime>

#pragma warning(disable : 4996) // POSIX deprecation warnings
#pragma warning(disable : 4267) // size_t to int warnings
#pragma warning(disable : 4244) // __int64 to int warnings
#include "../WDL/wdltypes.h"
#include "../WDL/ptrlist.h"
#include "../WDL/wdlstring.h"
#include "../WDL/heapbuf.h"
#include "../WDL/db2val.h"
#include "../WDL/wingui/wndsize.h"
#include "../WDL/lice/lice.h"
#include "../WDL/dirscan.h"
#include "../WDL/wingui/virtwnd.h"
#include "../WDL/wingui/virtwnd-controls.h"
#include "../WDL/assocarray.h"
#include "../WDL/win32_utf8.h"
#include "../WDL/lineparse.h"
#pragma warning(default : 4996)
#pragma warning(default : 4267)
#pragma warning(default : 4244)

// Reaper
#include "reaper/reaper_plugin.h"
#include "reaper/sws_rpf_wrapper.h"	// Must be before lice.h
#include "reaper/icontheme.h"		// Must be after sws_rpf_wrapper (reaper_plugin_functions.h) because it includes lice.h

// Headers that are used "enough" to be worth of being precompiled,
// at the expense of needing recompile of the headers on change
#include "Utility/SectionLock.h"
#include "sws_util.h"
#include "sws_wnd.h"
#include "resource.h"
#include "Xenakios/XenakiosExts.h"
#include "ObjectState/ObjectState.h"
#include "ObjectState/TrackFX.h"
#include "ObjectState/TrackSends.h"
#include "SnM/SnM_ChunkParserPatcher.h"
#include "Padre/padreUtils.h"
#include "Padre/padreMidi.h"
#include "Padre/padreMidiItemProcBase.h"
