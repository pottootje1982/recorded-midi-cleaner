#include "preheader.h"
#include "MidiCommands.h"
#include "Fingers/RprMidiTake.h"
#include "Fingers/CommandHandler.h"

RprMidiTake* GetActiveMidiMidiTake()
{
	void* me = MIDIEditor_GetActive();
	MediaItem_Take* takeInMe = me ? MIDIEditor_GetTake(me) : NULL;

	if (takeInMe)
	{
		RprMidiTake* midiTake = FNG_AllocMidiTake(takeInMe);
		return midiTake;
	}
	return NULL;
}

int CountSelectedNotes() 
{
	RprMidiTake* midiTake = GetActiveMidiMidiTake();

	int selectedCount = 0;

	if (midiTake)
	{
		int noteCount = FNG_CountMidiNotes(midiTake);
		for (int i = 0; i < noteCount; ++i) 
		{
			RprMidiNote* currNote=FNG_GetMidiNote(midiTake, i);
			bool selected =  FNG_GetMidiNoteIntProperty(currNote, "SELECTED");
			if (selected) ++selectedCount;
		}
	}
	return selectedCount;
}

void UnselectAllNotes()
{
	void* me = MIDIEditor_GetActive();
	MediaItem_Take* takeInMe = me ? MIDIEditor_GetTake(me) : NULL;
	RprMidiTake* midiTake = GetActiveMidiMidiTake();

	if (midiTake)
	{
		int noteCount = FNG_CountMidiNotes(midiTake);
		for (int i = 0; i < noteCount; ++i) 
		{
			RprMidiNote* currNote=FNG_GetMidiNote(midiTake, i);
			FNG_SetMidiNoteIntProperty(currNote, "SELECTED", FALSE);
		}
		FNG_FreeMidiTake(midiTake); // "Commit changes to MIDI take and free allocated memory"
	}
}

void RemoveDoubleTriggers(int noteLength, RemoveDoubleTriggerMode removeDoubleTriggerMode, bool previewJoinedNotes)
{
	if (!previewJoinedNotes)
		UnselectAllNotes();

	void* me = MIDIEditor_GetActive();
	MediaItem_Take* takeInMe = me ? MIDIEditor_GetTake(me) : NULL;
	RprMidiTake* midiTake = GetActiveMidiMidiTake();

	if (midiTake)
	{
		Undo_BeginBlock();

		int noteCount = FNG_CountMidiNotes(midiTake);
		std::map<int, int> pitches;

		for (int i = noteCount - 1; i >= 0; --i) 
		{
			RprMidiNote* currNote=FNG_GetMidiNote(midiTake, i);

			int pitch =  FNG_GetMidiNoteIntProperty(currNote, "PITCH");
			int position = FNG_GetMidiNoteIntProperty(currNote, "POSITION");
			if (pitches.find(pitch) != pitches.end())
			{
				int prevNoteIndex = pitches[pitch];
				RprMidiNote* prevNote = FNG_GetMidiNote(midiTake, prevNoteIndex);
				int prevPosition = FNG_GetMidiNoteIntProperty(prevNote, "POSITION");
				bool joinNotes = false;
				if (removeDoubleTriggerMode == JOIN_LEADING)
				{
					joinNotes = prevPosition - position <= noteLength;
				}
				else if (removeDoubleTriggerMode == JOIN_TRAILING)
				{
					int length = FNG_GetMidiNoteIntProperty(currNote, "LENGTH");
					int prevLength = FNG_GetMidiNoteIntProperty(prevNote, "LENGTH");
					joinNotes = (prevLength <= noteLength) && (position + length >= prevPosition);
				}
				if (joinNotes)
				{
					FNG_SetMidiNoteIntProperty(currNote, "SELECTED", 1);  // set note property to selected state
					FNG_SetMidiNoteIntProperty(prevNote, "SELECTED", 1);
					if (!previewJoinedNotes) 
					{
						FNG_FreeMidiTake(midiTake); // "Commit changes to MIDI take and free allocated memory"
						MIDIEditor_OnCommand(me, 40456); // join selected notes
						MIDIEditor_OnCommand(me, 40214); // unselect all notes
						midiTake = FNG_AllocMidiTake(takeInMe);
					}
				}
			}

			pitches[pitch] = i;

		}
		Undo_EndBlock("Remove Double Triggers", UNDO_STATE_ITEMS);

		FNG_FreeMidiTake(midiTake); // "Commit changes to MIDI take and free allocated memory"
	}
}

