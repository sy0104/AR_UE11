// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterAnimInstance.h"
#include "MonsterSpawnPoint.h"
#include "../UE11GameInstance.h"
#include "MonsterAIController.h"
#include "../UMG/MonsterHPBase.h"

// Sets default values
AMonster::AMonster()
	: m_Ratio(1.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 몬스터의 HP UI 를 표현해 줄 WidgetComponent 를 추가한다.
	mWidgetComopnent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCom"));
	mWidgetComopnent->SetupAttachment(GetMesh());

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

	mPatrolEnable = false;
	mPatrolCurrentDistance = 0.f;

	mDissolveEnable = false;
	mDissolveMax = 1.5f;
	mDissolveMin = -1.f;
	mDissolveTimeMax = 3.f;
	mDissolveTime = 0.f;

	mDissolve = 0.f;
	mDissolveRange = mDissolveMax - mDissolveMin;

	mSkillEnable = false;

	// -1은 사용하는 스킬이 없다는 의미이다.
	mUseSkillIndex = -1;

	//GetMesh()->SetRenderCustomDepth(true);
	//GetMesh()->SetCustomDepthStencilValue(16);
}

FVector AMonster::GetPatrolLocation() const
{
	switch (mPatrolType)
	{
	case EPatrolType::Point:
		return mPatrolPointLocationArray[mPatrolIndex];
	case EPatrolType::Spline:
		return mSpawnPoint->GetSplinePoint(mPatrolCurrentDistance);
	}

	return FVector::ZeroVector;
}

FVector AMonster::GetPatrolPointLocation() const
{
	switch (mPatrolType)
	{
	case EPatrolType::Point:
		return mPatrolPointLocationArray[mPatrolIndex];
	case EPatrolType::Spline:
		return mSpawnPoint->GetSplinePoint(mPatrolIndex * mPatrolCellDistance);
	}

	return FVector::ZeroVector;
}

void AMonster::OnDissolve()
{
	if (mDissolveMtrlArray.IsEmpty())
		return;

	mDissolveEnable = true;

	int32	Count = mDissolveMtrlArray.Num();

	for (int32 i = 0; i < Count; ++i)
	{
		mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("DissolveEnable"), 1.f);
	}
}

void AMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
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

		// 이동속도를 적용한다.
		// 앉아서 이동시 이동속도의 반으로 지정한다.
		GetCharacterMovement()->MaxWalkSpeed = mInfo.MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = mInfo.MoveSpeed * 0.5f;

		GetMesh()->SetSkeletalMesh(Info->Mesh);

		GetMesh()->SetAnimInstanceClass(Info->MonsterAnimClass);

		int32	IndexCount = mDissolveMtrlIndexArray.Num();

		for (int32 i = 0; i < IndexCount; ++i)
		{
			UMaterialInstanceDynamic* Mtrl = 
				GetMesh()->CreateDynamicMaterialInstance(mDissolveMtrlIndexArray[i].Index,
					mDissolveMtrlIndexArray[i].Mtrl);

			mDissolveMtrlArray.Add(Mtrl);
		}
	}
	
	mAnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());


	// 위젯컴포넌트가 지정한 MonsterHPBase 위젯에 프로그레스바를 조정한다.
	UMonsterHPBase* HPWidget = Cast<UMonsterHPBase>(mWidgetComopnent->GetWidget());
	if (IsValid(HPWidget))
	{
		HPWidget->SetInitHP(1.f);
	}

	int32 SkillCount = mSkillNameArray.Num();

	for (int32 i = 0; i < SkillCount; ++i)
	{
		const FSkillData* Data =
			GameInst->FindMonsterSkillTable(mSkillNameArray[i]);

		FMonsterSkillInfo	SkillInfo;

		SkillInfo.Type = Data->Type;
		SkillInfo.System = Data->System;
		SkillInfo.SkillName = Data->SkillName;
		SkillInfo.Description = Data->Description;
		SkillInfo.SkillOptionArray = Data->SkillOptionArray;
		SkillInfo.SkillEffectArray = Data->SkillEffectArray;
		SkillInfo.SkillUseDataArray = Data->SkillUseDataArray;

		SkillInfo.UseSkill = false;
		SkillInfo.UseMulti = false;

		int32 UseCount = SkillInfo.SkillUseDataArray.Num();

		for (int32 j = 0; j < UseCount; ++j)
		{
			if (SkillInfo.SkillUseDataArray[j].Type == ESkillUseType::Duration)
			{
				SkillInfo.UseMulti = true;
				break;
			}
		}

		SkillInfo.Duration = 0.f;
		SkillInfo.Distance = Data->Distance;
		SkillInfo.AnimType = Data->AnimType;

		mSkillDataArray.Add(SkillInfo);
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UMonsterHPBase* HPWidget = Cast<UMonsterHPBase>(mWidgetComopnent->GetWidget());	
	m_Ratio -= DeltaTime * 0.05f;

	if (IsValid(HPWidget))
	{
		HPWidget->SetHP(m_Ratio);
	}

	// 현재 패트롤 상태일 경우 속도벡터를 이용하여 이동양을 구한다.
	if (mPatrolEnable)
	{
		// 현재 도착지점의 인덱스를 이용해서 그 값을 넘어갈 경우 해당 값으로
		// 고정하여 거기까지만 이동할 수 있도록 한다.
		mPatrolCurrentDistance += (GetCharacterMovement()->MaxWalkSpeed *
			DeltaTime * mPatrolIndexAdd);

		if (GetArrive())
		{
			if (mPatrolIndexAdd == 1)
			{
				mPatrolCurrentDistance = mPatrolIndex * mPatrolCellDistance;
			}

			else
			{
				mPatrolCurrentDistance = mPatrolIndex * mPatrolCellDistance;
			}
			mPatrolEnable = false;
		}
	}

	if (mDissolveEnable)
	{
		mDissolveTime += DeltaTime;

		// 총 시간에서의 비율을 구한다.
		float	Ratio = mDissolveTime / mDissolveTimeMax;

		// 비율을 이용하여 Dissolve값의 전체 범위에 곱해서 얼마나 진행되었는지를 구한다.
		mDissolve = mDissolveMax - Ratio * mDissolveRange;

		int32	Count = mDissolveMtrlArray.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("Dissolve"), mDissolve);
		}

		if (mDissolveTime >= mDissolveTimeMax)
		{
			mSpawnPoint->RemoveMonster(this);
			Destroy();
		}
	}

	if (!mSkillEnable)
		UseSkill(DeltaTime);
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

		// 죽었다.
		mAnimInst->ChangeAnim(EMonsterAnimType::Death);

		AAIController* AI = Cast<AAIController>(GetController());

		if (IsValid(AI))
			AI->BrainComponent->StopLogic(TEXT("Death"));

		//mSpawnPoint->RemoveMonster(this);
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

	// 부모 기능을 실행하기 전에 먼저 BehaviorTree와
	// Blackboard를 지정해준다.
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

