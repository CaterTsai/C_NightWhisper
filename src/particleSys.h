#pragma once

#include "ofMain.h"

struct particleParam {
	particleParam() :
		haveLifetime(true)
		, fade(false)
		, lifeTime(0.0f)
		, pos(0)
		, vec(0)
		, acc(0)
		, rAxis(0)
		, deg(0)
		, rotV(0) 
		, cIdx(0)
	{}

	bool haveLifetime;
	bool fade;
	float lifeTime;
	ofVec3f pos, vec, acc;
	ofVec3f rAxis;
	float deg, rotV;
	ofColor color;
	float size;
	int cIdx;
};

class particle
{
public:
	
public:
	particle()
		:die(false)
		,timer(0)
	{}

	particle(particleParam p)
		:die(false)
	{
		param = p;
		timer = p.lifeTime;
	}
	void update(float delta)
	{
		if (die)
		{
			return;
		}
		if (param.haveLifetime)
		{
			timer -= delta;
			if (timer < 0.0f)
			{
				die = true;
				return;
			}

			if (param.fade)
			{
				float p = (1.0f - timer / param.lifeTime);
				if (p >= 0 && p < 0.1)
				{
					param.color.a = ofMap(p, 0.0f, 0.1f, 0, 255);
				}
				else if (p >= 0.9 && p <= 1.0f)
				{
					param.color.a = ofMap(p, 0.9f, 1.0f, 255, 0);
				}
			}
		}

		param.vec += param.acc * delta;
		param.pos += param.vec * delta;
		param.deg += param.rotV * delta;

		if (param.deg >= 360)
		{
			param.deg -= 360;
		}
		else if (param.deg <= -360)
		{
			param.deg += 360;
		}
	}

	bool isDie()
	{
		return die;
	}

public:
	particleParam param;

private:
	bool die;
	float timer;
};

class particleSys {
public:
	particleSys()
	{
		reset();
	}

	void update(float delta);
	void clear();
	void reset();

	list<particle>& getParticleList();

	void startEmit();
	void stopEmit();

	bool getIsStart() {
		return isStart;
	}
private:
	void checkDie();
	void checkEmit(float delta);
	void emit();

	ofVec3f getBetweenTwoVec(ofVec3f& begin, ofVec3f& end);
public:
	bool needFade, haveLifetime;
	float emitDuration;
	int emitNum, maxNum;

	float emitLifeTimeBegin, emitLifeTimeEnd;

	ofVec3f emitPosRangeBegin, emitPosRangeEnd;
	ofVec3f emitDirectionRangeBegin, emitDirectionRangeEnd;
	ofVec3f emitAccRangeBegin, emitAccRangeEnd;
	float emitSpeedRangeBegin, emitSpeedRangeEnd;

	ofVec3f emitRAxisBegin, emitRAxisEnd;
	float emitRotDRangeBegin, emitRotDRangeEnd;
	float emitRotVRangeBegin, emitRotVRangeEnd;

	ofColor emitColorBegin, emitColorEnd;
	float emitSizeBegin, emitSizeEnd;

	int emitCustomIndexBegin, emitCustomIndexEnd;

private:
	bool isStart;
	float timer;
	list<particle> particleList;

};