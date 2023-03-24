// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetTrace.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

UBTTask_TargetTrace::UBTTask_TargetTrace()
{
	NodeName = TEXT("TargetTrace");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TargetTrace::ExecuteTask(
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

	// Blackboard�� ����� Target�� ���;� �Ѵ�.
	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
	{
		// �̵��� �����.
		Controller->StopMovement();

		// �ִϸ��̼��� Idle�� ��ȯ�Ѵ�.
		Anim->ChangeAnim(EMonsterAnimType::Idle);

		return EBTNodeResult::Failed;
	}

	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	Monster->GetCharacterMovement()->MaxWalkSpeed = Info.MoveSpeed;

	// Ÿ�� ��ġ�� �̵���Ų��.
	UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller,
		Target);

	// 2�� ȣ��ȴ�.
	//Controller->MoveToActor();

	Anim->ChangeAnim(EMonsterAnimType::CombatRun);

	// ���Ͱ� Ÿ�ٿ� �����Ҷ����� �� �½�ũ�� ���������� ���ϰ��Ѵ�.
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_TargetTrace::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp,
		NodeMemory);

	return result;
}

void UBTTask_TargetTrace::TickTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller =
		Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
	{
		// Task�� �����Ų��.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	else if (Controller->GetBlackboardComponent()->GetValueAsBool(TEXT("SkillEnable")))
	{
		// �̵��� �����.
		Controller->StopMovement();

		// Task�� �����Ų��.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
	{
		// Task�� �����Ų��.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	// Blackboard�� ����� Target�� ���;� �Ѵ�.
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
	{
		// �̵��� �����.
		Controller->StopMovement();

		// �ִϸ��̼��� Idle�� ��ȯ�Ѵ�.
		Anim->ChangeAnim(EMonsterAnimType::Idle);

		// Task�� �����Ų��.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// �̵��� Ÿ�ٰ��� �Ÿ��� üũ�Ͽ� �Ÿ��� ���ݰŸ� �ȿ� ���
	// ���� ��� �������� ��ȯ�Ѵ�.
	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	// ������ ��ġ�� Ÿ���� ��ġ�� �̿��Ͽ� �Ÿ��� ���Ѵ�.
	// ������ �߹� ��ġ�� �������� �Ѵ�.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc = MonsterLoc - 
		FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TargetLoc = TargetLoc -
		FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// �� ��ġ ������ �Ÿ��� �����ش�.
	float	Distance = FVector::Distance(MonsterLoc, TargetLoc);

	// �� ��ġ ������ �Ÿ����� Capsule�� �ݰ��� ����.
	Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (Distance <= Info.AttackDistance)
	{
		// �̵��� �����.
		Controller->StopMovement();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_TargetTrace::OnTaskFinished(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, 
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
