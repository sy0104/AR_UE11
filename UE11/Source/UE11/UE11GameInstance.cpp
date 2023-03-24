// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11GameInstance.h"
#include "Network/NetworkManager.h"

#include "Manager/InventoryMgr.h"

UUE11GameInstance::UUE11GameInstance()
	: m_MonsterTable(nullptr)
	, m_ItemTable(nullptr)
	, m_Inven(nullptr)
	, mSelectJob(EPlayerJob::None)
{
	/*
	DataTable�� �ּ��� ������ �о�ͼ� TMap���� �����Ǵ� �ڷᱸ����
	������ �����صд�.
	TMap�� Key�� Value�� ������ �Ǵµ� Key�� DataTable�� ���̸���
	Ű�� ����Ǹ� Value�� �ش� DataTable�� �����ϴ� ����ü Ÿ����
	������ Ÿ������ ���ü� �ְ� �����ȴ�.
	���� ��� �Ʒ� DataTable�� FMonsterTableInfo ����ü��
	DataTable�� �����Ͽ��� ������ Ž���� ���� ���ϴ� �����͸� ã��
	��� FMonsterTableInfo�� ������ Ÿ������ ���� �� �ְ� �ȴ�.
	Key�� Ž�� �뵵�� ����� �ȴ�.	
	*/
	static ConstructorHelpers::FObjectFinder<UDataTable>	MonsterTable(TEXT("DataTable'/Game/Monster/MonsterTable.MonsterTable'"));
	if (MonsterTable.Succeeded())
		m_MonsterTable = MonsterTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	ItemTable(TEXT("DataTable'/Game/Blueprints/JeeHun/ItemDataTable.ItemDataTable'"));
	if (ItemTable.Succeeded())
	{
		m_ItemTable = ItemTable.Object;
		UInventoryMgr::GetInst(this)->SetItemTable(m_ItemTable);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>	SkillfInfoTable(TEXT("DataTable'/Game/Skill/DTMonsterSkill.DTMonsterSkill'"));
	if (SkillfInfoTable.Succeeded())
	{
		m_MonsterSkillInfoTable = SkillfInfoTable.Object;
	}
}

UUE11GameInstance::~UUE11GameInstance()
{
	CNetworkManager::DestroyInst();
}

void UUE11GameInstance::Init()
{
	Super::Init();

	CNetworkManager::GetInst()->Init();
}

const FMonsterTableInfo* UUE11GameInstance::FindMonsterTable(
	const FName& Name)
{
	// 2��° ������ ContextString�� �ش� Ű�� �̿��Ͽ� Ž����
	// ������ ��� ����� �޽����� FString Ÿ������ �����Ѵ�.
	return m_MonsterTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}

const FSkillData* UUE11GameInstance::FindMonsterSkillTable(const FName& Name)
{
	return m_MonsterSkillInfoTable->FindRow<FSkillData>(Name, TEXT(""));
}
