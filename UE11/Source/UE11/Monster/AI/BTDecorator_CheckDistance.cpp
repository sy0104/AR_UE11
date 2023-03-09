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

	// GetAIOwner() : UBehaviorTreeComponent�� ������ �ִ� AIController�� ���� �� �ִ�.
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
		return false;

	// Blackboard�� ����� Target�� ���;� �Ѵ�.
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
		return false;

	// ������ AttackDistance, TraceDistance�� �����;� ��.
	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	// ������ ��ġ�� Ÿ���� ��ġ�� �̿��Ͽ� �Ÿ��� ���Ѵ�.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// �� ��ġ ������ �Ÿ��� �����ش�.
	float Distance = FVector::Distance(MonsterLoc, TargetLoc);

	// �� ��ġ ������ �Ÿ����� Capsule�� �ݰ��� ����.
	Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

	float Check = 0.f;

	switch (mCheckType)
	{
	case ECheckDistanceType::Attack:
		Check = Info.AttackDistance;
		break;
	case ECheckDistanceType::Trace:
		Check = Info.TraceDistance;
		break;	
	}

	return Distance <= Check;
}
