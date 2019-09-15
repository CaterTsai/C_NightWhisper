#pragma once

#include "constParameter.h"
#include "../midiMgr.h"

class drawBase {
public:
	drawBase() :
		_isSetup(false)
		, _isStart(false)
	{}

	virtual void init() = 0;

	virtual void update(float delta) = 0;
	virtual void draw() = 0;
	virtual void reset() = 0;

	virtual void start() {
		reset();
		_isStart = true;
	}
	virtual void stop() {
		_isStart = false;
	}

	virtual void onNote(midiNodeEvent e) = 0;

	virtual string getInfo() = 0;

protected:
	bool _isSetup, _isStart;

};
