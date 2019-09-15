#pragma once

#include "ofMain.h"
#include "MidiFile.h"

using namespace smf;



class midiNodeEvent : public ofEventArgs {
public:
	enum nodeType {
		eNodeOn = 0,
		eNodeOff,
	}type;
	int trackNo, note;
};

class ofxMidiFile {
public:
	struct midiNote {
		int startTick, endTick;
		int trackNo;
		int note;
	};

public:
	ofxMidiFile();

	bool load(string path);
	void update(double t);
	void reset();
	void printMidiList();
	void printSourceMidiTick();

	bool isFinish();

private:
	void initOrderList();
	void checkNoteEvent();
private:
	bool _isLoad, _isFinish;
	float _maxDuration;
	float _nowTime;
	int _nowTick, _lastTick;
	int _listIdx;
	MidiFile _midiFile;
	vector<midiNote> _midiOrderList;
	list<int> _midiCheckOffList;

public:
	ofEvent<midiNodeEvent> midiNoteEvent;
	
};