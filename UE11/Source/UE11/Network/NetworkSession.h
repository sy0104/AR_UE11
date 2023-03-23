// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

/**
 * 
 */
class UE11_API CNetworkSession
{
public:
	CNetworkSession();
	~CNetworkSession();

private:
	FSocket*	mSocket;
	bool		mConnect;
	uint8		mReceivePacket[PACKET_SIZE];
	uint8		mSendPacket[PACKET_SIZE];

public:
	bool Connect(const FString& IPAddr, int32 Port);
	void Close();
	bool Receive(int32& PacketHeader, int32& Length, uint8* Packet);
	bool Send(int32 PacketHeader, int32 Length, uint8* Packet);
};
