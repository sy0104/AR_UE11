// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"
#include "../Network/NetworkManager.h"
#include "../Network/PacketQueue.h"
#include "../Network/ReceiveThread.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mSendBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("SendButton"))));
	mInputText = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("InputText"))));

	// StartBtn 에 클릭, 호버, 언호버 상황이 발생할 때 호출 시킬 델리게이트 등록
	mSendBtn->OnClicked.AddDynamic(this, &UChatWidget::OnSendBtnClicked);

	// 채팅 서버에 접속한다.
	CNetworkManager::GetInst()->Connect(TEXT("ChatSession"),
		TEXT("127.0.0.1"), 7070);

	// 채팅용 스레드 생성
	FThreadInfo* Info = CNetworkManager::GetInst()->CreateThread<CReceiveThread>(TEXT("ChatReceive"));

	((CReceiveThread*)Info->Action)->SetSession(CNetworkManager::GetInst()->FindSession(TEXT("ChatSession")));

	mQueue = CNetworkManager::GetInst()->FindPacketQueue(TEXT("ChatReceive_Queue"));

	CNetworkManager::GetInst()->SuspendThread(TEXT("ChatSession"), false);

	//mInputText->OnTextCommitted
}

void UChatWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// 채팅서버 접속을 종료한다.
	CNetworkManager::GetInst()->Close(TEXT("ChatSession"));

	CNetworkManager::GetInst()->RemoveThread(TEXT("ChatReceive"));

	CNetworkManager::GetInst()->RemoveSession(TEXT("ChatSession"));
}

void UChatWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);

	if (!mQueue->Empty())
	{
		int32	Header, Length;
		uint8	Packet[PACKET_SIZE] = {};

		mQueue->Pop(Header, Length, Packet);
	}
}

void UChatWidget::OnSendBtnClicked()
{
	LOGSTRING(mInputText->Text.ToString());
}
