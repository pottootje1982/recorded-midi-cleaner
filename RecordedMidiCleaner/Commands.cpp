#include "preheader.h"
#include "Fingers/CommandHandler.h"
#include "RemoveDoubleTriggerDialog.h"

void LaunchDoubleTriggerDialog(int flags, void *data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!MIDIEditor_GetActive())
		ShowMessageBox("Open a MIDI item first in the MIDI editor", "Open MIDI editor", 0);
	else
	{
		CRemoveDoubleTriggerDialog triggerDialog;
		triggerDialog.DoModal();
	}
}

void RegisterCommands() 
{
	RprCommand::registerCommand("Remove Double Triggers", "REMOVE_DOUBLE_TRIGGERS", &LaunchDoubleTriggerDialog, UNDO_STATE_ITEMS);
}	