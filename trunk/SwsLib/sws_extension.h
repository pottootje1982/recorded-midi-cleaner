#pragma once

static WDL_IntKeyedArray<COMMAND_T*> g_commands;

bool hookCommandProc(int iCmd, int flag);
int toggleActionHook(int iCmd);
void swsMenuHook(const char* menustr, HMENU hMenu, int flag);
