// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11GameInstance.h"

UUE11GameInstance::UUE11GameInstance()
{
	// DataTable은 에셋의 내용을 읽어와서 TMap으로 구성되는 자료구조에 내용을 저장해둔다.
	// TMap은 Key와 Value로 구성되는데 Key는 DataTable의 행이름이 키로 저장되며
	// Value는 해당 DataTable을 구성하는 구조체 타입의 포인터 타입으로 얻어올 수 있게 구성된다.
	// 예를 들어 아래 DataTable은 FMonsterTableInfo 구조체로 DataTable을 구성하였기 때문에
	// 탐색을 통해 원하는 데이터를 찾을 경우 FMonsterTableInfo의 포인터 타입으로 얻어올 수 있게 된다.
	// Key는 탐색 용도로 사용된다.
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
	// 2번째 인자인 ContextString은 해당 키를 이용하여 탐색이 실패할 경우 출력할 메시지를 FString 타입으로 전달한다.
	return mMonsterTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}
