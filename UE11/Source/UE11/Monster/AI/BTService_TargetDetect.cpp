// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TargetDetect.h"
#include "../MonsterAIController.h"
#include "../Monster.h"

UBTService_TargetDetect::UBTService_TargetDetect()
{
	NodeName = TEXT("TargetDetect");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_TargetDetect::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// GetAIOwner() : UBehaviorTreeComponent를 가지고 있는
	// AIController를 얻어올 수 있다.
	AMonsterAIController* Controller =
		Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
		return;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
		return;

	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	FCollisionQueryParams	param(NAME_None, false, Monster);

	TArray<FOverlapResult>	ResultArray;

	bool CollisionEnable = GetWorld()->OverlapMultiByChannel(ResultArray,
		Monster->GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(Info.TraceDistance),
		param);

#if ENABLE_DRAW_DEBUG

	// CollisionEnable 가 true이면 Red, false이면 Green을 저장한다.
	FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	// FRotationMatrix::MakeFromZ(GetActorForwardVector()) : 앞쪽을
	// 바라보는 회전행렬을 만들어서 .ToQuat() 함수를 이용하여 회전행렬을
	// 회전값으로 변환해준다.
	DrawDebugSphere(GetWorld(), Monster->GetActorLocation(),
		Info.TraceDistance, 20,
		DrawColor, false, 0.3f);

#endif

	if (CollisionEnable)
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), ResultArray[0].GetActor());

	else
	{
		Monster->ClearSkill();
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
}
