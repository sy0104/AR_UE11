// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThreadBase.h"

class UE11_API CReceiveThread	:
	public CThreadBase
{
public:
	CReceiveThread();
	~CReceiveThread();

private:
	class CNetworkSession* mSession;
	class CPacketQueue* mQueue;

public:
	void SetSession(class CNetworkSession* Session)
	{
		mSession = Session;
	}

	void SetPacketQueue(class CPacketQueue* Queue)
	{
		mQueue = Queue;
	}

public:
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
};
