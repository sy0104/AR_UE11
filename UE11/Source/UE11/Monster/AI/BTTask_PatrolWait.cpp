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

	// ���Ͱ� Ÿ���� ������ ������ �� �½�ũ�� ���������� ���ϰ� �Ѵ�.
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task�� �����Ų��.
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	if (!IsValid(Monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task�� �����Ų��.
		return;
	}

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	// Patrol�� ���� Ÿ���� ���� ���� �����̴�.
	// Patrol�� ���� �� ��ü�� Ÿ���� ���� �� ���� ���̹Ƿ� IsValid ���ǹ��� �ݴ�� ����� �Ѵ�.
	if (IsValid(Target))
	{
		// Task�� �����Ų��.
		// �� Task�� �����Ű�� �������� �ٲ���� �Ѵ�. (Ÿ���� �ֱ� ����)
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
