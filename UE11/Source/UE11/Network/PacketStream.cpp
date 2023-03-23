// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketStream.h"

CPacketStream::CPacketStream()
	: mBuffer(nullptr)
	, mLength(0)
{
}

CPacketStream::~CPacketStream()
{
}

void CPacketStream::AddData(void* Data, int32 Size)
{
	FMemory::Memcpy(mBuffer + mLength, Data, Size);
	mLength += Size;
}

void CPacketStream::GetData(void* Data, int32 Size)
{
	FMemory::Memcpy(Data, mBuffer + mLength, Size);
	mLength += Size;
}
