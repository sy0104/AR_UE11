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

	if (!IsValid(Target))
	{
		Controller->StopMovement();								// 이동을 멈춘다.
		Anim->ChangeAnim(EMonsterAnimType::Idle);				// 애니메이션을 Idle로 전환한다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task를 종료시킨다.

		return;
	}

	// 공격이 끝났는지 판단한다.
	if (Monster->GetAttackEnd())
	{
		// 타겟과 몬스터와의 거리를 체크한다.
		const FMonsterInfo& Info = Monster->GetMonsterInfo();

		// 몬스터 위치와 타겟의 위치를 이용하여 거리를 구한다.
		// 기준은 발밑 위치를 기준으로 한다.
		FVector	MonsterLoc = Monster->GetActorLocation();
		FVector TargetLoc = Target->GetActorLocation();

		MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		// 두 위치 사이의 거리를 구해준다.
		float Distance = FVector::Distance(MonsterLoc, Monster->GetPatrolPointLocation());

		Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
		Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (Distance > Info.AttackDistance)
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		// 여전히 공격거리 안에 존재할 경우 방향을 타겟쪽으로 변경한다.
		else
		{
			// 타겟을 바라볼 방향을 구한다.
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
