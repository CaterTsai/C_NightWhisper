#pragma once

#include "ofMain.h"
#include "ofxJson.h"

class songInfo {
public:
	songInfo() {
		reset();
	}
	void reset() {
		isTracking = isPlaying = false;
		estTime = estTempo = 0.0f;

		audioName = "";
		audioDuration = -1.0f;
	}

	void set(ofxJSONElement& value) {
		isTracking = value["is_tracking"].asBool();
		isPlaying = value["is_playing"].asBool();

		estTime = value["est_time"].asFloat();
		estTempo = value["est_tempo"].asFloat();

		audioName = value["ref_file"].asString();

		audioDuration = value["ref_length"].asFloat();
	}

public:
	bool isTracking;
	bool isPlaying;

	float estTime;
	float estTempo;

	string audioName;
	float audioDuration;
};