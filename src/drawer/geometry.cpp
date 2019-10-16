#include "geometry.h"

#pragma region dodecahedron
void dodecahedron::init(float r, ofColor lineColor, ofColor faceColor)
{
	_lineColor.set(lineColor);
	_faceColor.set(faceColor);

	for (auto& iter : _faceFlag)
	{
		iter = false;
	}

	_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES);
	createVertex(r);
	setFaceCenter();
	setFace();
}

void dodecahedron::draw()
{
	ofSetColor(255);
	drawEdge();
	
}
void dodecahedron::drawEdge()
{
	ofPushStyle();
	ofSetColor(255);
	for (int i = 0; i < cGeometryFaceNum; i++)
	{
		_faceList[i].draw();
	}
	ofPopStyle();

	
}
void dodecahedron::drawFaceCenter()
{
	ofPushStyle();
	ofSetColor(255, 0, 0);
	for (auto &iter : _faceVector)
	{
		ofDrawSphere(iter, 10);
	}
	ofPopStyle();
}
void dodecahedron::triggerOn(int index)
{
	if (index < 0 || index >= cGeometryFaceNum)
	{
		return;
	}

	if (!_faceFlag[index])
	{
		_faceFlag[index] = true;
		_faceList[index].setFilled(true);
	}
}
void dodecahedron::triggerOff(int index)
{
	if (index < 0 || index >= cGeometryFaceNum)
	{
		return;
	}

	if (_faceFlag[index])
	{
		_faceFlag[index] = false;
		_faceList[index].setFilled(false);
	}
}
void dodecahedron::createVertex(float r)
{
	float sigma = ((sqrt(5.0f) + 1) / 2.0f);
	float invSigma = (1.0f / sigma);
	sigma *= r;
	invSigma *= r;

	int sign[4][2] = { { 1, 1 },{ 1, -1 },{ -1, -1 },{ -1, 1 } };
	//yz
	for (int i = 0; i < 4; i++)
	{
		float v1 = sigma * sign[i][0];
		float v2 = invSigma * sign[i][1];
		_mesh.addVertex(ofVec3f(0, v1, v2));
	}

	//xz
	for (int i = 0; i < 4; i++)
	{
		float v1 = sigma * sign[i][0];
		float v2 = invSigma * sign[i][1];
		_mesh.addVertex(ofVec3f(v2, 0, v1));
	}

	//zy
	for (int i = 0; i < 4; i++)
	{
		float v1 = sigma * sign[i][0];
		float v2 = invSigma * sign[i][1];
		_mesh.addVertex(ofVec3f(v1, v2, 0));
	}

	int signCube[8][3] = { { 1, 1, 1 },{ 1, 1, -1 },{ 1, -1, 1 },{ 1, -1, -1 },
	{ -1, 1, 1 },{ -1, 1, -1 },{ -1, -1, 1 },{ -1, -1, -1 } };
	for (int i = 0; i < 8; i++)
	{
		float v1 = r * signCube[i][0];
		float v2 = r * signCube[i][1];
		float v3 = r * signCube[i][2];
		_mesh.addVertex(ofVec3f(v1, v2, v3));
	}
}
void dodecahedron::createFaceTri()
{
	int triOrder[3][3] = { { 0, 1, 4 },{ 1, 2, 4 },{ 2, 3, 4 } };
	for (int i = 0; i < cGeometryFaceNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			_mesh.addTriangle(faceIndex[i][triOrder[j][0]], faceIndex[i][triOrder[j][1]], faceIndex[i][triOrder[j][2]]);
		}

	}
}
void dodecahedron::setFaceCenter()
{
	auto vertices = _mesh.getVertices();
	for (int i = 0; i < cGeometryFaceNum; i++)
	{
		ofVec3f center;
		for (int j = 0; j < 5; j++)
		{
			center += vertices[faceIndex[i][j]];
		}
		center /= 5.0f;
		_faceVector[i] = center.normalized();
	}
}
void dodecahedron::setFace()
{
	auto vertices = _mesh.getVertices();
	for (int i = 0; i < cGeometryFaceNum; i++)
	{
		ofPath face;
		face.moveTo(vertices[faceIndex[i][0]]);
		for (int j = 1; j < 5; j++)
		{
			face.lineTo(vertices[faceIndex[i][j]]);
		}
		face.close();

		face.setStrokeColor(_lineColor);
		face.setStrokeWidth(2);
		face.setFilled(false);
		face.setFillColor(_faceColor);
		_faceList[i] = face;
	}
}
#pragma endregion

