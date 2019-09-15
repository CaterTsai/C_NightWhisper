#include "particleSys.h"

void particleSys::update(float delta)
{
	if (isStart)
	{
		checkEmit(delta);
	}

	for (auto &iter : particleList)
	{
		iter.update(delta);
	}
	checkDie();
}

void particleSys::clear()
{
	particleList.clear();
}

void particleSys::reset()
{
	isStart = false;
	needFade = false;
	emitDuration = 1.0f;
	emitNum = 1;
	maxNum = 1000;
	emitPosRangeBegin.set(0); 
	emitPosRangeEnd.set(0);
	emitDirectionRangeBegin.set(0);
	emitDirectionRangeEnd.set(0);
	emitAccRangeBegin.set(0);
	emitAccRangeEnd.set(0);
	emitSpeedRangeBegin = emitSpeedRangeEnd = 0.0f;
	emitCustomIndexBegin = emitCustomIndexEnd = 0;

	emitColorBegin = emitColorEnd = ofColor(255);

	particleList.clear();
	
}

list<particle>& particleSys::getParticleList()
{
	return particleList;
}

void particleSys::startEmit()
{
	isStart = true;
	timer = 0;
}

void particleSys::stopEmit()
{
	isStart = false;
}

void particleSys::checkDie()
{
	auto newEnd = remove_if(
		particleList.begin(),
		particleList.end(),
		[](particle& p) {return p.isDie();}
	);
	particleList.erase(newEnd, particleList.end());
}

void particleSys::checkEmit(float delta)
{
	timer -= delta;
	if (timer < 0.0f && particleList.size() < maxNum)
	{
		int num = MIN(emitNum, maxNum - particleList.size());
		for (int i = 0; i < num; i++)
		{
			emit();
		}
		timer = emitDuration;
	}
}

void particleSys::emit()
{
	particleParam pParam;
	pParam.fade = needFade;
	pParam.lifeTime = ofRandom(emitLifeTimeBegin, emitLifeTimeEnd);

	pParam.pos = getBetweenTwoVec(emitPosRangeBegin, emitPosRangeEnd);
	float speed = ofRandom(emitSpeedRangeBegin, emitSpeedRangeEnd);

	pParam.vec = speed * getBetweenTwoVec(emitDirectionRangeBegin, emitDirectionRangeEnd);
	pParam.acc = getBetweenTwoVec(emitAccRangeBegin, emitAccRangeEnd);
	
	pParam.rAxis = getBetweenTwoVec(emitRAxisBegin, emitRAxisEnd);
	pParam.deg = ofRandom(emitRotDRangeBegin, emitRotDRangeEnd);
	pParam.rotV = ofRandom(emitRotVRangeBegin, emitRotVRangeEnd);

	pParam.color = emitColorBegin.getLerped(emitColorEnd, ofRandom(1.0f));
	pParam.size = ofRandom(emitSizeBegin, emitSizeEnd);

	pParam.cIdx = emitCustomIndexBegin + rand() % (emitCustomIndexEnd - emitCustomIndexBegin + 1);

	particle newP(pParam);
	particleList.push_back(particle(pParam));
}

ofVec3f particleSys::getBetweenTwoVec(ofVec3f & begin, ofVec3f & end)
{
	ofVec3f ret;
	ret.x = ofRandom(begin.x, end.x);
	ret.y = ofRandom(begin.y, end.y);
	ret.z = ofRandom(begin.z, end.z);
	return ret;
}

