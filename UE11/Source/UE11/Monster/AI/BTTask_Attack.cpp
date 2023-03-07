// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
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

	if (!IsValid(Target))
	{
		// 이동을 멈춘다.
		Controller->StopMovement();

		// 애니메이션을 Idle로 전환한다.
		Anim->ChangeAnim(EMonsterAnimType::Idle);

		return EBTNodeResult::Failed;
	}

	Anim->ChangeAnim(EMonsterAnimType::Attack);

	// 몬스터가 타겟에 도착할때까지 이 태스크를 빠져나가지 못하게한다.
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp,
		NodeMemory);

	return result;
}

void UBTTask_Attack::TickTask(
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
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
	{
		// 이동을 멈춘다.
		Controller->StopMovement();

		// 애니메이션을 Idle로 전환한다.
		Anim->ChangeAnim(EMonsterAnimType::Idle);

		// Task를 종료시킨다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 공격이 끝났는지 판단한다.
	if (Monster->GetAttackEnd())
	{
		// 타겟과 몬스터와의 거리를 체크한다.
		const FMonsterInfo& Info = Monster->GetMonsterInfo();

		// 몬스터의 위치와 타겟의 위치를 이용하여 거리를 구한다.
		// 기준은 발밑 위치를 기준으로 한다.
		FVector	MonsterLoc = Monster->GetActorLocation();
		FVector	TargetLoc = Target->GetActorLocation();

		MonsterLoc = MonsterLoc -
			FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TargetLoc = TargetLoc -
			FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		// 두 위치 사이의 거리를 구해준다.
		float	Distance = FVector::Distance(MonsterLoc, TargetLoc);

		// 두 위치 사이의 거리에서 Capsule의 반경을 뺀다.
		Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
		Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (Distance > Info.AttackDistance)
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		// 여전히 공격거리 안에 존재할 경우 방향을 타겟쪽으로
		// 변경한다.
		else
		{
			// 타겟을 바라볼 방향을 구한다.
			FVector	Dir = TargetLoc - MonsterLoc;
			Dir.Z = 0.f;
			// Normalize 는 벡터를 정규화하는 함수이다.
			Dir.Normalize();

			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}
		
		Monster->SetAttackEnd(false);
	}
}

void UBTTask_Attack::OnTaskFinished(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}


