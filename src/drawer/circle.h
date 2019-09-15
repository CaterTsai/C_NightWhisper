#pragma once

#include "drawBase.h"

class circle : public drawBase {

public:
	class point {
	public:
		point()
			:_isDead(false), _timer(3.0f)
		{}

		point(ofVec3f pos)
			:_isDead(false), _pos(pos), _timer(3.0f)
		{}

		void update(float delta) {
			if (!_isDead)
			{
				_timer -= delta;
				if (_timer < 0.0f)
				{
					_isDead = true;
				}
			}
		}
	public:
		bool _isDead;
		float _timer;
		ofVec3f _pos;
	};

public:
	circle() :
		drawBase(),
		_raius(200),
		_rotD(0),
		_rotV(5)
	{};
	void init() override;
	void update(float delta) override;
	void draw() override;
	void reset() override;

	void onNote(midiNodeEvent e) override;

	string getInfo() override;

private:
	void updateCircle();
	void drawCircle();

	void updatePoint(float delta);
	void drawPointLine();
	void addPoint();

private:
	float _raius;
	float _rotD, _rotV;

	array<bool, cNoteNum> _noteFlag;
	array<ofVec2f, cNoteNum> _noteList;
	list<point> _pointList;
};