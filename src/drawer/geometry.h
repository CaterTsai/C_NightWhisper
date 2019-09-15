#pragma once

#include "drawBase.h"


#pragma region dodecahedron
class dodecahedron {
public:
	void init(float r, ofColor lineColor, ofColor faceColor);
	
	void draw();
	void drawEdge();
	void drawFaceCenter();

	void triggerOn(int index);
	void triggerOff(int index);

private:
	void createVertex(float r);
	void createFaceTri();
	void setFaceCenter();
	void setFace();

private:
	ofColor _lineColor, _faceColor;
	ofVboMesh _mesh;

	const int faceIndex[cGeometryFaceNum][5] = { { 0, 12, 4, 5, 16 },{ 0, 1, 13, 8, 12 },{ 1, 17, 6, 7, 13 },
	{ 1, 0, 16, 11, 17 },{ 8, 13, 7, 15, 9 },{ 7, 6, 19, 2, 15 },
	{ 6,17, 11, 10, 19 },{ 11, 16, 5, 18, 10 },{ 12, 8, 9, 14, 4 },
	{ 5, 4, 14, 3, 18 },{ 18, 3, 2, 19, 10 },{ 2, 3, 14, 9, 15 } };

public:
	array<ofVec3f, cGeometryFaceNum> _faceVector;
	array<ofPath, cGeometryFaceNum> _faceList;
	array<bool, cGeometryFaceNum> _faceFlag;

};
#pragma endregion

class geometry : public drawBase{
private:
	class point {
	public:
		point()
			:_isDead(false), _timer(5.0f)
		{}

		point(ofVec3f pos)
			:_isDead(false), _pos(pos), _timer(5.0f)
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
	geometry();
	void init() override;
	void update(float delta) override;
	void draw() override;
	void reset() override;

	void onNote(midiNodeEvent e) override;

	string getInfo() override;

private:
	void updatePointList(float delta);
	void drawPointList();
	list<ofVec3f> getPointOnBall();
	void addPointList(list<ofVec3f> newPList);

	

private:
	float _ballR;
	float _viewRotD, _viewRotV;

	float _rotD, _rotV;
	ofVec3f _rotAxis;
	dodecahedron _dodecahedron;

	list<point> _pList;
};