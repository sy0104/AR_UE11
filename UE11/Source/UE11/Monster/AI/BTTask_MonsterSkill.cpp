// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterSkill.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

UBTTask_MonsterSkill::UBTTask_MonsterSkill()
{
	NodeName = TEXT("MonsterSkill");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MonsterSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (!IsValid(Target) || !Controller->GetBlackboardComponent()->GetValueAsBool(TEXT("SkillEnable")))
	{
		Controller->StopMovement();	
		Anim->ChangeAnim(EMonsterAnimType::Idle);
		return EBTNodeResult::Failed;
	}

	const FMonsterSkillInfo* Info = Monster->GetSkillData();
	Anim->ChangeAnim(Info->AnimType);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_MonsterSkill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_MonsterSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(Controller) || !Controller->GetBlackboardComponent()->GetValueAsBool(TEXT("SkillEnable")))
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

	if (!IsValid(Target))
	{
		Controller->StopMovement();								// �̵��� �����.
		Anim->ChangeAnim(EMonsterAnimType::Idle);				// �ִϸ��̼��� Idle�� ��ȯ�Ѵ�.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task�� �����Ų��.

		return;
	}

	// ������ �������� �Ǵ��Ѵ�.
	if (Monster->GetAttackEnd())
	{
		// Ÿ�ٰ� ���Ϳ��� �Ÿ��� üũ�Ѵ�.
		const FMonsterInfo& Info = Monster->GetMonsterInfo();

		// ���� ��ġ�� Ÿ���� ��ġ�� �̿��Ͽ� �Ÿ��� ���Ѵ�.
		// ������ �߹� ��ġ�� �������� �Ѵ�.
		FVector	MonsterLoc = Monster->GetActorLocation();
		FVector TargetLoc = Target->GetActorLocation();

		MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		// �� ��ġ ������ �Ÿ��� �����ش�.
		float Distance = FVector::Distance(MonsterLoc, Monster->GetPatrolPointLocation());

		Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
		Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (Distance > Info.AttackDistance)
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		// ������ ���ݰŸ� �ȿ� ������ ��� ������ Ÿ�������� �����Ѵ�.
		else
		{
			// Ÿ���� �ٶ� ������ ���Ѵ�.
			FVector Dir = TargetLoc - MonsterLoc;
			Dir.Z = 0.f;
			Dir.Normalize();

			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}

		Monster->SetAttackEnd(false);
	}
}

void UBTTask_MonsterSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
