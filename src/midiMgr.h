#pragma once

#include "ofMain.h"
#include "ofxMidiFile.h"

class midiMgr {
public:

	void update(float t);

	void add(string path);
	void play(int idx);
	void stop();

	inline int getMidiNum() {
		return _midiList.size();
	}

	inline int isPlaying() {
		return _isPlaying;
	}

private:
	void clearAllNote();

public:
	void onMidiNoteEvent(midiNodeEvent& e);

public:
	ofEvent<midiNodeEvent> noteEvent;

private:
	bool _isPlaying;
	int _playIdx;
	vector<ofxMidiFile> _midiList;
	array<bool, 255> _noteFlag;

//-------------------
//Singleton
//-------------------
private:
	midiMgr()
		:_isPlaying(false)
		, _playIdx(-1)
	{
		for (auto& iter : _noteFlag)
		{
			iter = false;
		}
	};
	~midiMgr()
	{
		midiMgr::Destroy();
	}
	midiMgr(midiMgr const&);
	void operator=(midiMgr const&);

public:
	static midiMgr* GetInstance();
	static void Destroy();

private:
	static midiMgr *pInstance;
};