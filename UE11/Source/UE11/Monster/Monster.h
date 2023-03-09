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

	AActor* mHitActor;

	EPatrolType	mPatrolType;

	class UMonsterAnimInstance* mAnimInst;
	class AMonsterSpawnPoint* mSpawnPoint;	// 자신이 태어난 스폰 포인트를 알고있도록 한다

	bool		mAttackEnd;

	TArray<FVector>		mPatrolPointLocationArray;
	EPatrolEndDir		mPatrolDir;
	int32				mPatrolIndex;
	int32				mPatrolIndexAdd;
	float				mPatrolWaitTime;

	bool				mPatrolEnable;

	int32				mPatrolSplineCount;
	float				mPatrolSplineLength;
	float				mPatrolCellDistance;
	float				mPatrolCurrentDistance;

	float				mPatrolTime;
	float				mPatrolTimeAcc;

public:
	bool GetArrive()	const
	{
		float	Dist = 10.f + GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (mPatrolIndexAdd == 1)
			return mPatrolIndex * mPatrolCellDistance - Dist <= mPatrolCurrentDistance;

		return mPatrolIndex * mPatrolCellDistance + Dist >= mPatrolCurrentDistance;
	}

	int32 GetPatrolSplineCount()	const
	{
		return mPatrolSplineCount;
	}

	float GetPatrolSplineLength()	const
	{
		return mPatrolSplineLength;
	}

	EPatrolType GetPatrolType()	const
	{
		return mPatrolType;
	}

	bool GetPatrolEnable()	const
	{
		return mPatrolPointLocationArray.Num() >= 2;
	}

	FVector GetPatrolLocation()	const; // 현재 이동해야 하는 지점의 위치 변환
	FVector GetPatrolPointLocation() const;	// 지점이 1개라면 patrol을 할 수 있다.

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
	void SetPatrolEnable(bool Enable)
	{
		mPatrolEnable = Enable;
	}

	void SetPatrolSplineCount(int32 Count)
	{
		mPatrolSplineCount = Count;
	}

	void SetPatrolCellDistance(float Length)
	{
		mPatrolCellDistance = Length;
	}

	void SetPatrolSplineLength(float Length)
	{
		mPatrolSplineLength = Length;
	}

	void SetPatrolType(EPatrolType Type)
	{
		mPatrolType = Type;
	}

	void NextPatrolPoint()
	{
		mPatrolIndex += mPatrolIndexAdd;

		switch (mPatrolType)
		{
		case EPatrolType::Point:
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
			break;
		case EPatrolType::Spline:
			if (mPatrolIndex == mPatrolSplineCount + 1)
			{
				switch (mPatrolDir)
				{
				case EPatrolEndDir::Progress:
					mPatrolIndex = 1;
					mPatrolCurrentDistance -= mPatrolSplineLength;
					break;
				case EPatrolEndDir::Repeat:
					mPatrolCurrentDistance = mPatrolSplineLength - 100.f -
						GetCapsuleComponent()->GetScaledCapsuleRadius();
					mPatrolIndexAdd = -1;
					mPatrolIndex = mPatrolSplineCount - 1;
					break;
				}
			}

			else if (mPatrolIndex < 0)
			{
				mPatrolCurrentDistance = 100.f +
					GetCapsuleComponent()->GetScaledCapsuleRadius();
				mPatrolIndexAdd = 1;
				mPatrolIndex = 1;
			}
			break;
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

public:
	virtual void Attack();
};
