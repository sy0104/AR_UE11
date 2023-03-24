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
	// InventoryMgr 을 요청할때 현재 월드를 기억
	m_CurWorld = _World;

	UUE11GameInstance * GameInst = Cast<UUE11GameInstance>(UGameplayStatics::GetGameInstance(_World));

	// 인벤토리를 처음으로 요청한 순간
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

	// 인벤토리를 처음으로 요청한 순간
	if (nullptr == GameInst->m_Inven)
	{
		GameInst->m_Inven = NewObject<UInventoryMgr>();
		GameInst->m_Inven->AddToRoot();
	}

	return GameInst->m_Inven; 
}


bool UInventoryMgr::IsInventoryOpen()
{	
	// 현재 게임모드를 알아낸다. (입력으로 현재 월드가 요구된다, 현재 월드의 게임모드를 알아내야 하기 때문)
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(m_CurWorld));

	// 알아낸 게임모드가 AUE11GameModeBase 가 아니라면 캐스팅 실패 == 현재 월드가 메인 레벨이 아니라는 뜻
	if(nullptr == GameMode)
		return false;

	// 현재 게임모드가 AUE11GameModeBase 가 맞다면, MainHUD 에 접근해서 InventoryWiget 의 Visible 여부를 확인한다.
	UMainHUDBase*	MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InveotyrWidget = MainHUD->GetInventoryWidget();

	return InveotyrWidget->IsVisible();
}



void UInventoryMgr::ShowInventory(bool _Show)
{
	// 이전에 호출한 뒤에 일정 시간이 지났는지 체크
	int32 CurTime = 0;
	float CurPartial = 0.f;
	UGameplayStatics::GetAccurateRealTime(CurTime, CurPartial);

	// 시간차이가 1초를 넘어서지 못하면 반환
	if (CurTime - m_PrevTime < 1)
		return;

	m_PrevTime = CurTime;



	// 현재 게임모드를 알아낸다. (입력으로 현재 월드가 요구된다, 현재 월드의 게임모드를 알아내야 하기 때문)
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(m_CurWorld));

	// 알아낸 게임모드가 AUE11GameModeBase 가 아니라면 캐스팅 실패 == 현재 월드가 메인 레벨이 아니라는 뜻
	if (nullptr == GameMode)
		return;


	// 현재 게임모드가 AUE11GameModeBase 가 맞다면, MainHUD 에 접근해서 InventoryWiget 의 Visible 여부를 확인한다.
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InveotyrWidget = MainHUD->GetInventoryWidget();

	// 인벤토리 온
	if (_Show)
	{
		InveotyrWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = m_CurWorld->GetFirstPlayerController();
				 
		FInputModeUIOnly uionly;
		Controller->SetInputMode(uionly);
		Controller->bShowMouseCursor = true;
	}
	
	// 인벤토리 오프
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

	// 데이터 테이블에 들어있는 모든 구조체 정보
	FString str;
	TArray<FItemDataInfo*> AllItemInfo;
	m_ItemTable->GetAllRows<FItemDataInfo>(str, AllItemInfo);
	
	// 데이터 테이블에 들어있는 모든 행 이름
	TArray<FName> AllRowname;
	AllRowname = m_ItemTable->GetRowNames();


	// 테이블정보를 다 받아와서 전체 반복문을 돈다.
	// ItemID 를 키값으로, 해당 아이템의 RowName 을 데이터로 연결한다.
	for (int i = 0; i < AllItemInfo.Num(); ++i)
	{
		m_RownameFromID.Add(AllItemInfo[i]->ID, AllRowname[i]);
	}
}

const FItemDataInfo* UInventoryMgr::GetItemInfo(EITEM_ID _ItemID)
{
	// ItemID 에 연결해둔 RowName 을 찾아서
	FName RowName = m_RownameFromID.FindRef(_ItemID);

	// 데이터 테이블에서 검색을 한다.
	FItemDataInfo* ItemInfo = m_ItemTable->FindRow<FItemDataInfo>(RowName, nullptr);

	return ItemInfo;
}