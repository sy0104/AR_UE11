// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"
#include "../MonsterAIController.h"
#include "../Monster.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("this : %d"), (int64)this));

	// GetAIOwner(): UBehaviorTreeComponent�� ������ �ִ� AIController�� ���� �� �ִ�.
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller))
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	if (!IsValid(Monster))
		return false;

	// Blackboard�� ����� Target�� ���;� �Ѵ�.
	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!IsValid(Target))
		return false;

	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	return false;
}
