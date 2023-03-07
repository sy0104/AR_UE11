// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp,
		NodeMemory);

	AMonsterAIController* Controller =
		Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	// Blackboard에 저장된 Target을 얻어와야 한다.
	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		return EBTNodeResult::Succeeded;
	}

	if(!Monster->GetPatrolEnable())
		return EBTNodeResult::Succeeded;


	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	Monster->GetCharacterMovement()->MaxWalkSpeed = Info.MoveSpeed * 0.7f;

	Anim->ChangeAnim(EMonsterAnimType::Run);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller,
		Monster->GetPatrolLocation());

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp,
		NodeMemory);

	return result;
}

void UBTTask_Patrol::TickTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller =
		Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
	{
		// Task를 종료시킨다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
	{
		// Task를 종료시킨다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	// Blackboard에 저장된 Target을 얻어와야 한다.
	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		// Task를 종료시킨다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 도착했는지 판단한다.
	FVector	MonsterLoc = Monster->GetActorLocation();

	MonsterLoc = MonsterLoc -
		FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// 두 위치 사이의 거리를 구해준다.
	float	Distance = FVector::Distance(MonsterLoc, Monster->GetPatrolLocation());

	Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	Distance -= 10.f;

	if (Distance <= 0.f)
	{
		// 이동을 멈춘다.
		Controller->StopMovement();

		// 이동 지점을 다음 지점으로 변경시켜둔다.
		Monster->NextPatrolPoint();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Patrol::OnTaskFinished(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
