// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

/**
 * 
 */
class UE11_API CNetworkManager
{
private:
	CNetworkManager();
	~CNetworkManager();

public:
	bool Init();

private:
	static CNetworkManager*		mInst;

public:
	static CNetworkManager* GetInst()
	{
		if (!mInst)
			mInst = new CNetworkManager;

		return mInst;
	}

	static void DestroyInst()
	{
		if (mInst)
		{
			delete mInst;
			mInst = nullptr;
		}
	}
};
