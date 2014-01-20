// RecordedMidiCleaner.cpp : Defines the initialization routines for the DLL.
//

#include "preheader.h"
#include "Commands.h"
#include "Misc/RecCheck.h"
#include "sws_extension.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Globals
void(*g_KeyUpUndoHandler)()=0;

CWinApp winApp;

// This function creates the extension menu (flag==0) and handles checking menu items (flag==1).
// Reaper automatically checks menu items of customized menus using toggleActionHook above,
// but since we can't tell if a menu is customized we always check either way.
void swsMenuHook(const char* menustr, HMENU hMenu, int flag)
{
	if (flag == 1)
	{
		// Handle checked menu items
		// Go through every menu item - see if it exists in the table, then check if necessary
		MENUITEMINFO mi={sizeof(MENUITEMINFO),};
		mi.fMask = MIIM_ID | MIIM_SUBMENU;
		for (int i = 0; i < GetMenuItemCount(hMenu); i++)
		{
			GetMenuItemInfo(hMenu, i, true, &mi);
			if (mi.hSubMenu)
				swsMenuHook(menustr, mi.hSubMenu, flag);
			else if (mi.wID >= (UINT)g_iFirstCommand && mi.wID <= (UINT)g_iLastCommand) {
				if (COMMAND_T* t = g_commands.Get(mi.wID, NULL))
					CheckMenuItem(hMenu, i, MF_BYPOSITION | (t->getEnabled && t->getEnabled(t) ? MF_CHECKED : MF_UNCHECKED));
			}
		}
	}
	else if (!strcmp(menustr, "Main extensions"))
		AddToMenu(hMenu, "Remove Double Triggers", NamedCommandLookup("_REMOVE_DOUBLE_TRIGGERS"));
}

static AFX_EXTENSION_MODULE RecordedMidiCleanerDLL = { NULL, NULL };

