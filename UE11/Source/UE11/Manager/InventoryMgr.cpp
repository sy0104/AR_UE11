// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMgr.h"

#include "../UE11GameInstance.h"
#include "../UE11GameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UMG/InventoryBase.h"

UWorld* UInventoryMgr::m_CurWorld = nullptr;


UInventoryMgr::UInventoryMgr()
	: m_PrevTime(0)
{
	
}

UInventoryMgr::~UInventoryMgr()
{

}

UInventoryMgr* UInventoryMgr::GetInst(UWorld* _World)
{
	// InventoryMgr �� ��û�Ҷ� ���� ���带 ���
	m_CurWorld = _World;

	UUE11GameInstance * GameInst = Cast<UUE11GameInstance>(UGameplayStatics::GetGameInstance(_World));

	// �κ��丮�� ó������ ��û�� ����
	if (nullptr == GameInst->m_Inven)
	{
		GameInst->m_Inven = NewObject<UInventoryMgr>();
		GameInst->m_Inven->AddToRoot();		
	}	

	return GameInst->m_Inven;
}

UInventoryMgr* UInventoryMgr::GetInst(UGameInstance* _GameInst)
{
	UUE11GameInstance* GameInst = Cast<UUE11GameInstance>(_GameInst);

	// �κ��丮�� ó������ ��û�� ����
	if (nullptr == GameInst->m_Inven)
	{
		GameInst->m_Inven = NewObject<UInventoryMgr>();
		GameInst->m_Inven->AddToRoot();
	}

	return GameInst->m_Inven; 
}


bool UInventoryMgr::IsInventoryOpen()
{	
	// ���� ���Ӹ�带 �˾Ƴ���. (�Է����� ���� ���尡 �䱸�ȴ�, ���� ������ ���Ӹ�带 �˾Ƴ��� �ϱ� ����)
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(m_CurWorld));

	// �˾Ƴ� ���Ӹ�尡 AUE11GameModeBase �� �ƴ϶�� ĳ���� ���� == ���� ���尡 ���� ������ �ƴ϶�� ��
	if(nullptr == GameMode)
		return false;

	// ���� ���Ӹ�尡 AUE11GameModeBase �� �´ٸ�, MainHUD �� �����ؼ� InventoryWiget �� Visible ���θ� Ȯ���Ѵ�.
	UMainHUDBase*	MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InveotyrWidget = MainHUD->GetInventoryWidget();

	return InveotyrWidget->IsVisible();
}



void UInventoryMgr::ShowInventory(bool _Show)
{
	// ������ ȣ���� �ڿ� ���� �ð��� �������� üũ
	int32 CurTime = 0;
	float CurPartial = 0.f;
	UGameplayStatics::GetAccurateRealTime(CurTime, CurPartial);

	// �ð����̰� 1�ʸ� �Ѿ�� ���ϸ� ��ȯ
	if (CurTime - m_PrevTime < 1)
		return;

	m_PrevTime = CurTime;



	// ���� ���Ӹ�带 �˾Ƴ���. (�Է����� ���� ���尡 �䱸�ȴ�, ���� ������ ���Ӹ�带 �˾Ƴ��� �ϱ� ����)
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(m_CurWorld));

	// �˾Ƴ� ���Ӹ�尡 AUE11GameModeBase �� �ƴ϶�� ĳ���� ���� == ���� ���尡 ���� ������ �ƴ϶�� ��
	if (nullptr == GameMode)
		return;


	// ���� ���Ӹ�尡 AUE11GameModeBase �� �´ٸ�, MainHUD �� �����ؼ� InventoryWiget �� Visible ���θ� Ȯ���Ѵ�.
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InveotyrWidget = MainHUD->GetInventoryWidget();

	// �κ��丮 ��
	if (_Show)
	{
		InveotyrWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = m_CurWorld->GetFirstPlayerController();
				 
		FInputModeUIOnly uionly;
		Controller->SetInputMode(uionly);
		Controller->bShowMouseCursor = true;
	}
	
	// �κ��丮 ����
	else
	{
		InveotyrWidget->SetVisibility(ESlateVisibility::Hidden);

		APlayerController* Controller = m_CurWorld->GetFirstPlayerController();

		FInputModeGameOnly gameonly;
		Controller->SetInputMode(gameonly);
		Controller->bShowMouseCursor = false;
	}
	
}

void UInventoryMgr::SetItemTable(UDataTable* _Table)
{
	m_ItemTable = _Table;

	// ������ ���̺� ����ִ� ��� ����ü ����
	FString str;
	TArray<FItemDataInfo*> AllItemInfo;
	m_ItemTable->GetAllRows<FItemDataInfo>(str, AllItemInfo);
	
	// ������ ���̺� ����ִ� ��� �� �̸�
	TArray<FName> AllRowname;
	AllRowname = m_ItemTable->GetRowNames();


	// ���̺������� �� �޾ƿͼ� ��ü �ݺ����� ����.
	// ItemID �� Ű������, �ش� �������� RowName �� �����ͷ� �����Ѵ�.
	for (int i = 0; i < AllItemInfo.Num(); ++i)
	{
		m_RownameFromID.Add(AllItemInfo[i]->ID, AllRowname[i]);
	}
}

const FItemDataInfo* UInventoryMgr::GetItemInfo(EITEM_ID _ItemID)
{
	// ItemID �� �����ص� RowName �� ã�Ƽ�
	FName RowName = m_RownameFromID.FindRef(_ItemID);

	// ������ ���̺��� �˻��� �Ѵ�.
	FItemDataInfo* ItemInfo = m_ItemTable->FindRow<FItemDataInfo>(RowName, nullptr);

	return ItemInfo;
}