// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

class UE11_API CThreadBase	:
	public FRunnable
{
public:
	CThreadBase();
	~CThreadBase();

protected:
	bool	mLoop;
	FString	mName;

public:
	void SetName(const FString& Name)
	{
		mName = Name;
	}

	void SetLoop(bool Loop)
	{
		mLoop = Loop;
	}

public:
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
};
