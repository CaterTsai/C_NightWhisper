#include "circle.h"

void circle::init()
{
}

void circle::update(float delta)
{
	if (!_isStart)
	{
		return;
	}

	_rotD += _rotV * delta;
	updateCircle();
	updatePoint(delta);
	addPoint();
}

void circle::draw()
{
	if (!_isStart)
	{
		return;
	}
	drawCircle();
	drawPointLine();
}

void circle::reset()
{
	for (auto& iter : _noteFlag)
	{
		iter = false;
	}

	
	_rotD = 0;
}

void circle::onNote(midiNodeEvent e)
{
	if (!_isStart)
	{
		return;
	}
	int index = e.note - cNoteStart;
	if (e.type == e.eNodeOn)
	{
		_noteFlag[index] = true;
	}
	else {
		_noteFlag[index] = false;
	}
}

string circle::getInfo()
{
	return "[Draw]Circle";
}


void circle::updateCircle()
{



	float diff = 360.0f / cNoteNum;
	ofVec2f pos(_raius, 0);
	pos.rotate(_rotD);
	for (int i = 0; i < cNoteNum; i++)
	{
		_noteList[i] = pos;
		pos.rotate(diff);
	}
}

void circle::drawCircle()
{

	ofPushStyle();
	ofSetColor(255, 150);
	ofNoFill();
	for (int i = 0; i < cNoteNum; i++)
	{
		_noteFlag[i]? ofSetColor(255): ofSetColor(255, 100);

		ofDrawSphere(_noteList[i], 5);
	}
	ofPopStyle();
}

void circle::updatePoint(float delta)
{
	for (auto& iter : _pointList)
	{
		iter.update(delta);
	}

	_pointList.remove_if(
		[](const point& p) {
		return p._isDead;
	}
	);

}

void circle::drawPointLine()
{
	ofPushStyle();
	for (auto& iter : _pointList)
	{
		ofSetColor(255, 255.0f * (iter._timer / 3.0f));
		ofLine(ofVec2f(0), iter._pos);
	}
	ofPopStyle();
}

void circle::addPoint()
{
	for (int i = 0; i < cNoteNum; i++)
	{
		if (_noteFlag[i]) {
			point newP;
			newP._pos = _noteList[i];
			_pointList.push_back(newP);
		}
	}
}
