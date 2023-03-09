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

	// GetAIOwner() : UBehaviorTreeComponent를 가지고 있는 AIController를 얻어올 수 있다.
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
		return false;

	// Blackboard에 저장된 Target을 얻어와야 한다.
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
		return false;

	// 몬스터의 AttackDistance, TraceDistance를 가져와야 함.
	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	// 몬스터의 위치와 타겟의 위치를 이용하여 거리를 구한다.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// 두 위치 사이의 거리를 구해준다.
	float Distance = FVector::Distance(MonsterLoc, TargetLoc);

	// 두 위치 사이의 거리에서 Capsule의 반경을 뺀다.
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
