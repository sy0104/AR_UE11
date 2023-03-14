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
	// InventoryManager �� ��û�� �� ���� ���带 ���
	// ���� ������ ���� �ν��Ͻ��� �����;� �ϴµ� InventoryManager�� ���� ���带 ��

	// InventoryManager�� Ư�� ������ ������ �ִ� ���� �ƴ϶� NewObject�� �츮�� ���� ���̴�.
	// ���� �ν��Ͻ��� �ּҰ��� �����ϰ� �ִ�.
	// �÷��̾� ĳ���ʹ� ���� ������ �����ϰ� �ְ�, GetWorld()�� ȣ�����ָ� �ڽ��� �����ִ� ���带 �������ش�.
	// �κ��丮 �ȿ��� GetWorld()�� ���� �κ��丮 �Ŵ����� ��𿡵� �������� �ʱ� ������(� ����, ���� �ȿ� �������� ����) nullptr�� ���´�.
	// -> ���� ���带 �˱� ���ؼ� GetInstance() �� ���ڷ� ���� ���带 �޾ƿ´�.

	mCurWorld = World;

	UUE11GameInstance* GameInst = Cast<UUE11GameInstance>(UGameplayStatics::GetGameInstance(World));

	// �κ��丮�� ó������ ��û�� ����
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

	// �κ��丮�� ó������ ��û�� ����
	if (!GameInst->mInven)
	{
		GameInst->mInven = NewObject<UInventoryManager>();
		GameInst->mInven->AddToRoot();
	}

	return GameInst->mInven;
}

void UInventoryManager::ShowInventory(bool Show)
{
	// ShowInventory() �� ��� ����Ǵ� ���� �����ϱ� ���� �ð� ���� �ɾ�д� - 1��
	// ������ ȣ���� �ڿ� ���� �ð��� �������� üũ
	int32 CurTime = 0;
	float CurPartial = 0.f;
	UGameplayStatics::GetAccurateRealTime(CurTime, CurPartial);

	// �ð����̰� 1�ʸ� �Ѿ�� ���ϸ� ��ȯ
	if (CurTime - mPrevTime < 1)
		return;

	mPrevTime = CurTime;

	// ���� ���Ӹ�带 �˾Ƴ���. ���� ���Ӹ�尡 AUE11GameModeBase�� �ƴ϶�� ���� ����� ���� ������ �ƴϴ� -> return
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));

	if (!GameMode)
		return;

	// ���� ���Ӹ�尡 AUE11GameModeBase�� �´ٸ�, MainHUD�� �����ؼ� InventoryWidget�� �����´�.
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InventoryWidget = MainHUD->GetInventoryWidget();

	// �κ��丮 On
	if (Show)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = mCurWorld->GetFirstPlayerController();

		FInputModeUIOnly uionly;
		Controller->SetInputMode(uionly);
		Controller->bShowMouseCursor = true;
	}

	// �κ��丮 Off
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		APlayerController* Controller = mCurWorld->GetFirstPlayerController();

		FInputModeGameOnly GameOnly;
		Controller->SetInputMode(GameOnly);
		Controller->bShowMouseCursor = false;
	}
}

// �� �Լ��� ���� ���������� �����ؾ� �Ѵ�. ���� ������ HUD�� �����ϰ� �ֱ� �����̴�.
// �� �Լ��� ȣ��Ǵ� ������ ���Ӹ�带 ��Ȯ�� üũ�غ��� �Ѵ� -> UE11GameModeBase ���� �Ѵ�.
bool UInventoryManager::IsInventoryOpen()
{
	// ���� ���Ӹ�带 �˾Ƴ���. (�Է����� ���� ���尡 �䱸��, ���� ������ ���Ӹ�带 �˾Ƴ��� �ϱ� ����)
	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));

	// �˾Ƴ� ���Ӹ�尡 AUE11GameModeBase �� �ƴ϶�� ĳ���� ���� == ���� ���尡 ���� ������ �ƴ϶�� ��
	if (!GameMode)
		return false;

	// ���� ���Ӹ�尡 AUE11GameModeBase�� �´ٸ�, MainHUD�� �����ؼ� InventoryWidget�� Visible ���θ� Ȯ���Ѵ�.
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InventoryWidget = MainHUD->GetInventoryWidget();

	return InventoryWidget->IsVisible();
}

const FItemDataInfo* UInventoryManager::GetItemInfo(EItemID ItemID)
{
	// ItemID �� �����ص� RowName �� ã�Ƽ�
	FName RowName = mRownameFromID.FindRef(ItemID);

	// ������ ���̺��� �˻��Ѵ�.
	FItemDataInfo* ItemInfo = mItemTable->FindRow<FItemDataInfo>(RowName, nullptr);

	return ItemInfo;
}

void UInventoryManager::SetItemTable(UDataTable* Table)
{
	mItemTable = Table;

	// ������ ���̺� ����ִ� ��� ����ü ����
	FString str;
	TArray<FItemDataInfo*> AllItemInfo;
	mItemTable->GetAllRows<FItemDataInfo>(str, AllItemInfo);

	// ������ ���̺� ����ִ� ��� �� �̸�
	TArray<FName> AllRowName;
	AllRowName = mItemTable->GetRowNames();

	// ���̺� ������ �� �޾ƿͼ� ��ü �ݺ����� ����.
	// ItemID�� Ű������, �ش� �������� RowName�� �����ͷ� �����Ѵ�.
	for (int i = 0; i < AllItemInfo.Num(); ++i)
		mRownameFromID.Add(AllItemInfo[i]->ID, AllRowName[i]);
}
