// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class UE11_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMonsterInfo	mInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName		mMonsterTableRowName;

	class UMonsterAnimInstance* mAnimInst;

	class AMonsterSpawnPoint* mSpawnPoint;

	bool		mAttackEnd;

	TArray<FVector>		mPatrolPointLocationArray;
	EPatrolEndDir		mPatrolDir;
	int32				mPatrolIndex;
	int32				mPatrolIndexAdd;
	float				mPatrolWaitTime;

public:
	bool GetPatrolEnable()	const
	{
		return mPatrolPointLocationArray.Num() >= 2;
	}

	const FVector& GetPatrolLocation()	const
	{
		return mPatrolPointLocationArray[mPatrolIndex];
	}

	float GetPatrolWaitTime()	const
	{
		return mPatrolWaitTime;
	}

	bool GetAttackEnd()	const
	{
		return mAttackEnd;
	}

	const FMonsterInfo& GetMonsterInfo()	const
	{
		return mInfo;
	}

	class UMonsterAnimInstance* GetMonsterAnimInst()	const
	{
		return mAnimInst;
	}

public:
	void NextPatrolPoint()
	{
		mPatrolIndex += mPatrolIndexAdd;

		if (mPatrolIndex == mPatrolPointLocationArray.Num())
		{
			switch (mPatrolDir)
			{
			case EPatrolEndDir::Progress:
				mPatrolIndex = 0;
				break;
			case EPatrolEndDir::Repeat:
				mPatrolIndexAdd = -1;
				mPatrolIndex = mPatrolPointLocationArray.Num() - 2;
				break;
			}
		}

		else if (mPatrolIndex < 0)
		{
			mPatrolIndexAdd = 1;
			mPatrolIndex = 1;
		}
	}

	void ClearPatrolWaitTime()
	{
		mPatrolWaitTime = 0.f;
	}

	void AddPatrolWaitTime(float Time)
	{
		mPatrolWaitTime += Time;
	}

	void SetPatrolDir(EPatrolEndDir Dir)
	{
		mPatrolDir = Dir;
	}

	void SetPatrolPointLocation(const TArray<FVector>& Array)
	{
		mPatrolPointLocationArray = Array;
	}

	void SetAttackEnd(bool AttackEnd)
	{
		mAttackEnd = AttackEnd;
	}

	void SetSpawnPoint(class AMonsterSpawnPoint* SpawnPoint)
	{
		mSpawnPoint = SpawnPoint;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();
};
