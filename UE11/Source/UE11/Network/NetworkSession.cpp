// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkSession.h"
#include "PacketStream.h"

CNetworkSession::CNetworkSession()	:
	mSocket(nullptr),
	mConnect(false),
	mReceivePacket{},
	mSendPacket{}
{
}

CNetworkSession::~CNetworkSession()
{
	Close();
}

bool CNetworkSession::Connect(const FString& IPAddr, int32 Port)
{
	// ������ �����Ѵ�.
	mSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(
		NAME_Stream, TEXT("Default"), false);

	// ���ڿ��� ���� IP�� Port�� �̿��ؼ� �ּ� ��ü�� ������ش�.
	FIPv4Address	ip;
	FIPv4Address::Parse(IPAddr, ip);

	TSharedRef<FInternetAddr>	Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	Addr->SetIp(ip.Value);
	Addr->SetPort(Port);

	mConnect = mSocket->Connect(*Addr);

	return true;
}

void CNetworkSession::Close()
{
	if (mConnect)
	{
		mSocket->Close();
		delete mSocket;
	}
}

bool CNetworkSession::Receive(int32& PacketHeader, int32& Length, 
	uint8* Packet)
{
	if (!mConnect)
		return false;

	bool Result = mSocket->Recv(mReceivePacket, PACKET_SIZE, Length,
		ESocketReceiveFlags::None);

	if (!Result || Length == 0)
		return false;

	CPacketStream	stream;
	stream.SetBuffer(mReceivePacket);

	stream.GetData(&PacketHeader, 4);
	stream.GetData(&Length, 4);
	stream.GetData(Packet, Length);

	return true;
}

bool CNetworkSession::Send(int32 PacketHeader, int32 Length,
	uint8* Packet)
{
	if (!mConnect)
		return false;

	CPacketStream	stream;

	stream.SetBuffer(mSendPacket);

	stream.AddData(&PacketHeader, 4);
	stream.AddData(&Length, 4);
	stream.AddData(Packet, Length);

	return mSocket->Send(mSendPacket, stream.GetLength(), Length);
}
