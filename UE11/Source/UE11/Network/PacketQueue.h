// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

struct FPacketQueueData
{
	int32	Header;
	int32	Length;
	uint8	Packet[PACKET_SIZE];
};

class UE11_API CPacketQueue
{
public:
	CPacketQueue();
	~CPacketQueue();

private:
	FCriticalSection	mCrt;
	FPacketQueueData	mQueue[201];
	int32				mHead;
	int32				mTail;
	int32				mSize;

public:
	void Push(int32 PacketHeader, int32 Length, uint8* Packet);
	void Pop(int32& PacketHeader, int32& Length, uint8* Packet);
	bool Empty();
	int32 Size();
	void Clear();
};
