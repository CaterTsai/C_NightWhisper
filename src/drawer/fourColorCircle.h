#pragma once

#include "drawBase.h"



class fourColorCircle : public drawBase {

private:

#pragma region CLASS colorCircle
	class colorCircle {
	public:
		colorCircle() :_isInit(false), _useNoise(false)
		{}

		void init(ofVec3f center, float baseSize, float deltaLength, int keyNodeNum = 7, int resolution = 10);

		void update();
		void draw();
		void triggerKey(unsigned int index);

		void reset();
	private:
		void setNodeByKey(int index);
		void updateNode();

		void initTriggerValue();

		void initMesh();
		void updateMesh();
	private:
		bool _isInit;
		int _triggerWindowSize;
		float _baseRadius, _deltaLength;
		ofVec3f _center;
		vector<int> _keyNodeIndex;
		vector<float> _triggerValue;
		vector<float> _circleNode;
		ofVboMesh _mesh;
		ofPath _circle;


		//Noise
	public:
		void enableNoise(float noiseLength);
		void disableNoise();
	private:
		void updateNoise();

	private:
		bool _useNoise;
		float _noiseDelta;
		vector<float> _noiseValue;
		float _noiseLength;
	};
#pragma endregion

private:

#pragma region CirCleUnit
	struct circleUnit {
		circleUnit() :
			_rD(ofRandom(0, 359))
		{
			_rV = (rand() % 2 ? 1 : -1) * ofRandom(5, 20);
		}
		void update(float delta) {
			_rD += delta * _rV;
			if (_rV > 0 && _rD > 360.0f)
			{
				_rD -= 360.0f;
			}
			else if (_rV < 0 && _rD < -360.0f)
			{
				_rD += 360.0f;
			}
			_colorCircle.update();
		}

		void draw() {
			ofPushMatrix();
			ofRotateZ(_rD);
			ofSetColor(_color);
			_colorCircle.draw();
			ofPopMatrix();
		}
		colorCircle _colorCircle;
		ofColor _color;
		float _rV, _rD;
	};
#pragma endregion



public:
	fourColorCircle(int startNote)
		:drawBase()
		, _startNo(startNote)
	{
		init();
	};
	void init() override;
	void update(float delta) override;
	void draw() override;
	void reset() override;

	void onNote(midiNodeEvent e) override;

	string getInfo() override;


private:
	array<circleUnit, 4> _colorCircleSet;
	int _startNo;

};

