#include "fourColorCircle.h"

#pragma region colorCircle
void fourColorCircle::colorCircle::init(ofVec3f center, float baseSize, float deltaLength, int keyNodeNum, int resolution)
{
	_center.set(center);
	_baseRadius = baseSize;
	_deltaLength = deltaLength;
	_keyNodeIndex.resize(keyNodeNum);
	_circleNode.resize(keyNodeNum + keyNodeNum * resolution);
	_noiseValue.resize(keyNodeNum + keyNodeNum * resolution);

	for (int i = 0; i < keyNodeNum; i++)
	{
		_keyNodeIndex[i] = i * resolution;
	}
	_triggerWindowSize = resolution;
	initTriggerValue();
	initMesh();
	reset();
	_isInit = true;
}

void fourColorCircle::colorCircle::update()
{
	if (!_isInit)
	{
		return;
	}
	updateNode();
	updateNoise();
	updateMesh();
}

void fourColorCircle::colorCircle::triggerKey(unsigned int index)
{
	if (!_isInit)
	{
		return;
	}

	if (index >= 0 && index < _keyNodeIndex.size())
	{
		setNodeByKey(_keyNodeIndex[index]);
	}
}

void fourColorCircle::colorCircle::draw()
{
	if (!_isInit)
	{
		return;
	}

	ofPushStyle();

	//_mesh.draw();
	_mesh.drawWireframe();
	ofPopStyle();
}

void fourColorCircle::colorCircle::reset()
{
	for (auto& iter : _circleNode)
	{
		iter = 0.0f;
	}
}
void fourColorCircle::colorCircle::setNodeByKey(int index)
{
	int tIdx = 0;
	for (int i = -_triggerWindowSize; i <= _triggerWindowSize; i++, tIdx++)
	{
		int v = index + i;
		if (v < 0)
		{
			v += _circleNode.size();
		}
		else if (v >= _circleNode.size())
		{
			v -= _circleNode.size();
		}

		if (abs(_triggerValue[tIdx]) > abs(_circleNode[v]))
		{
			_circleNode[v] = _triggerValue[tIdx];
		}


	}
}
void fourColorCircle::colorCircle::updateNode()
{
	for (auto& iter : _circleNode)
	{
		if (abs(iter) > 0.01f)
		{
			iter *= cColorCircleAttenuation;
			if (abs(iter) <= 0.01f)
			{
				iter = 0;
			}
		}
	}
}
void fourColorCircle::colorCircle::initTriggerValue()
{
	int num = _triggerWindowSize * 2 + 1;
	_triggerValue.resize(num);

	float delta = TWO_PI / (num-1);
	float rad = -PI;
	for (int i = 0; i < num; i++)
	{
		_triggerValue[i] = cos(rad) * 0.5f + 0.5f;
		rad += delta;
	}
}
void fourColorCircle::colorCircle::initMesh()
{
	_mesh.addVertex(_center);

	float delta = TWO_PI / _circleNode.size();
	for (int i = 0; i < _circleNode.size(); i++)
	{
		ofVec2f pos = _center;
		pos.x += sin(delta * i) * _baseRadius;
		pos.y += cos(delta * i) * _baseRadius;
		_mesh.addVertex(pos);
	}

	for (int i = 0; i < _circleNode.size(); i++)
	{
		int idx2 = i;
		int idx3 = (i + 1) % _circleNode.size();

		_mesh.addTriangle(0, idx2 + 1, idx3 + 1);
	}
}
void fourColorCircle::colorCircle::updateMesh()
{
	float delta = TWO_PI / _circleNode.size();
	for (int i = 0; i < _circleNode.size(); i++)
	{
		ofVec2f pos = _center;
		float r = (_baseRadius + _circleNode[i] * _deltaLength + _noiseValue[i] * _noiseLength);
		pos.x += sin(delta * i) * r;
		pos.y += cos(delta * i) * r;
		_mesh.setVertex(i + 1, pos);
	}

}

void fourColorCircle::colorCircle::enableNoise(float noiseLength)
{
	_useNoise = true;
	_noiseLength = noiseLength;
	_noiseDelta = ofRandom(0, 1);
}
void fourColorCircle::colorCircle::disableNoise()
{
	_useNoise = false;
}
void fourColorCircle::colorCircle::updateNoise()
{
	if (_useNoise)
	{
		float t = ofGetElapsedTimef();
		float num = _noiseValue.size() * 0.5;
		for (int i = 0; i < num; i++)
		{
			float v = ofNoise(t * _noiseDelta, i * (5.0f / _noiseValue.size())) - 0.5f;
			_noiseValue[i] = v;
			_noiseValue[_noiseValue.size() - i - 1] = v;
		}
	}
}
#pragma endregion