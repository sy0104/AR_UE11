// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkManager.h"
#include "NetworkSession.h"
#include "PacketQueue.h"

CNetworkManager* CNetworkManager::mInst = nullptr;

CNetworkManager::CNetworkManager()
{
}

CNetworkManager::~CNetworkManager()
{
	for (auto& ThreadInfo : mThreadMap)
	{
		ThreadInfo.Value->Action->Exit();
	}
	
	for (auto& Session : mSessionMap)
	{
		Session.Value->Close();
	}

	for (auto& ThreadInfo : mThreadMap)
	{
		ThreadInfo.Value->Thread->Kill();
		ThreadInfo.Value->Thread->WaitForCompletion();

		delete	ThreadInfo.Value->Thread;
		delete ThreadInfo.Value->Action;
		delete ThreadInfo.Value;
	}

	for (auto& Session : mSessionMap)
	{
		delete Session.Value;
	}
	
	for (auto& Queue : mPacketQueueMap)
	{
		delete Queue.Value;
	}

	/*for (auto iter = mSessionMap.CreateConstIterator(); iter; ++iter)	
	{
		iter.Key();
		iter->Key;
		iter.Key(), iter->Key
		iter.Value()	iter->Value
	}*/
}

bool CNetworkManager::Init()
{
	return true;
}

bool CNetworkManager::CheckSession(const FString& Name)
{
	return mSessionMap.Contains(Name);
}

bool CNetworkManager::Connect(const FString& Name, const FString& IPAddr,
	int32 Port)
{
	CNetworkSession* Session = nullptr;

	// 세션이 없을경우 새로 생성한다.
	if (!CheckSession(Name))
	{
		Session = new CNetworkSession;

		Session->SetName(Name);

		// 새로 생성된 Session을 목록에 추가한다.
		//mSessionMap.Add(,);
		// 임시객체 생성을 피할 수 있다.
		mSessionMap.Emplace(Name, Session);
	}

	else
		Session = FindSession(Name);

	if (Session->IsConnect())
		return false;

	return Session->Connect(IPAddr, Port);
}

bool CNetworkManager::Close(const FString& Name)
{
	// Session이 없으면 실패다.
	if (!CheckSession(Name))
		return false;

	CNetworkSession* Session = FindSession(Name);

	if (!Session->IsConnect())
		return false;

	Session->Close();

	return true;
}

bool CNetworkManager::Receive(const FString& Name, int32& PacketHeader, 
	int32& Length, uint8* Packet)
{
	// Session이 없으면 실패다.
	if (!CheckSession(Name))
		return false;

	CNetworkSession* Session = FindSession(Name);

	if (!Session->IsConnect())
		return false;

	return Session->Receive(PacketHeader, Length, Packet);
}

bool CNetworkManager::Send(const FString& Name, int32 PacketHeader,
	int32 Length, uint8* Packet)
{
	// Session이 없으면 실패다.
	if (!CheckSession(Name))
		return false;

	CNetworkSession* Session = FindSession(Name);

	if (!Session->IsConnect())
		return false;

	return Session->Send(PacketHeader, Length, Packet);
}

CNetworkSession* CNetworkManager::FindSession(const FString& Name)
{
	if (!mSessionMap.Contains(Name))
		return nullptr;

	return mSessionMap.FindRef(Name);
}

bool CNetworkManager::RemoveSession(const FString& Name)
{
	// Session이 없으면 실패다.
	if (!CheckSession(Name))
		return false;

	CNetworkSession* Session = FindSession(Name);

	delete	Session;

	mSessionMap.Remove(Name);

	return false;
}

bool CNetworkManager::CreatePacketQueue(const FString& Name)
{
	if (mPacketQueueMap.Contains(Name))
		return false;

	CPacketQueue* Queue = new CPacketQueue;

	mPacketQueueMap.Emplace(Name, Queue);

	return true;
}

CPacketQueue* CNetworkManager::FindPacketQueue(const FString& Name)
{
	if (!mPacketQueueMap.Contains(Name))
		return nullptr;

	return mPacketQueueMap.FindRef(Name);
}

bool CNetworkManager::SuspendThread(const FString& Name, bool Pause)
{
	if (!mThreadMap.Contains(Name))
		return false;

	mThreadMap.FindRef(Name)->Thread->Suspend(Pause);

	return true;
}

bool CNetworkManager::RemoveThread(const FString& Name)
{
	if (!mThreadMap.Contains(Name))
		return false;

	delete mThreadMap.FindRef(Name)->Thread;
	delete mThreadMap.FindRef(Name)->Action;
	delete mThreadMap.FindRef(Name);

	mThreadMap.Remove(Name);

	return false;
}
