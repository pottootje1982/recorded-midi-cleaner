#pragma once

static WDL_IntKeyedArray<COMMAND_T*> g_commands;

void AddToMenu(HMENU hMenu, const char* text, int id, int iInsertAfter = -1, bool bPos = false, UINT uiSate = MFS_UNCHECKED);
void AddSubMenu(HMENU hMenu, HMENU subMenu, const char* text, int iInsertAfter = -1, UINT uiSate = MFS_UNCHECKED);
bool hookCommandProc(int iCmd, int flag);
int toggleActionHook(int iCmd);
void swsMenuHook(const char* menustr, HMENU hMenu, int flag);
