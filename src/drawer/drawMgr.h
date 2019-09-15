#pragma once

#include "circle.h"
#include "geometry.h"
#include "fourColorCircle.h"

class drawMgr {
public:
	drawMgr():
		_isInit(false), _isStart(false), _drawIndex(0){

	}
	void init()
	{
		_drawList.push_back(ofPtr<fourColorCircle>(new fourColorCircle(48)));
		_drawList.push_back(ofPtr<geometry>(new geometry()));
		_drawList.push_back(ofPtr<circle>(new circle()));
		
		_isInit = true;
	}

	void update(float delta)
	{
		if (!_isInit || !_isStart)
		{
			return;
		}
		_drawList[_drawIndex]->update(delta);
	}

	void draw()
	{
		if (!_isInit || !_isStart)
		{
			return;
		}
		_drawList[_drawIndex]->draw();
	}

	void start(int index) {
		if (!_isInit)
		{
			return;
		}

		if (_isStart && index != _drawIndex)
		{
			_drawList[_drawIndex]->stop();

		}
		_isStart = true;
		_drawIndex = index;
		_drawList[_drawIndex]->start();
	}

	void stop() {
		if (!_isInit)
		{
			return;
		}

		if (_isStart) {
			_drawList[_drawIndex]->stop();
		}
		_isStart = false;
	}

	void onNote(midiNodeEvent e) {
		if (!_isInit || !_isStart)
		{
			return;
		}
		_drawList[_drawIndex]->onNote(e);
	}

	string getMsg() {
		if (!_isStart)
		{
			return "[Draw]1:Geometry 2:Circle";
		}
		else
		{
			return _drawList[_drawIndex]->getInfo();
		}
	}

private:
	bool _isInit, _isStart;
	vector<ofPtr<drawBase>> _drawList;
	int _drawIndex;
};