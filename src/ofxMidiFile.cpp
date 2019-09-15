#include "ofxMidiFile.h"

ofxMidiFile::ofxMidiFile()
	:_isLoad(false)
	, _isFinish(false)
	, _listIdx(0)
{

}

bool ofxMidiFile::load(string path)
{
	_midiFile.read("data/" + path);
	if (!_midiFile.status())
	{
		ofLog(OF_LOG_ERROR, "[load]Load Midi Status Error");
		return false;
	}
	
	_maxDuration = _midiFile.getFileDurationInSeconds();
	_midiFile.linkNotePairs();
	initOrderList();
	_isLoad = true;
	return true;
}

void ofxMidiFile::update(double t)
{
	if (!_isLoad)
	{
		return;
	}

	if (t > _nowTime)
	{	
		if (t >= _maxDuration)
		{
			_nowTick = _midiFile.getTrackCount();
			_isFinish = true;
		}
		else {
			_nowTime = t;
			_nowTick = static_cast<int>(_midiFile.getAbsoluteTickTime(t) + 0.5f);
		}
		checkNoteEvent();
	}
}

void ofxMidiFile::reset()
{
	if (!_isLoad)
	{
		return;
	}

	_nowTime = 0.0f;
	_nowTick = 0;
	_listIdx = 0;
	_isFinish = false;
}


void ofxMidiFile::printMidiList()
{
	for (auto& iter : _midiOrderList)
	{
		cout << "Note:" << iter.note << "\tStart:" << iter.startTick << "\tEnd:" << iter.endTick << endl;
	}
}

void ofxMidiFile::printSourceMidiTick()
{

	cout << "Tick\tDur\tMessage" << endl;
	for (int track = 0; track < _midiFile.getTrackCount(); track++)
	{
		cout << "Track:" << track << endl;
		for (int event = 0; event < _midiFile[track].size(); event++) {
			if (!_midiFile[track][event].isNoteOn())
			{
				continue;
			}
			cout << dec << _midiFile[track][event].tick;
			cout << '\t';
			if (_midiFile[track][event].isNoteOn())
				cout << _midiFile[track][event].getTickDuration();
			cout << '\t' << hex;
			for (int i = 0; i < _midiFile[track][event].size(); i++)
				cout << (int)_midiFile[track][event][i] << ' ';
			cout << endl;
		}
	}

}

bool ofxMidiFile::isFinish()
{
	return _isFinish;
}

void ofxMidiFile::initOrderList()
{
	_lastTick = 0;
	for (int t = 0; t < _midiFile.getTrackCount(); t++)
	{
		for (int i = 0; i < _midiFile[t].size(); i++)
		{
			if (!_midiFile[t][i].isNoteOn())
			{
				continue;
			}
			midiNote note;
			note.startTick = _midiFile[t][i].tick;
			note.endTick = _midiFile[t][i].tick + _midiFile[t][i].getTickDuration();
			note.note = (int)_midiFile[t][i][1];
			note.trackNo = t;

			if (note.endTick > _lastTick)
			{
				_lastTick = note.endTick;
			}
			_midiOrderList.push_back(note);
		}
	}

	sort(_midiOrderList.begin(),
		_midiOrderList.end(),
		[](const midiNote& a, const midiNote& b) -> bool
	{
		return a.startTick < b.startTick;
	}
	);

}

void ofxMidiFile::checkNoteEvent()
{
	//Check Note-Off
	auto iter = _midiCheckOffList.begin();
	while (iter != _midiCheckOffList.end())
	{
		if (_midiOrderList[*iter].endTick < _nowTick)
		{
			midiNodeEvent offEvent;
			offEvent.type = midiNodeEvent::eNodeOff;
			offEvent.note = _midiOrderList[*iter].note;
			offEvent.trackNo = _midiOrderList[*iter].trackNo;
			ofNotifyEvent(midiNoteEvent, offEvent, this);
			iter = _midiCheckOffList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	//Check Note-On
	for (int i = _listIdx; i < _midiOrderList.size(); i++)
	{
		if (_midiOrderList[i].startTick < _nowTick)
		{
			_midiCheckOffList.push_back(i);

			midiNodeEvent onEvent;
			onEvent.type = midiNodeEvent::eNodeOn;
			onEvent.note = _midiOrderList[i].note;
			onEvent.trackNo = _midiOrderList[i].trackNo;
			ofNotifyEvent(midiNoteEvent, onEvent, this);
		}
		else
		{
			_listIdx = i;
			break;
		}
	}
}