#pragma region Class geometry
geometry::geometry()
	:drawBase()
{
	init();
}

void geometry::init()
{
	_dodecahedron.init(60, ofColor(255), ofColor(128));
	_ballR = 450;
	_rotV = 180;
	_viewRotV = 20;
	_rotAxis.set(1, 1, 0);
}

void geometry::update(float delta)
{
	if (!_isStart)
	{
		return;
	}

	_rotD += _rotV * delta;
	_viewRotD += _viewRotV * delta;
	if (_rotD > 360)
	{
		_rotD -= 360;
	}

	_rotAxis.x = ofNoise(ofGetElapsedTimef() * 0.3);
	_rotAxis.y = ofNoise(ofGetElapsedTimef() * 0.5);
	_rotAxis.z = ofNoise(ofGetElapsedTimef() * 0.2);
	updatePointList(delta);
}

void geometry::draw()
{
	if (!_isStart)
	{
		return;
	}

	ofPushMatrix();
	ofRotateY(_viewRotD);

	ofPushStyle();
	ofEnableDepthTest();
	ofSetColor(255);

	ofPushMatrix();

	ofRotate(_rotD, _rotAxis.x, _rotAxis.y, _rotAxis.z);
	_dodecahedron.draw();

	ofPopMatrix();

	//Draw New Point
	auto plist = getPointOnBall();
	ofSetColor(255, 0, 0);
	for (auto& iter : plist) {
		ofLine(ofVec3f(0), iter);
	}
	addPointList(plist);

	//Draw Point
	drawPointList();

	ofNoFill();
	ofSetColor(255, 150);
	ofDrawSphere(_ballR);

	ofDisableDepthTest();
	ofPopStyle();

	ofPopMatrix();
}

void geometry::reset()
{
	_rotD = 0.0f;
	_viewRotD = 0.0f;
	_pList.clear();
}

void geometry::onNote(midiNodeEvent e)
{
	if (!_isStart)
	{
		return;
	}

	if (e.type == e.eNodeOn)
	{
		_dodecahedron.triggerOn(e.note % 12);
	}
	else
	{
		_dodecahedron.triggerOff(e.note % 12);
	}

}

string geometry::getInfo()
{
	return "[Draw]Geometry";
}

void geometry::updatePointList(float delta)
{
	for (auto& iter : _pList)
	{
		iter.update(delta);
	}

	_pList.remove_if(
		[](const point& p) {
		return p._isDead;
		}
	);
}

void geometry::drawPointList()
{
	ofPushStyle();
	ofNoFill();
	for (auto& iter : _pList)
	{
		ofSetColor(255, 255.0f * (iter._timer/5.0f));
		ofDrawCircle(iter._pos, 10);
	}

	ofPopStyle();
}

list<ofVec3f> geometry::getPointOnBall()
{
	ofVec3f center;
	ofVec3f pos;

	center.set(0);
	pos.set(0);

	list<ofVec3f> pList;
	for (int i = 0; i < cGeometryFaceNum; i++)
	{
		if (!_dodecahedron._faceFlag[i])
		{
			continue;
		}

		ofVec3f v = _dodecahedron._faceVector[i];
		ofVec3f k = pos;

		auto ksc = (k - center);
		float b = 2 * v.dot(ksc);
		float c = ksc.squareDistance(ofVec3f(0)) - _ballR*_ballR;

		float sol = sqrt(b*b - 4 * c);
		float ans1 = (-b + sol) / 2.0f;
		float ans2 = (-b - sol) / 2.0f;

		float a = (ans1 > 0 ? ans1 : ans2);

		v.rotate(_rotD, _rotAxis);
		ofVec3f p = k + v * a;
		pList.push_back(p);
	}

	return pList;
}

void geometry::addPointList(list<ofVec3f> newPList)
{
	for (auto& iter : newPList)
	{
		point newP(iter);
		_pList.push_back(newP);
	}
}

#pragma endregion