extern "C"
{
	#define ERR_RETURN(a) { return 0; }
	#define OK_RETURN(a)  { return 1; }

	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t *rec)
	{
		if (!rec)
		{
			// Terminate the library before destructors are called
			AfxTermExtensionModule(RecordedMidiCleanerDLL);

			ERR_RETURN("Exiting Reaper.\n")
		}

		// Extension DLL one-time initialization
		AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);

		int errcnt=0;
		IMPAPI(AddExtensionsMainMenu);
		IMPAPI(AddMediaItemToTrack);
		IMPAPI(AddProjectMarker);
		IMPAPI(AddProjectMarker2);
		IMPAPI(AddTakeToMediaItem);
		IMPAPI(adjustZoom);
		IMPAPI(ApplyNudge);
		IMPAPI(AttachWindowTopmostButton);
		IMPAPI(AttachWindowResizeGrip);
		IMPAPI(Audio_RegHardwareHook);
		IMPAPI(CoolSB_GetScrollInfo);
		IMPAPI(CoolSB_SetScrollInfo);
		IMPAPI(CountActionShortcuts);
		IMPAPI(CountMediaItems);
		IMPAPI(CountSelectedMediaItems);
		IMPAPI(CountSelectedTracks);
		IMPAPI(CountTakes);
		IMPAPI(CountTCPFXParms);
		IMPAPI(CountTempoTimeSigMarkers);
		IMPAPI(CountTracks);
		IMPAPI(CountTrackMediaItems);
		IMPAPI(CountTrackEnvelopes);
		IMPAPI(CreateLocalOscHandler);
		IMPAPI(CreateNewMIDIItemInProj);
		IMPAPI(CSurf_FlushUndo);
		IMPAPI(CSurf_GoEnd);
		IMPAPI(CSurf_OnMuteChange);
		IMPAPI(CSurf_OnPanChange);
		IMPAPI(CSurf_OnSelectedChange);
		IMPAPI(CSurf_OnTrackSelection);
		IMPAPI(CSurf_OnVolumeChange);
		IMPAPI(CSurf_OnWidthChange);
		IMPAPI(CSurf_TrackFromID);
		IMPAPI(CSurf_TrackToID);
		IMPAPI(DeleteActionShortcut);
		IMPAPI(DeleteProjectMarker);
		IMPAPI(DeleteTrack);
		IMPAPI(DeleteTrackMediaItem);
		IMPAPI(DestroyLocalOscHandler);
		IMPAPI(DoActionShortcutDialog);
		IMPAPI(Dock_UpdateDockID);
		IMPAPI(DockIsChildOfDock);
		IMPAPI(DockWindowActivate);
		IMPAPI(DockWindowAdd);
		IMPAPI(DockWindowAddEx);
		IMPAPI(DockWindowRefresh);
		IMPAPI(DockWindowRemove);
		IMPAPI(EnsureNotCompletelyOffscreen);
		IMPAPI(EnumProjectMarkers);
		IMPAPI(EnumProjectMarkers2);
		IMPAPI(EnumProjectMarkers3);
		IMPAPI(EnumProjects);
		IMPAPI(file_exists);
		IMPAPI(format_timestr);
		IMPAPI(format_timestr_pos);
		IMPAPI(format_timestr_len);	
		IMPAPI(FreeHeapPtr);
		IMPAPI(GetActionShortcutDesc);
		IMPAPI(GetActiveTake);
		IMPAPI(GetAppVersion);
		IMPAPI(GetColorThemeStruct);
		IMPAPI(GetContextMenu);
		IMPAPI(GetCurrentProjectInLoadSave);
		IMPAPI(GetCursorContext);
		IMPAPI(GetCursorPosition);
		IMPAPI(GetCursorPositionEx);
		IMPAPI(GetEnvelopeName);
		IMPAPI(GetExePath);
		IMPAPI(GetFocusedFX);
		IMPAPI(GetHZoomLevel);
		IMPAPI(GetIconThemePointer);
		IMPAPI(GetIconThemeStruct);
		IMPAPI(GetInputChannelName);
		IMPAPI(GetItemEditingTime2);
		IMPAPI(GetLastTouchedFX);
		IMPAPI(GetLastTouchedTrack);
		IMPAPI(GetMainHwnd);
		IMPAPI(GetMasterMuteSoloFlags);
		IMPAPI(GetMasterTrack);
		IMPAPI(GetMediaItem);
		IMPAPI(GetMediaItem_Track);
		IMPAPI(GetMediaItemInfo_Value);
		IMPAPI(GetMediaItemNumTakes);
		IMPAPI(GetMediaItemTake);
		IMPAPI(GetMediaItemTakeByGUID);
		IMPAPI(GetMediaItemTake_Item);
		IMPAPI(GetMediaItemTake_Source);
		IMPAPI(GetMediaItemTake_Track);
		IMPAPI(GetMediaItemTakeInfo_Value);
		IMPAPI(GetMediaSourceFileName);
		IMPAPI(GetMediaSourceType);
		IMPAPI(GetMediaTrackInfo_Value);
		IMPAPI(get_midi_config_var);
		IMPAPI(GetMouseModifier);
		IMPAPI(GetNumTracks);
		IMPAPI(GetOutputChannelName);
		IMPAPI(GetPeakFileName);
		IMPAPI(GetPeaksBitmap);
		IMPAPI(GetPlayPosition);
		IMPAPI(GetPlayPosition2);
		IMPAPI(GetPlayPositionEx);
		IMPAPI(GetPlayPosition2Ex);
		IMPAPI(GetPlayState);
		IMPAPI(GetPlayStateEx);
		IMPAPI(GetProjectPath);
		IMPAPI(GetProjectTimeSignature2);
		IMPAPI(GetResourcePath);
		IMPAPI(GetSelectedMediaItem);
		IMPAPI(GetSelectedTrack);
		IMPAPI(GetSelectedTrackEnvelope);
		IMPAPI(GetSet_ArrangeView2);
		IMPAPI(GetSetEnvelopeState);
		IMPAPI(GetSetMediaItemInfo);
		IMPAPI(GetSetMediaItemTakeInfo);
		IMPAPI(GetSetMediaTrackInfo);
		IMPAPI(GetSetObjectState);
		IMPAPI(GetSetObjectState2);
		IMPAPI(GetSetRepeat);
		IMPAPI(GetTempoTimeSigMarker);
		IMPAPI(GetTakeEnvelopeByName);
		IMPAPI(GetSetTrackSendInfo);
		IMPAPI(GetSetTrackState);
		IMPAPI(GetSet_LoopTimeRange);
		IMPAPI(GetSet_LoopTimeRange2);
		IMPAPI(GetSubProjectFromSource);
		IMPAPI(GetTake);
		IMPAPI(GetTCPFXParm);
		IMPAPI(GetToggleCommandState);
		IMPAPI(GetToggleCommandState2);
		IMPAPI(GetTrack);
		IMPAPI(GetTrackGUID);
		IMPAPI(GetTrackEnvelope);
		IMPAPI(GetTrackEnvelopeByName);
		IMPAPI(GetTrackInfo);
		IMPAPI(GetTrackMediaItem);
		IMPAPI(GetTrackNumMediaItems);
		IMPAPI(GetTrackUIVolPan);
		IMPAPI(GetUserInputs);
		IMPAPI(get_config_var);
		IMPAPI(get_ini_file);
		IMPAPI(GR_SelectColor);
		IMPAPI(GSC_mainwnd);
		IMPAPI(guidToString);
		IMPAPI(Help_Set);
		IMPAPI(InsertMedia);
		IMPAPI(InsertTrackAtIndex);
		IMPAPI(IsMediaExtension);
		IMPAPI(kbd_enumerateActions);
		IMPAPI(kbd_formatKeyName); 
		IMPAPI(kbd_getCommandName);
		IMPAPI(kbd_getTextFromCmd);
		IMPAPI(KBD_OnMainActionEx);
		IMPAPI(kbd_reprocessMenu);
		IMPAPI(kbd_RunCommandThroughHooks);
		IMPAPI(kbd_translateAccelerator);
		IMPAPI(Main_OnCommand);
		IMPAPI(Main_OnCommandEx);
		IMPAPI(Main_openProject);
		IMPAPI(MainThread_LockTracks);
		IMPAPI(MainThread_UnlockTracks);
		*(void**)&MarkProjectDirty = rec->GetFunc("MarkProjectDirty"); //>= v4.55pre2
		IMPAPI(MIDIEditor_GetActive);
		IMPAPI(MIDIEditor_GetMode);
		IMPAPI(MIDIEditor_GetTake);
		IMPAPI(MIDIEditor_LastFocused_OnCommand);
		IMPAPI(MIDIEditor_OnCommand);
		IMPAPI(MIDI_eventlist_Create);
		IMPAPI(MIDI_eventlist_Destroy);
		IMPAPI(mkpanstr);
		IMPAPI(mkvolpanstr);
		IMPAPI(mkvolstr);
		IMPAPI(MoveEditCursor);
		IMPAPI(MoveMediaItemToTrack);
		IMPAPI(OnPlayButton);
		IMPAPI(OnStopButton);
		IMPAPI(NamedCommandLookup);
		IMPAPI(parse_timestr_len);
		IMPAPI(parse_timestr_pos);
		IMPAPI(PCM_Sink_Create);
		IMPAPI(PCM_Source_CreateFromFile);
		IMPAPI(PCM_Source_CreateFromFileEx);
		IMPAPI(PCM_Source_CreateFromSimple);
		IMPAPI(PCM_Source_CreateFromType);
		IMPAPI(PlayPreview);
		IMPAPI(PlayTrackPreview);
		IMPAPI(PlayTrackPreview2Ex);
		IMPAPI(plugin_getFilterList);
		IMPAPI(plugin_getImportableProjectFilterList);
		IMPAPI(plugin_register);
		IMPAPI(PreventUIRefresh);
		IMPAPI(projectconfig_var_addr);
		IMPAPI(projectconfig_var_getoffs);
		IMPAPI(RefreshToolbar);
