/******************************************************************************
/ sws_extension.cpp
/
/ Copyright (c) 2013 Tim Payne (SWS), Jeffos
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
#include "Console/Console.h"
#include "Freeze/Freeze.h"
#include "MarkerActions/MarkerActions.h"
#include "Snapshots/SnapshotClass.h"
#include "Snapshots/Snapshots.h"
#include "Zoom.h"
#include "Misc/Misc.h"
#include "Misc/RecCheck.h"
#include "Misc/Adam.h"
#include "Color/Color.h"
#include "Color/Autocolor.h"
#include "MarkerList/MarkerListClass.h"
#include "MarkerList/MarkerList.h"
#include "TrackList/TracklistFilter.h"
#include "TrackList/Tracklist.h"
#include "Projects/ProjectMgr.h"
#include "Projects/ProjectList.h"
#include "SnM/SnM.h"
#include "SnM/SnM_CSurf.h"
#include "SnM/SnM_Dlg.h"
#include "Padre/padreActions.h"
#include "Fingers/FNG_client.h"
#include "Autorender/Autorender.h"
#include "IX/IX.h"
#include "Breeder/BR.h" 

#define LOCALIZE_IMPORT_PREFIX "sws_"
#ifdef LOCALIZE_IMPORT_PREFIX
#include "./reaper/localize-import.h"
#endif
#include "./reaper/localize.h"


// Globals
REAPER_PLUGIN_HINSTANCE g_hInst = NULL;
HWND g_hwndParent = NULL;

#ifdef ACTION_DEBUG
void freeCmdFilesValue(WDL_String* p) {delete p;}
static WDL_IntKeyedArray<WDL_String*> g_cmdFiles(freeCmdFilesValue);
#endif
static WDL_IntKeyedArray<COMMAND_T*> g_commands; // no valdispose (cmds can be allocated in different ways)

int g_iFirstCommand = 0;
int g_iLastCommand = 0;


bool hookCommandProc(int iCmd, int flag)
{
	static WDL_PtrList<const char> sReentrantCmds;

	// for Xen extensions
	g_KeyUpUndoHandler=0;

	// "Hack" to make actions will #s less than 1000 work with SendMessage (AHK)
	// no recursion check here: handled by REAPER
	if (iCmd < 1000)
		return KBD_OnMainActionEx(iCmd, 0, 0, 0, g_hwndParent, NULL) ? true : false; // C4800

	// Special case for checking recording
	if (iCmd == 1013 && !RecordInputCheck())
		return true;

	// Ignore commands that don't have anything to do with us from this point forward
	if (COMMAND_T* cmd = SWSGetCommandByID(iCmd))
	{
		if (cmd->accel.accel.cmd==iCmd && cmd->doCommand && cmd->doCommand!=SWS_NOOP)
		{
			if (sReentrantCmds.Find(cmd->id) == -1)
			{
				sReentrantCmds.Add(cmd->id);
				cmd->fakeToggle = !cmd->fakeToggle;
#ifndef DEBUG_PERFORMANCE_TIME
				cmd->doCommand(cmd);
#else
				CommandTimer(cmd);
#endif
				sReentrantCmds.Delete(sReentrantCmds.Find(cmd->id));
				return true;
			}
#ifdef ACTION_DEBUG
			else
			{
				OutputDebugString("hookCommandProc - recursive action: ");
				OutputDebugString(cmd->id);
				OutputDebugString("\n");
			}
#endif
		}
	}
	return false;
}

// Returns:
// -1 = action does not belong to this extension, or does not toggle
//  0 = action belongs to this extension and is currently set to "off"
//  1 = action belongs to this extension and is currently set to "on"
int toggleActionHook(int iCmd)
{
	static WDL_PtrList<const char> sReentrantCmds;
	if (COMMAND_T* cmd = SWSGetCommandByID(iCmd))
	{
		if (cmd->accel.accel.cmd==iCmd && cmd->getEnabled && cmd->doCommand!=SWS_NOOP)
		{
			if (sReentrantCmds.Find(cmd->id) == -1)
			{
				sReentrantCmds.Add(cmd->id);
				int state = cmd->getEnabled(cmd);
				sReentrantCmds.Delete(sReentrantCmds.Find(cmd->id));
				return state;
			}
#ifdef ACTION_DEBUG
			else
			{
				OutputDebugString("toggleActionHook - recursive action: ");
				OutputDebugString(cmd->id);
				OutputDebugString("\n");
			}
#endif
		}
	}
	return -1;
}

// 1) Get command ID from Reaper
// 2) Add keyboard accelerator (with localized action name) and add to the "action" list
int SWSRegisterCmd(COMMAND_T* pCommand, const char* cFile, int cmdId, bool localize)
{
	if (pCommand->doCommand)
	{
		// SWS - Unfortunately can't check for duplicate actions here because when commands are used in the mouse editor
		//   they have a command ID (53000+) assigned before SWS is even loaded.
		// char pId[128];
		// if (_snprintf(pId, 128, "_%s", pCommand->id)<=0 || NamedCommandLookup(pId))
		//	return 0; // duplicated action

		if (!cmdId && !(cmdId = plugin_register("command_id", (void*)pCommand->id)))
			return 0;

		pCommand->accel.accel.cmd = cmdId;

		// localized action name, if needed
		const char* defaultName = pCommand->accel.desc;
		if (localize)
			pCommand->accel.desc = GetLocalizedActionName(pCommand->accel.desc); // no alloc + no-op when no LangPack file is defined
		
		if (!plugin_register("gaccel", &pCommand->accel))
			return 0;

		// now that it is registered, restore the default action name
		if (pCommand->accel.desc != defaultName)
			pCommand->accel.desc = defaultName;

		if (!g_iFirstCommand || g_iFirstCommand > cmdId)
			g_iFirstCommand = cmdId;
		if (cmdId > g_iLastCommand)
			g_iLastCommand = cmdId;

		g_commands.Insert(cmdId, pCommand);
#ifdef ACTION_DEBUG
		g_cmdFiles.Insert(cmdId, new WDL_String(cFile));
#endif
	}
	return pCommand->accel.accel.cmd;
}

// For each item in table call SWSRegisterCommand
int SWSRegisterCmds(COMMAND_T* pCommands, const char* cFile, bool localize)
{
	int i = 0;
	while(pCommands[i].id != LAST_COMMAND)
		SWSRegisterCmd(&pCommands[i++], cFile, 0, localize);
	return 1;
}

// Make and register a dynamic action (created at runtime)
// If cmdId==0, get command ID from Reaper (use the provided cmdId otherwise)
// Note: SWSFreeUnregisterDynamicCmd() can be used to free/unregister such an action
int SWSCreateRegisterDynamicCmd(int cmdId, void (*doCommand)(COMMAND_T*), int (*getEnabled)(COMMAND_T*), const char* cID, const char* cDesc, const char* cMenu, INT_PTR user, const char* cFile, bool localize)
{
	COMMAND_T* ct = new COMMAND_T;
	memset(ct, 0, sizeof(COMMAND_T));
	ct->accel.desc = _strdup(cDesc);
	ct->id = _strdup(cID);
	ct->doCommand = doCommand;
	ct->getEnabled = getEnabled;
	ct->user = user;
	ct->menuText = cMenu;
	return SWSRegisterCmd(ct, cFile, cmdId, localize);
}

void SWSFreeUnregisterDynamicCmd(int id)
{
	if (COMMAND_T* ct = SWSUnregisterCmd(id))
	{
		free((void*)ct->accel.desc);
		free((void*)ct->id);
		DELETE_NULL(ct);
	}
}

// Returns the COMMAND_T entry (so it can be freed if necessary)
COMMAND_T* SWSUnregisterCmd(int id)
{
	if (COMMAND_T* ct = g_commands.Get(id, NULL))
	{
		plugin_register("-gaccel", &ct->accel);
		plugin_register("-command_id", &id);
		g_commands.Delete(id);
#ifdef ACTION_DEBUG
		g_cmdFiles.Delete(id);
#endif
		return ct;
	}
	return NULL;
}

#ifdef ACTION_DEBUG
void ActionsList(COMMAND_T*)
{
	// Output sws_actions.csv
	char cBuf[512];
	strncpy(cBuf, get_ini_file(), 256);
	char* pC = strrchr(cBuf, PATH_SLASH_CHAR);
	if (pC)
	{
		strcpy(pC+1, "sws_actions.csv");
		FILE* f = fopenUTF8(cBuf, "w");
		fputs("Action,File,CmdID,CmdStr\n", f);
		if (f)
		{
			for (int i = 0; i < g_commands.GetSize(); i++)
			{
				if (COMMAND_T* cmd = g_commands.Enumerate(i, NULL, NULL))
				{
					WDL_String* pFn = g_cmdFiles.Get(cmd->accel.accel.cmd, NULL);
					sprintf(cBuf, "\"%s\",%s,%d,_%s\n", cmd->accel.desc, pFn ? pFn->Get() : "", cmd->accel.accel.cmd, cmd->id);
					fputs(cBuf, f);
				}
			}
			fclose(f);
		}
	}
}
#endif

int SWSGetCommandID(void (*cmdFunc)(COMMAND_T*), INT_PTR user, const char** pMenuText)
{
	for (int i=0; i<g_commands.GetSize(); i++)
	{
		if (COMMAND_T* cmd = g_commands.Enumerate(i, NULL, NULL))
		{
			if (cmd->doCommand == cmdFunc && cmd->user == user)
			{
				if (pMenuText)
					*pMenuText = cmd->menuText;
				return cmd->accel.accel.cmd;
			}
		}
	}
	return 0;
}

COMMAND_T* SWSGetCommandByID(int cmdId) {
	if (cmdId >= g_iFirstCommand && cmdId <= g_iLastCommand) // not enough to ensure it is a SWS action
		return g_commands.Get(cmdId, NULL);
	return NULL;
}

int IsSwsAction(const char* _actionName)
{
	if (_actionName)
		if (const char* p = strstr(_actionName, ": ")) // no strchr() here: make sure p[2] is not out of bounds
			if (const char* tag = strstr(_actionName, "SWS")) // make sure it is a SWS tag
				if (tag < p) // make really sure
					return ((int)(p+2-_actionName));
	return 0;
}


