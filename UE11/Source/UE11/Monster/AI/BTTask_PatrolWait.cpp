// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolWait.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

UBTTask_PatrolWait::UBTTask_PatrolWait()
{
	NodeName = TEXT("PatrolWait");
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	mWaitTime = 2.f;
}

EBTNodeResult::Type UBTTask_PatrolWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller))
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
		return EBTNodeResult::Succeeded;

	Anim->ChangeAnim(EMonsterAnimType::Idle);
	Controller->StopMovement();

	// 몬스터가 타겟이 도착할 때까지 이 태스크를 빠져나가지 못하게 한다.
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PatrolWait::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_PatrolWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task를 종료시킨다.
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	if (!IsValid(Monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task를 종료시킨다.
		return;
	}

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	// Patrol일 때는 타겟이 있을 때가 문제이다.
	// Patrol로 들어온 것 자체가 타겟이 없을 때 들어온 것이므로 IsValid 조건문을 반대로 해줘야 한다.
	if (IsValid(Target))
	{
		// Task를 종료시킨다.
		// 이 Task를 종료시키고 전투모드로 바꿔줘야 한다. (타겟이 있기 때문)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		
		return;
	}

	Monster->AddPatrolWaitTime(DeltaSeconds);

	if (Monster->GetPatrolWaitTime() >= mWaitTime)
	{
		Monster->ClearPatrolWaitTime();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

}

void UBTTask_PatrolWait::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
