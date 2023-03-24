// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ThreadBase.h"

struct FThreadInfo
{
	CThreadBase* Action;
	FRunnableThread* Thread;
};

class UE11_API CNetworkManager
{
private:
	CNetworkManager();
	~CNetworkManager();

private:
	TMap<FString, class CNetworkSession*>	mSessionMap;
	TMap<FString, FThreadInfo*>	mThreadMap;
	TMap<FString, class CPacketQueue*>	mPacketQueueMap;

public:
	bool Init();
	bool CheckSession(const FString& Name);
	bool Connect(const FString& Name, const FString& IPAddr, int32 Port);
	bool Close(const FString& Name);
	bool Receive(const FString& Name, int32& PacketHeader, int32& Length, uint8* Packet);
	bool Send(const FString& Name, int32 PacketHeader, int32 Length, uint8* Packet);
	class CNetworkSession* FindSession(const FString& Name);
	bool RemoveSession(const FString& Name);

	bool CreatePacketQueue(const FString& Name);
	class CPacketQueue* FindPacketQueue(const FString& Name);

public:
	bool SuspendThread(const FString& Name, bool Pause);
	bool RemoveThread(const FString& Name);

	template <typename T>
	FThreadInfo* CreateThread(const FString& Name)
	{
		if (mThreadMap.Contains(Name))
			return nullptr;

		FThreadInfo* Info = new FThreadInfo;

		Info->Action = new T;

		Info->Action->SetName(Name);

		Info->Action->Init();

		Info->Thread = FRunnableThread::Create(Info->Action, *Name);
		Info->Thread->Suspend();

		mThreadMap.Emplace(Name, Info);

		return Info;
	}

private:
	static CNetworkManager* mInst;

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
