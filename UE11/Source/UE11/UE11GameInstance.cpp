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
	// 2번째 인자인 ContextString은 해당 키를 이용하여 탐색이
	// 실패할 경우 출력할 메시지를 FString 타입으로 전달한다.
	return m_MonsterTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}

const FSkillData* UUE11GameInstance::FindMonsterSkillTable(const FName& Name)
{
	return mMonsterSkillInfoTable->FindRow<FSkillData>(Name, TEXT(""));
}
