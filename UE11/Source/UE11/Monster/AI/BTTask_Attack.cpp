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

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	{

		Controller->StopMovement();					// Ÿ���� ���ٸ� or Ÿ���� ���������� ����ٸ� �̵��� �����.
		Anim->ChangeAnim(EMonsterAnimType::Idle);	// �ִϸ��̼��� Idle�� ��ȯ�Ѵ�.

		return EBTNodeResult::Failed;
	}

	Anim->ChangeAnim(EMonsterAnimType::Attack);

	// ���Ͱ� Ÿ���� ������ ������ �� �½�ũ�� ���������� ���ϰ� �Ѵ�.
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	if (!IsValid(Target))
	{
		Controller->StopMovement();								// �̵��� �����.
		Anim->ChangeAnim(EMonsterAnimType::Idle);				//  �ִϸ��̼��� Idle�� ��ȯ�Ѵ�.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);		// Task�� �����Ų��.

		return;
	}

	// ������ �������� �Ǵ��Ѵ�.
	// ������ ���� �� ���ݰŸ� �ȿ� �ִٸ� �÷��̾ �ִ� �������� ȸ�������ְ� �ƴ϶�� task�� �����Ų��.
	if (Monster->GetAttackEnd())
	{
		// Ÿ�ٰ� ���Ϳ��� �Ÿ��� üũ�Ѵ�.
		const FMonsterInfo& Info = Monster->GetMonsterInfo();

		// ������ ��ġ�� Ÿ���� ��ġ�� �̿��Ͽ� �Ÿ��� ���Ѵ�.
		// ������ �߹� ��ġ�� �������� �Ѵ�.
		// -> ũ�� ���̶����� ���� ���̰� ���� ������ ���� �ָ� ���� ���� ����. Z�� �����ֱ� ���ٴ� �߹��� ���Ѵ�.
		FVector MonsterLoc = Monster->GetActorLocation();
		FVector TargetLoc = Target->GetActorLocation();

		MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		// �� ��ġ ������ �Ÿ��� �����ش�.
		float Distance = FVector::Distance(MonsterLoc, TargetLoc);

		// �� ��ġ ������ �Ÿ����� Capsule�� �ݰ��� ����.
		Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
		Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

		// �� ��ġ ������ �Ÿ��� ���ݰŸ����� ũ�� Task�� ��������ش�.
		// ���Ͱ� ������ �� ���´µ� Ÿ���� ���� �Ÿ��� �������� ���
		if (Distance > Info.AttackDistance)
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		// ������ ���ݰŸ� �ȿ� ������ ��� ������ Ÿ�������� �����Ѵ�.
		// ������ ������ ���� ���ؼ� ���� �ڷ� �� ���
		// ���� �������� ���� ����� �� ���� �� �ڷ� ����.
		else
		{
			// Ÿ���� �ٶ� ������ ���Ѵ�.
			FVector Dir = TargetLoc - MonsterLoc;
			Dir.Z = 0.f;
			Dir.Normalize();	// ���� ����ȭ

			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}

		Monster->SetAttackEnd(false);
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}