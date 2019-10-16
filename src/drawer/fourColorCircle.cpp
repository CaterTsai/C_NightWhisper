#include "fourColorCircle.h"

#pragma region Basic Function
void fourColorCircle::init()
{
	for (auto& iter : _colorCircleSet)
	{
		iter._colorCircle.init(ofVec3f(0), 300, ofRandom(80, 120), cFourColorNoteNumEach, 40);
		iter._colorCircle.enableNoise(60);
	}
	_colorCircleSet[0]._color.set(252, 0, 0, 255);
	_colorCircleSet[1]._color.set(3, 240, 0, 255);
	_colorCircleSet[2]._color.set(0, 15, 192, 255);
	_colorCircleSet[3]._color.set(0, 0, 255);
}

void fourColorCircle::update(float delta)
{
	if (!_isStart)
	{
		return;
	}

	for (auto& iter : _colorCircleSet)
	{
		iter.update(delta);
	}
}

void fourColorCircle::draw()
{
	if (!_isStart)
	{
		return;
	}

	ofPushStyle();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	for (auto& iter : _colorCircleSet)
	{
		iter.draw();
	}
	ofPopStyle();

}

void fourColorCircle::reset()
{
	for (auto& iter : _colorCircleSet)
	{
		iter._colorCircle.reset();
	}
}

void fourColorCircle::onNote(midiNodeEvent e)
{
	if (e.type == e.eNodeOn)
	{
		int note = e.note - _startNo;
		int noteOnCircle = note % cFourColorNoteNumEach;
		int indexOfCircle = (int)floor(note / (float)cFourColorNoteNumEach);

		if (indexOfCircle >= 0 && indexOfCircle < 4)
		{
			_colorCircleSet[indexOfCircle]._colorCircle.triggerKey(noteOnCircle);
		}
	}
}

string fourColorCircle::getInfo()
{
	return "[Draw]fourColorCircle";
}

#pragma endregion