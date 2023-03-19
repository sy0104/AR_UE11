// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11GameInstance.h"
#include "Manager/InventoryManager.h"

UUE11GameInstance::UUE11GameInstance()
	: m_MonsterTable(nullptr)
	, mItemTable(nullptr)
	, mInven(nullptr)

{
	static ConstructorHelpers::FObjectFinder<UDataTable>	MonsterTable(TEXT("DataTable'/Game/Monster/MonsterTable.MonsterTable'"));

	if (MonsterTable.Succeeded())
		m_MonsterTable = MonsterTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	ItemTable(TEXT("DataTable'/Game/Blueprints/UIClass/ItemDataTable.ItemDataTable'"));
	if (ItemTable.Succeeded())
	{
		mItemTable = ItemTable.Object;
		UInventoryManager::GetInstance(this)->SetItemTable(mItemTable);
	}
}

UUE11GameInstance::~UUE11GameInstance()
{
}

void UUE11GameInstance::Init()
{
	Super::Init();
}

const FMonsterTableInfo* UUE11GameInstance::FindMonsterTable(const FName& Name)
{
	// 2��° ������ ContextString�� �ش� Ű�� �̿��Ͽ� Ž����
	// ������ ��� ����� �޽����� FString Ÿ������ �����Ѵ�.
	return m_MonsterTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}

const FSkillData* UUE11GameInstance::FindMonsterSkillTable(const FName& Name)
{
	return mMonsterSkillInfoTable->FindRow<FSkillData>(Name, TEXT(""));
}
