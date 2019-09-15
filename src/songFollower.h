#pragma once

#include "ofMain.h"
#include "ofxUDPManager.h"

#include "songInfo.h"

const int cUDPLength = 250;

class songFollower {
public:
	songFollower();
	void setup(int port);
	void update();

	void start();
	void stop();

	songInfo& getSongInfo();

	inline bool isStart() {
		return _isStart;
	}

private:
	bool _isSetup, _isStart;
	ofxUDPManager _udpMgr;

	songInfo _songInfo;

	ofxJSONElement _jsonReader;
};