#ifdef _WIN32
		IMPAPI(RemoveXPStyle);
#endif
		IMPAPI(Resampler_Create);
		IMPAPI(screenset_register);
		IMPAPI(screenset_registerNew);
		IMPAPI(screenset_unregister);
		IMPAPI(SectionFromUniqueID);
		IMPAPI(SelectProjectInstance);
		IMPAPI(SendLocalOscMessage);
		IMPAPI(SetCurrentBPM);
		IMPAPI(SetEditCurPos);
		IMPAPI(SetEditCurPos2);
		IMPAPI(SetMediaItemInfo_Value);
		IMPAPI(SetMediaItemLength);
		IMPAPI(SetMediaItemPosition);
		IMPAPI(SetMediaItemTakeInfo_Value);
		IMPAPI(SetMediaTrackInfo_Value);
		IMPAPI(SetMixerScroll);
		IMPAPI(SetMouseModifier);
		IMPAPI(SetOnlyTrackSelected);
		IMPAPI(SetProjectMarker);
		IMPAPI(SetProjectMarker2);
		IMPAPI(SetProjectMarker3);
		IMPAPI(SetTempoTimeSigMarker);
		IMPAPI(SetTrackSelected);
		IMPAPI(ShowActionList);
		IMPAPI(ShowConsoleMsg);
		IMPAPI(ShowMessageBox);
		IMPAPI(SnapToGrid);
		IMPAPI(SplitMediaItem);
		IMPAPI(StopPreview);
		IMPAPI(StopTrackPreview);
		IMPAPI(stringToGuid);
		IMPAPI(TimeMap_GetDividedBpmAtTime);
		IMPAPI(TimeMap_GetTimeSigAtTime);
		IMPAPI(TimeMap_QNToTime);
		IMPAPI(TimeMap_timeToQN);
		IMPAPI(TimeMap2_beatsToTime);
		IMPAPI(TimeMap2_GetDividedBpmAtTime);
		IMPAPI(TimeMap2_GetNextChangeTime);
		IMPAPI(TimeMap2_QNToTime);
		IMPAPI(TimeMap2_timeToBeats);
		IMPAPI(TimeMap2_timeToQN);
		IMPAPI(TrackFX_FormatParamValue);
		IMPAPI(TrackFX_GetByName);
		IMPAPI(TrackFX_GetChainVisible);
		IMPAPI(TrackFX_GetEnabled);
		IMPAPI(TrackFX_GetFloatingWindow);
		IMPAPI(TrackFX_GetCount);
		IMPAPI(TrackFX_GetFXName);
		IMPAPI(TrackFX_GetFXGUID);
		IMPAPI(TrackFX_GetNumParams);
		IMPAPI(TrackFX_GetOpen);
		IMPAPI(TrackFX_GetParam);
		IMPAPI(TrackFX_GetParamName);
		IMPAPI(TrackFX_GetPreset);
		IMPAPI(TrackFX_GetPresetIndex);
		IMPAPI(TrackFX_NavigatePresets);
		IMPAPI(TrackFX_SetEnabled);
		IMPAPI(TrackFX_SetOpen);
		IMPAPI(TrackFX_SetParam);
		IMPAPI(TrackFX_SetPreset);
		IMPAPI(TrackFX_SetPresetByIndex);
		IMPAPI(TrackFX_Show);
		IMPAPI(TrackList_AdjustWindows);
		IMPAPI(Undo_BeginBlock);
		IMPAPI(Undo_BeginBlock2);
		IMPAPI(Undo_CanRedo2);
		IMPAPI(Undo_CanUndo2);
		IMPAPI(Undo_DoUndo2);
		IMPAPI(Undo_EndBlock);
		IMPAPI(Undo_EndBlock2);
		IMPAPI(Undo_OnStateChange);
		IMPAPI(Undo_OnStateChange_Item);
		IMPAPI(Undo_OnStateChange2);
		IMPAPI(Undo_OnStateChangeEx);
		IMPAPI(Undo_OnStateChangeEx2);
		IMPAPI(UpdateArrange);
		IMPAPI(UpdateItemInProject);
		IMPAPI(UpdateTimeline);
		IMPAPI(ValidatePtr);

		g_hInst = hInstance;
		g_hwndParent = GetMainHwnd();

		CWnd* wnd = AfxGetMainWnd();

		if (!rec->Register("hookcommand",(void*)hookCommandProc))
			ERR_RETURN("hook command error\n")

		if (!rec->Register("toggleaction", (void*)toggleActionHook))
			ERR_RETURN("Toggle action hook error\n")

		RegisterCommands();

		if (!rec->Register("hookcustommenu", (void*)swsMenuHook))
			ERR_RETURN("Menu hook error\n");
		AddExtensionsMainMenu();
		OK_RETURN("OK");
	}
}

