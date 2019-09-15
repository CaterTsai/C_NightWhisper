#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "Spout/Spout.h"
#include "midiMgr.h"
#include "pianoView.h"
#include "songFollower.h"
#include "drawMgr.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	float _mainTimer;
	ofEasyCam _cam;


#pragma region Scence
public:
	void drawScence();

private:
	drawMgr _drawMgr;
#pragma endregion


#pragma region MidiMgr
public:
	void onMidiNoteEvent(midiNodeEvent& e);

private:
	void initMidi();

private:
	int _playIdx;

#pragma endregion

#pragma region Follower
private:
	void updateFollower(float delta);
private:
	bool _usedDebugTimer;
	float _debugTimer;
	songFollower _follower;
#pragma endregion

#pragma region Post
private:
	ofxPostProcessing _post;
	ofFbo _canvas;
#pragma endregion

#pragma region Spout
private:
	void initSpout();
	void updateSpout();
	void beginSpout();
	void endSpout();
	void drawSpoutPreview(int x, int y, int w, int h);

private:
	bool _isSpoutInit;
	SpoutSender _spoutSender;
	ofFbo _spoutCanvas;
#pragma endregion



};
