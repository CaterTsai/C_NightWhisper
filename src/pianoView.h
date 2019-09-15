#pragma once

#include "ofMain.h"

const int cPianoNoteMin = 21; //A0
const int cPianoNoteMax = 108; //C8
const int cPianoNoteNum = cPianoNoteMax - cPianoNoteMin + 1;

class pianoView {
public:
	pianoView() {
		for (auto& iter : _noteFlag)
		{
			iter = false;
		}
	}
	void draw(int x, int y, int w, int h) {

		ofPushMatrix();
		ofTranslate(x, y);

		ofPushStyle();
		float keyH = h / (float)cPianoNoteNum;
		int startNote = cPianoNoteMin;
		float posY = 0;
		for (int i = 0; i < cPianoNoteNum; i++) {
			ofFill();
			ofSetColor(getPianoColor(startNote + i));
			ofDrawRectangle(0, posY, w, keyH);
			ofSetColor(0);
			ofNoFill();
			ofDrawRectangle(0, posY, w, keyH);

			if (_noteFlag[i])
			{
				ofSetColor(255, 0, 0, 200);
				ofFill();
				ofDrawRectangle(0, posY, w, keyH);
			}
			posY += keyH;
		}

		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(0, 0, w, h);
		ofPopStyle();

		ofPopMatrix();
	}

	void noteOn(int note) {
		int index = note - cPianoNoteMin;

		if (index < cPianoNoteNum)
		{
			_noteFlag[index] = true;
		}
	}

	void noteOff(int note) {
		int index = note - cPianoNoteMin;

		if (index < cPianoNoteNum)
		{
			_noteFlag[index] = false;
		}
	}


private:

	ofColor getPianoColor(int node) {
		int val = node % 12;
		if (val == 1 || val == 3 || val == 6 || val == 8 || val == 10)
		{
			return ofColor(0);
		}
		else
		{
			return ofColor(255);
		}
	}

	array<bool, cPianoNoteNum> _noteFlag;
};