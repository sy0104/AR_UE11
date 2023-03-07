// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterAnimInstance.h"
#include "MonsterSpawnPoint.h"
#include "../UE11GameInstance.h"
#include "MonsterAIController.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMonsterAIController::StaticClass();

	mAttackEnd = false;

	mPatrolIndex = 1;
	mPatrolIndexAdd = 1;
	mPatrolWaitTime = 0.f;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	UUE11GameInstance* GameInst = 
		GetWorld()->GetGameInstance<UUE11GameInstance>();

	const FMonsterTableInfo* Info = 
		GameInst->FindMonsterTable(mMonsterTableRowName);

	if (Info)
	{
		mInfo.Name = Info->Name;
		mInfo.AttackPoint = Info->AttackPoint;
		mInfo.ArmorPoint = Info->ArmorPoint;
		mInfo.HP = Info->HP;
		mInfo.HPMax = Info->HP;
		mInfo.MP = Info->MP;
		mInfo.MPMax = Info->MP;
		mInfo.Level = Info->Level;
		mInfo.Exp = Info->Exp;
		mInfo.Gold = Info->Gold;
		mInfo.MoveSpeed = Info->MoveSpeed;
		mInfo.AttackDistance = Info->AttackDistance;
		mInfo.TraceDistance = Info->TraceDistance;

		// �̵��ӵ��� �����Ѵ�.
		// �ɾƼ� �̵��� �̵��ӵ��� ������ �����Ѵ�.
		GetCharacterMovement()->MaxWalkSpeed = mInfo.MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = mInfo.MoveSpeed * 0.5f;

		GetMesh()->SetSkeletalMesh(Info->Mesh);

		GetMesh()->SetAnimInstanceClass(Info->MonsterAnimClass);
	}
	
	mAnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMonster::TakeDamage(float DamageAmount, 
	FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	int32 Damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent,
		EventInstigator, DamageCauser);

	Damage = Damage - mInfo.ArmorPoint;

	Damage = Damage < 1 ? 1 : Damage;

	mInfo.HP -= Damage;

	if (mInfo.HP <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// �׾���.
		mAnimInst->ChangeAnim(EMonsterAnimType::Death);

		AAIController* AI = Cast<AAIController>(GetController());

		if (IsValid(AI))
			AI->BrainComponent->StopLogic(TEXT("Death"));

		mSpawnPoint->RemoveMonster(this);
	}

	else
	{
		mAnimInst->Hit();
	}

	return Damage;
}

void AMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// �θ� ����� �����ϱ� ���� ���� BehaviorTree��
	// Blackboard�� �������ش�.
	AMonsterAIController* AI =
		Cast<AMonsterAIController>(NewController);

	if (IsValid(AI))
	{
		AI->SetBehaviorTree(TEXT("BehaviorTree'/Game/Monster/AI/BTMonster.BTMonster'"));
		AI->SetBlackboard(TEXT("BlackboardData'/Game/Monster/AI/BBMonster.BBMonster'"));
	}
}

void AMonster::UnPossessed()
{
	Super::UnPossessed();
}

