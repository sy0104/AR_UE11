// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"

#include "../UE11GameInstance.h"
#include "../UE11GameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UMG/InventoryBase.h"

UWorld* UInventoryManager::mCurWorld = nullptr;

UInventoryManager::UInventoryManager()
	: mPrevTime(0)
{
}

UInventoryManager::~UInventoryManager()
{
}

UInventoryManager* UInventoryManager::GetInstance(UWorld* World)
{
	// InventoryManager 을 요청할 때 현재 월드를 기억
	// 현재 월드의 게임 인스턴스를 가져와야 하는데 InventoryManager는 현재 월드를 모름

	// InventoryManager는 특정 레벨에 속해져 있는 것이 아니라 NewObject로 우리가 만든 것이다.
	// 게임 인스턴스가 주소값을 관리하고 있다.
	// 플레이어 캐릭터는 실제 레벨에 존재하고 있고, GetWorld()를 호출해주면 자신이 속해있는 월드를 리턴해준다.
	// 인벤토리 안에서 GetWorld()를 쓰면 인벤토리 매니저는 어디에도 속해있지 않기 때문에(어떤 레벨, 월드 안에 속해있지 않음) nullptr이 나온다.
	// -> 현재 월드를 알기 위해서 GetInstance() 의 인자로 현재 월드를 받아온다.

	mCurWorld = World;

	UUE11GameInstance* GameInst = Cast<UUE11GameInstance>(UGameplayStatics::GetGameInstance(World));

	// 인벤토리를 처음으로 요청한 순간
	if (!GameInst->mInven)
	{
		GameInst->mInven = NewObject<UInventoryManager>();
		GameInst->mInven->AddToRoot();
	}

	return GameInst->mInven;
}

UInventoryManager* UInventoryManager::GetInstance(UGameInstance* GameInstance)
{
	UUE11GameInstance* GameInst = Cast<UUE11GameInstance>(GameInstance);

	// 인벤토리를 처음으로 요청한 순간
	if (!GameInst->mInven)
	{
		GameInst->mInven = NewObject<UInventoryManager>();
		GameInst->mInven->AddToRoot();
	}

	return GameInst->mInven;
}

void UInventoryManager::ShowInventory(bool Show)
{
	// ShowInventory() 가 계속 실행되는 것을 방지하기 위해 시간 락을 걸어둔다 - 1초
	// 이전에 호출한 뒤에 일정 시간이 지났는지 체크
	int32 CurTime = 0;
	float CurPartial = 0.f;
	UGameplayStatics::GetAccurateRealTime(CurTime, CurPartial);

	// 시간차이가 1초를 넘어서지 못하면 반환
	if (CurTime - mPrevTime < 1)
		return;

	mPrevTime = CurTime;

	// 현재 게임모드를 알아낸다. 현재 게임모드가 AUE11GameModeBase가 아니라면 현재 월드는 메인 레벨이 아니다 -> return
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));

	if (!GameMode)
		return;

	// 현재 게임모드가 AUE11GameModeBase가 맞다면, MainHUD에 접근해서 InventoryWidget을 가져온다.
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InventoryWidget = MainHUD->GetInventoryWidget();

	// 인벤토리 On
	if (Show)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = mCurWorld->GetFirstPlayerController();

		FInputModeUIOnly uionly;
		Controller->SetInputMode(uionly);
		Controller->bShowMouseCursor = true;
	}

	// 인벤토리 Off
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		APlayerController* Controller = mCurWorld->GetFirstPlayerController();

		FInputModeGameOnly GameOnly;
		Controller->SetInputMode(GameOnly);
		Controller->bShowMouseCursor = false;
	}
}

// 이 함수는 메인 레벨에서만 동작해야 한다. 메인 레벨만 HUD를 소유하고 있기 때문이다.
// 이 함수가 호출되는 시점의 게임모드를 정확히 체크해봐야 한다 -> UE11GameModeBase 여야 한다.
bool UInventoryManager::IsInventoryOpen()
{
	// 현재 게임모드를 알아낸다. (입력으로 현재 월드가 요구됨, 현재 월드의 게임모드를 알아내야 하기 때문)
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));

	// 알아낸 게임모드가 AUE11GameModeBase 가 아니라면 캐스팅 실패 == 현재 월드가 메인 레벨이 아니라는 뜻
	if (!GameMode)
		return false;

	// 현재 게임모드가 AUE11GameModeBase가 맞다면, MainHUD에 접근해서 InventoryWidget의 Visible 여부를 확인한다.
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InventoryWidget = MainHUD->GetInventoryWidget();

	return InventoryWidget->IsVisible();
}

const FItemDataInfo* UInventoryManager::GetItemInfo(EItemID ItemID)
{
	// ItemID 에 연결해둔 RowName 을 찾아서
	FName RowName = mRownameFromID.FindRef(ItemID);

	// 데이터 테이블에서 검색한다.
	FItemDataInfo* ItemInfo = mItemTable->FindRow<FItemDataInfo>(RowName, nullptr);

	return ItemInfo;
}

void UInventoryManager::SetItemTable(UDataTable* Table)
{
	mItemTable = Table;

	// 데이터 테이블에 들어있는 모든 구조체 정보
	FString str;
	TArray<FItemDataInfo*> AllItemInfo;
	mItemTable->GetAllRows<FItemDataInfo>(str, AllItemInfo);

	// 데이터 테이블에 들어있는 모든 행 이름
	TArray<FName> AllRowName;
	AllRowName = mItemTable->GetRowNames();

	// 테이블 정보를 다 받아와서 전체 반복문을 돈다.
	// ItemID를 키값으로, 해당 아이템의 RowName을 데이터로 연결한다.
	for (int i = 0; i < AllItemInfo.Num(); ++i)
		mRownameFromID.Add(AllItemInfo[i]->ID, AllRowName[i]);
}