void AMonster::Attack()
{
}

void AMonster::PauseAI()
{
	AAIController* AI = Cast<AAIController>(GetController());

	if (IsValid(AI))
		AI->BrainComponent->StopLogic(TEXT("Pause"));

	PrintViewport(1.f, FColor::Red, TEXT("Pause AI"));
}

void AMonster::ReStartAI()
{
	AAIController* AI = Cast<AAIController>(GetController());

	if (IsValid(AI))
		AI->BrainComponent->StartLogic();

	PrintViewport(1.f, FColor::Red, TEXT("ReStart AI"));

	//mAnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
}

void AMonster::UseSkill(float DeltaTime)
{
	// 전투상태인지 판단한다.
	AMonsterAIController* AIController =
		Cast<AMonsterAIController>(GetController());

	AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
		return;

	float	HalfHeight = 0.f;
	
	if (Cast<ACharacter>(Target))
		HalfHeight = Cast<ACharacter>(Target)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector	TargetLoc = Target->GetActorLocation();
	TargetLoc.Z -= HalfHeight;
	FVector	Loc = GetActorLocation();
	Loc.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	float	Dist = (float)FVector::Distance(TargetLoc, Loc);

	Dist -= GetCapsuleComponent()->GetScaledCapsuleRadius();
	Dist -= Cast<ACharacter>(Target)->GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 스킬을 사용해야 하는지 판단한다.
	int32	SkillCount = mSkillDataArray.Num();

	for (int32 i = 0; i < SkillCount; ++i)
	{
		// 스킬을 사용했는데 해당 스킬이 1번만 사용되고 더이상 사용 못할 경우
		if (mSkillDataArray[i].UseSkill)
		{
			if (!mSkillDataArray[i].UseMulti)
			{
				continue;
			}
		}

		// 사용된 스킬이 아니라면 현재 스킬
		int32 UseDataCount = mSkillDataArray[i].SkillUseDataArray.Num();

		bool	Use = true;

		for (int32 j = 0; j < UseDataCount; ++j)
		{
			switch (mSkillDataArray[i].SkillUseDataArray[j].Type)
			{
			case ESkillUseType::HPPercent:
				// 체력계산.
				if ((float)mInfo.HP / mInfo.HPMax >
					mSkillDataArray[i].SkillUseDataArray[j].Data)
				{
					Use = false;
				}
				break;
			case ESkillUseType::Duration:
				mSkillDataArray[i].Duration += DeltaTime;

				if (mSkillDataArray[i].Duration <
					mSkillDataArray[i].SkillUseDataArray[j].Data)
				{
					Use = false;
				}
				break;
			case ESkillUseType::Ratio:
				float Percent;
				Percent = FMath::FRand();

				if (Percent > mSkillDataArray[i].SkillUseDataArray[j].Data)
					Use = false;
				break;
			}

			if (!Use)
				break;
		}

		if (!mSkillEnable)
		{
			if (Dist > mSkillDataArray[i].Distance)
				Use = false;

			// 모두 통과했으므로 스킬을 사용해야 한다.
			if (Use)
			{
				mSkillDataArray[i].UseSkill = true;
				mSkillEnable = true;
				mUseSkillIndex = i;

				AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("SkillEnable"), true);
			}
		}
	}
}

void AMonster::ClearSkill()
{
	mUseSkillIndex = -1;
	mSkillEnable = false;

	int32	SkillCount = mSkillDataArray.Num();

	for (int32 i = 0; i < SkillCount; ++i)
	{
		mSkillDataArray[i].UseSkill = false;
		mSkillDataArray[i].Duration = 0.f;
	}

	AMonsterAIController* AIController =
		Cast<AMonsterAIController>(GetController());

	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("SkillEnable"), false);
}

void AMonster::ClearCurrentSkill()
{
	if (mUseSkillIndex == -1)
		return;

	mSkillDataArray[mUseSkillIndex].UseSkill = false;
	mSkillDataArray[mUseSkillIndex].Duration = 0.f;

	mUseSkillIndex = -1;
	mSkillEnable = false;

	AMonsterAIController* AIController =
		Cast<AMonsterAIController>(GetController());

	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("SkillEnable"), false);
}

void AMonster::Skill1()
{
}

void AMonster::Skill2()
{
}

void AMonster::Skill3()
{
}
