// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "Monster.h"
#include "AI/MonsterPatrolPoint.h"

// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	mSpawnTime = 3.f;
	mTime = 0.f;
	mSpawnCount = 1;
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	// SpawnPoint�� ��ġ�� �߰��Ѵ�.
	mPatrolPointLocationArray.Add(GetActorLocation());

	int32 PatrolPointCount = mPatrolPointArray.Num();

	for (int32 i = 0; i < PatrolPointCount; ++i)
		mPatrolPointLocationArray.Add(mPatrolPointArray[i]->GetActorLocation());
	

	// ������ ���� Ŭ������ ���� ��쿡�� �����Ѵ�.
	if (IsValid(mSpawnClass))
	{
		mSpawnCount = mSpawnCount < 1 ? 1 : mSpawnCount;	// mSpawnCount�� 0���� ������ �ȵ�

		FActorSpawnParameters Spawnparam;
		Spawnparam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMonster* SpawnMonster =
			GetWorld()->SpawnActor<AMonster>(mSpawnClass, GetActorLocation(), GetActorRotation(), Spawnparam);

		SpawnMonster->SetSpawnPoint(this);
		SpawnMonster->SetPatrolPointLocation(mPatrolPointLocationArray);
		SpawnMonster->SetPatrolDir(mPatrolDir);
		mMonsterArray.Add(SpawnMonster);
	}
}

// Called every frame
void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ���� Ŭ������ ���� ��쿡�� �����Ѵ�.
	if (IsValid(mSpawnClass))
	{
		// �迭�� ����� ������ ���� �����ؾ��� ������ ���� ���ٸ� ��� �����Ǿ��ٴ� ���̴�.
		if (mMonsterArray.Num() == mSpawnCount)
		{
			mTime += DeltaTime;

			if (mTime >= mSpawnTime)
			{
				mTime -= mSpawnTime;

				FActorSpawnParameters Spawnparam;
				Spawnparam.SpawnCollisionHandlingOverride = 
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AMonster* SpawnMonster =
					GetWorld()->SpawnActor<AMonster>(mSpawnClass, GetActorLocation(), GetActorRotation(), Spawnparam);

				SpawnMonster->SetSpawnPoint(this);
				mMonsterArray.Add(SpawnMonster);

				if (mMonsterArray.Num() == mSpawnCount)
					mTime = 0.f;
			}
		}
	}
}

