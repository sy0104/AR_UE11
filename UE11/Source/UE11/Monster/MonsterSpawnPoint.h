// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

UCLASS()
class UE11_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* mRoot;

	// TSubclassOf �� ���ø����� ���� Ÿ���� UCLASS ������ �����ϴ� Ÿ���̴�.
	// Calss ������ �����صξ��ٰ� �ʿ�� �ش� Ŭ���� ������ �̿��Ͽ� ��ü�� ����
	// �� �� �ִ� �뵵�� ����� �� �ִ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMonster>		mSpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mSpawnCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonster*>	mMonsterArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPatrolType	mPatrolType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USplineComponent* mPatrolSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mDivideCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mDivideMeshVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh*	mDivideMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FVector>	mSplinePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FRotator>	mSplineRotation;

	float	mSplineLength;
	float	mCellDistance;

	int32	mPrevDivideCount;
	float	mPrevLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*>	mMeshArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonsterPatrolPoint*>	mPatrolPointArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPatrolEndDir	mPatrolDir;

	TArray<FVector>		mPatrolPointLocationArray;

	float	mTime;

public:
	float GetSplineLength()	const
	{
		return mSplineLength;
	}

	const TArray<FVector>& GetSplinePointArray()	const
	{
		return mSplinePoint;
	}

	FVector GetSplinePoint(int32 Index)	const
	{
		return mSplinePoint[Index];
	}

	FVector GetSplinePoint(float Distance)	const
	{
		return mPatrolSpline->GetLocationAtDistanceAlongSpline(
			Distance, ESplineCoordinateSpace::World);
	}

	void RemoveMonster(class AMonster* Monster)
	{
		mMonsterArray.Remove(Monster);
	}

public:
	virtual void OnConstruction(const FTransform& Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void ComputeSpline();
};
