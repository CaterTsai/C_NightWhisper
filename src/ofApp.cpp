#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	initMidi();
	_usedDebugTimer = false;
	_follower.setup(2233);
	initSpout();

	_post.init(cOutputWidth, cOutputHeight, true);
	_canvas.allocate(cOutputWidth, cOutputHeight, GL_RGBA);
	_post.createPass<BloomPass>()->setEnabled(true);
	_post.createPass<BloomPass>()->setEnabled(true);
	_post.setFlip(false);

	_drawMgr.init();

	ofBackground(0);
	ofSetFrameRate(60);
	_mainTimer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _mainTimer;
	_mainTimer += delta;
	updateFollower(delta);

	_drawMgr.update(delta);

#ifdef DRAW_TO_SPOUT
	//Draw On Spout
	beginSpout();
	drawScence();
	endSpout();
	updateSpout();
#endif // DRAW_TO_SPOUT


}

//--------------------------------------------------------------
void ofApp::draw() {

#ifdef DRAW_TO_SPOUT
	drawSpoutPreview(0, 0, 1024, 768);
#else
	_cam.begin();
	_drawMgr.draw();
	_cam.end();
#endif // DRAW_TO_SPOUT


	//Debug
	stringstream ss;
	ss << "Play Index(up/down):" << _playIdx << endl;
	ss << "Is Play(p/s):" << midiMgr::GetInstance()->isPlaying() << endl;
	ss << "Is Follower Start(q/w):" << _follower.isStart() << endl;
	ss << "Follower T :" << _follower.getSongInfo().estTime << endl;
	ss << "Debug T(e/r):" << _debugTimer << endl;
	ss << "FPS:" << ofGetFrameRate() << endl;
	ss << "Info:" << _drawMgr.getMsg() << endl;

	ofDrawBitmapStringHighlight(ss.str(), 10, 400);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case OF_KEY_UP: {
		_playIdx++;
		_playIdx %= midiMgr::GetInstance()->getMidiNum();
		break;
	}
	case OF_KEY_DOWN: {
		_playIdx--;
		if (_playIdx < 0)
		{
			_playIdx = midiMgr::GetInstance()->getMidiNum() - 1;
		}
		break;
	}
	case 'p': {
		midiMgr::GetInstance()->play(_playIdx);
		break;
	}
	case 's': {
		midiMgr::GetInstance()->stop();
		break;
	}
	case 'q': {
		_follower.start();
		break;
	}
	case 'w': {
		_follower.stop();
		break;
	}
	case 'e': {
		_usedDebugTimer = true;
		_debugTimer = 0.0f;
		break;
	}
	case 'r': {
		_usedDebugTimer = false;
		_debugTimer = 0.0f;
		break;
	}
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':{
		_drawMgr.start(key - '1');
		break;
	}
	case '0': {
		_drawMgr.stop();
		break;
	}
	}
}

void ofApp::drawScence()
{
	ofPushStyle();
	_post.begin(_cam);
	//_cam.begin();
	_drawMgr.draw();
	//_cam.end();

	_post.end();
	ofPopStyle();
}

#pragma region MidiMgr
void ofApp::onMidiNoteEvent(midiNodeEvent & e)
{
	_drawMgr.onNote(e);
}

void ofApp::initMidi()
{
	midiMgr::GetInstance()->add("midi/01-hoffmeister.mid");
	midiMgr::GetInstance()->add("midi/02-porter.mid");
	midiMgr::GetInstance()->add("midi/03-porter.mid");
	midiMgr::GetInstance()->add("midi/04-reger.mid");
	midiMgr::GetInstance()->add("midi/05-beethoven.mid");
	midiMgr::GetInstance()->add("midi/06-beethoven.mid");
	midiMgr::GetInstance()->add("midi/07-encore.mid");

	ofAddListener(midiMgr::GetInstance()->noteEvent, this, &ofApp::onMidiNoteEvent);
	_playIdx = 0;
}
#pragma endregion

void ofApp::updateFollower(float delta)
{
	if (_usedDebugTimer)
	{
		_debugTimer += delta;
		midiMgr::GetInstance()->update(_debugTimer);
	}
	else
	{
		_follower.update();
		auto songInfo = _follower.getSongInfo();
		if (songInfo.isTracking && songInfo.isPlaying)
		{
			midiMgr::GetInstance()->update(songInfo.estTime);
		}
	}

}

#pragma region Spout
void ofApp::initSpout()
{
	char sendername[] = { "[NightWhisper]" };
	_isSpoutInit = _spoutSender.CreateSender(sendername, cOutputWidth, cOutputHeight);
	_spoutCanvas.allocate(cOutputWidth, cOutputHeight, GL_RGBA);
}
void ofApp::updateSpout()
{
	if (_isSpoutInit)
	{
		_spoutSender.SendTexture(
			_spoutCanvas.getTextureReference().getTextureData().textureID,
			_spoutCanvas.getTextureReference().getTextureData().textureTarget,
			_spoutCanvas.getWidth(),
			_spoutCanvas.getHeight()
		);
	}


}
void ofApp::beginSpout()
{
	_spoutCanvas.begin();
	ofClear(0);


}
void ofApp::endSpout()
{
	//Cover

	_spoutCanvas.end();
}
void ofApp::drawSpoutPreview(int x, int y, int w, int h)
{
	ofPushStyle();
	ofSetColor(255);
	_spoutCanvas.draw(x, y, w, h);
	ofPopStyle();
}
#pragma endregion