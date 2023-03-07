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
	FName			mMonsterTableRowName;

	class UMonsterAnimInstance*		mAnimInst;
	class AMonsterSpawnPoint*		mSpawnPoint;	// 자신이 태어난 스폰 포인트를 알고있도록 한다

	bool	mAttackEnd;

	TArray<FVector>		mPatrolPointLocationArray;
	EPatrolEndDir		mPatrolDir;
	int32				mPatrolIndex;
	int32				mPatrolIndexAdd;
	float				mPatrolWaitTime;

public:
	float GetPatrolWaitTime() const
	{
		return mPatrolWaitTime;
	}

	const FMonsterInfo& GetMonsterInfo() const
	{
		return mInfo;
	}

	class UMonsterAnimInstance* GetMonsterAnimInst() const
	{
		return mAnimInst;
	}

	bool GetAttackEnd() const
	{
		return mAttackEnd;
	}

	const FVector& GetPatrolLocation() const
	{
		return mPatrolPointLocationArray[mPatrolIndex];	// 현재 이동해야 하는 지점의 위치 반환
	}

	bool GetPatrolEnable() const
	{
		return mPatrolPointLocationArray.Num() >= 2;	// 지점이 1개라면 patrol을 할 수 없다.
	}

public:
	void SetSpawnPoint(class AMonsterSpawnPoint* SpawnPoint)
	{
		mSpawnPoint = SpawnPoint;
	}

	void SetAttackEnd(bool AttackEnd)
	{
		mAttackEnd = AttackEnd;
	}

	void SetPatrolPointLocation(const TArray<FVector>& Array)
	{
		mPatrolPointLocationArray = Array;
	}

	void SetPatrolDir(EPatrolEndDir Dir)
	{
		mPatrolDir = Dir;
	}

	void AddPatrolWaitTime(float Time)
	{
		mPatrolWaitTime += Time;
	}

	void ClearPatrolWaitTime()
	{
		mPatrolWaitTime = 0.f;
	}

	void NextPatrolPoint();

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
