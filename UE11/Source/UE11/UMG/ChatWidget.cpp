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

	// StartBtn �� Ŭ��, ȣ��, ��ȣ�� ��Ȳ�� �߻��� �� ȣ�� ��ų ��������Ʈ ���
	mSendBtn->OnClicked.AddDynamic(this, &UChatWidget::OnSendBtnClicked);

	// ä�� ������ �����Ѵ�.
	CNetworkManager::GetInst()->Connect(TEXT("ChatSession"),
		TEXT("127.0.0.1"), 7070);

	// ä�ÿ� ������ ����
	FThreadInfo* Info = CNetworkManager::GetInst()->CreateThread<CReceiveThread>(TEXT("ChatReceive"));

	((CReceiveThread*)Info->Action)->SetSession(CNetworkManager::GetInst()->FindSession(TEXT("ChatSession")));

	mQueue = CNetworkManager::GetInst()->FindPacketQueue(TEXT("ChatReceive_Queue"));

	CNetworkManager::GetInst()->SuspendThread(TEXT("ChatSession"), false);

	//mInputText->OnTextCommitted
}

void UChatWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// ä�ü��� ������ �����Ѵ�.
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
