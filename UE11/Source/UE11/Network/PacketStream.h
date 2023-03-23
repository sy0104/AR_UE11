// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

/**
 * 
 */
class UE11_API CPacketStream
{
public:
	CPacketStream();
	~CPacketStream();

private:
	uint8*	mBuffer;
	int32	mLength;

public:
	void SetBuffer(uint8* Buffer)
	{
		mBuffer = Buffer;
	}

public:
	int32 GetLength()
	{
		return mLength;
	}

public:
	void AddData(void* Data, int32 Size);
	void GetData(void* Data, int32 Size);
};
