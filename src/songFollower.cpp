#include "songFollower.h"

songFollower::songFollower()
	:_isSetup(false)
	, _isStart(false)
{}

void songFollower::setup(int port)
{
	if (_isSetup)
	{
		_udpMgr.Close();
	}

	_udpMgr.Create();
	_udpMgr.Bind(port);
	_udpMgr.SetNonBlocking(true);

	_isSetup = true;

}

void songFollower::update()
{
	if (!_isSetup || !_isStart)
	{
		return;
	}

	char msg[cUDPLength];
	_udpMgr.Receive(msg, cUDPLength);

	string msgStr = msg;

	if (msgStr != "")
	{	
		_jsonReader.clear();
		if (_jsonReader.parse(msgStr))
		{
			_songInfo.set(_jsonReader);
		}
		else
		{
			ofLog(OF_LOG_WARNING, "[songFollower::update]Read json failed");
		}
	}

}

void songFollower::start()
{
	ofLog(OF_LOG_NOTICE, "[songFollower::start]Start");
	_songInfo.reset();
	
	_isStart = true;
}

void songFollower::stop()
{
	ofLog(OF_LOG_NOTICE, "[songFollower::stop]Stop");
	_isStart = false;
}

songInfo & songFollower::getSongInfo()
{
	return _songInfo;
}
