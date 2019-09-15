#include "midiMgr.h"

void midiMgr::update(float t)
{
	if (!_isPlaying)
	{
		return;
	}
	_midiList[_playIdx].update(t);

	if (_midiList[_playIdx].isFinish())
	{
		stop();
	}
}

void midiMgr::add(string path)
{
	ofxMidiFile newMidi;
	if (newMidi.load(path)) {
		_midiList.push_back(newMidi);
	}
	else {
		ofLog(OF_LOG_ERROR, "[midiMgr::add]Load failed");
	}
}
void midiMgr::play(int idx)
{
	if (idx < 0 || idx >= _midiList.size()) {
		ofLog(OF_LOG_ERROR, "[midiMgr::play]out of range");
		return;
	}

	if (_isPlaying)
	{
		stop();
	}
	_playIdx = idx;
	_midiList[_playIdx].reset();
	ofAddListener(_midiList[_playIdx].midiNoteEvent, this, &midiMgr::onMidiNoteEvent);
	_isPlaying = true;
}

void midiMgr::stop()
{
	if (!_isPlaying)
	{
		return;
	}
	_midiList[_playIdx].reset();
	ofRemoveListener(_midiList[_playIdx].midiNoteEvent, this, &midiMgr::onMidiNoteEvent);
	clearAllNote();
	_playIdx = -1;
	_isPlaying = false;
}

void midiMgr::clearAllNote()
{
	for (int i = 0; i < 255; i++)
	{
		if (_noteFlag[i])
		{
			midiNodeEvent e;
			e.type = e.eNodeOff;
			e.note = i;
			ofNotifyEvent(noteEvent, e, this);
			_noteFlag[i] = false;
		}
	}
}

void midiMgr::onMidiNoteEvent(midiNodeEvent & e)
{
	_noteFlag[e.note] = (e.type == e.eNodeOn);
	ofNotifyEvent(noteEvent, e, this);
}

//--------------------------------------------------------------
midiMgr* midiMgr::pInstance = 0;

midiMgr* midiMgr::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new midiMgr();
	}
	return pInstance;
}

//--------------------------------------------------------------
void midiMgr::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}