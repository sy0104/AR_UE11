// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11GameInstance.h"

UUE11GameInstance::UUE11GameInstance()
{
	// DataTable�� ������ ������ �о�ͼ� TMap���� �����Ǵ� �ڷᱸ���� ������ �����صд�.
	// TMap�� Key�� Value�� �����Ǵµ� Key�� DataTable�� ���̸��� Ű�� ����Ǹ�
	// Value�� �ش� DataTable�� �����ϴ� ����ü Ÿ���� ������ Ÿ������ ���� �� �ְ� �����ȴ�.
	// ���� ��� �Ʒ� DataTable�� FMonsterTableInfo ����ü�� DataTable�� �����Ͽ��� ������
	// Ž���� ���� ���ϴ� �����͸� ã�� ��� FMonsterTableInfo�� ������ Ÿ������ ���� �� �ְ� �ȴ�.
	// Key�� Ž�� �뵵�� ���ȴ�.
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterTable(TEXT("DataTable'/Game/Monster/MonsterTable.MonsterTable'"));

	if (MonsterTable.Succeeded())
		mMonsterTable = MonsterTable.Object;
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
	// 2��° ������ ContextString�� �ش� Ű�� �̿��Ͽ� Ž���� ������ ��� ����� �޽����� FString Ÿ������ �����Ѵ�.
	return mMonsterTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}
