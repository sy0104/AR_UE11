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

// 실행할 때 한번 실행되는 함수
EBTNodeResult::Type UBTTask_TargetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

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
		Controller->StopMovement();					// 타겟이 없다면 or 타겟이 추적범위를 벗어났다면 이동을 멈춘다.
		Anim->ChangeAnim(EMonsterAnimType::Idle);	// 애니메이션을 Idle로 전환한다.

		return EBTNodeResult::Failed;
	}

	const FMonsterInfo& Info = Monster->GetMonsterInfo();
	Monster->GetCharacterMovement()->MaxWalkSpeed = Info.MoveSpeed;

	// 타겟 위치로 이동시킨다.
	UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, Target);

	// 2번 호출된다.
	//Controller->MoveToActor();

	// 타겟을 찾았다면 모션을 CombatRun으로 변경한다.
	Anim->ChangeAnim(EMonsterAnimType::CombatRun);

	// 몬스터가 타겟에 도착할때까지 이 태스크를 빠져나가지 못하게한다.
	return EBTNodeResult::InProgress;
}

// 중단될 때 들어오는 함수
EBTNodeResult::Type UBTTask_TargetTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_TargetTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	// Blackboard에 저장된 Target을 얻어와야 한다.
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
	{
		Controller->StopMovement();								// 이동을 멈춘다.
		Anim->ChangeAnim(EMonsterAnimType::Idle);				// 애니메이션을 Idle로 전환한다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task를 종료시킨다.

		return;
	}

	// 이동중 타겟과의 거리를 체크하여 거리가 공격거리 안에 들어갔을 경우 공격으로 전환한다.
	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	// 몬스터의 위치와 타겟의 위치를 이용하여 거리를 구한다.
	// 기준은 발밑 위치를 기준으로 한다.
	// -> 크기 차이때문에 높이 차이가 나기 때문에 서로 멀리 있을 수도 있음. Z를 맞춰주기 보다는 발밑을 구한다.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// 두 위치 사이의 거리를 구해준다.
	float Distance = FVector::Distance(MonsterLoc, TargetLoc);

	// 두 위치 사이의 거리에서 Capsule의 반경을 뺀다.
	Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (Distance <= Info.AttackDistance)
	{
		Controller->StopMovement();		// 이동을 멈춘다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_TargetTrace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
