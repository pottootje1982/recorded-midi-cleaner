#pragma once

class RprMidiTake;

enum RemoveDoubleTriggerMode {
	JOIN_LEADING,
	JOIN_TRAILING
};

RprMidiTake* GetActiveMidiMidiTake();
int CountSelectedNotes();
void SelectShortNotes(int maxNoteLength);
void UnselectAllNotes();
void RemoveDoubleTriggers(int noteLength, RemoveDoubleTriggerMode removeDoubleTriggerMode, bool previewJoinedNotes);
