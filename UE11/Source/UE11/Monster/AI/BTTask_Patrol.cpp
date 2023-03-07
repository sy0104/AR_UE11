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

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (IsValid(Target))
	{
		return EBTNodeResult::Succeeded;
	}

	if (!Monster->GetPatrolEnable())
		return EBTNodeResult::Succeeded;

	const FMonsterInfo& Info = Monster->GetMonsterInfo();
	Monster->GetCharacterMovement()->MaxWalkSpeed = Info.MoveSpeed * 0.7f;	// patrol�� ��� õõ�� �ȵ���
	Anim->ChangeAnim(EMonsterAnimType::Run);

	// ���͸� ���ϴ� ��ġ�� �̵������ش�.
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Monster->GetPatrolLocation());

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);		// Task�� �����Ų��.
		return;
	}

	//  �����ߴ��� �Ǵ��Ѵ�.
	FVector MonsterLoc = Monster->GetActorLocation();
	MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// �� ��ġ ������ �Ÿ��� �����ش�.
	float Distance = FVector::Distance(MonsterLoc, Monster->GetPatrolLocation());

	Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	Distance -= 10.f;

	if (Distance <= 0.f)
	{
		Controller->StopMovement();		// �̵��� �����
		Monster->NextPatrolPoint();		// �̵� ������ ���� �������� ������ѵд�.

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Patrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
