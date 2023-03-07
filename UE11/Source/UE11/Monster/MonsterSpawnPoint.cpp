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
	mPatrolSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolSpline"));

	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	mSpawnTime = 3.f;
	mTime = 0.f;
	mSpawnCount = 1;

	mPatrolType = EPatrolType::Point;

	mDivideCount = 5;
	mDivideMeshVisible = true;
	mSplineLength = 0.f;
	mCellDistance = 0.f;
	mPrevDivideCount = 0;
	mPrevLength = 0.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DivideMesh(TEXT("StaticMesh'/Game/Blueprints/Cube.Cube'"));

	if (DivideMesh.Succeeded())
		mDivideMesh = DivideMesh.Object;
}

void AMonsterSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ComputeSpline();
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

void AMonsterSpawnPoint::ComputeSpline()
{
	if (mDivideMeshVisible)
	{
		if (mDivideCount != mPrevDivideCount)
		{
			int32 ComCount = mMeshArray.Num();

			for (int32 i = 0; i < ComCount; ++i)
			{
				if (mMeshArray[i])
					mMeshArray[i]->DestroyComponent();
			}

			if (mMeshArray.Num() > 0)
				mMeshArray.RemoveAll([](UStaticMeshComponent*) { return true; });
		}
	}

	else
	{
		int32 ComCount = mMeshArray.Num();

		for (int32 i = 0; i < ComCount; ++i)
		{
			if (mMeshArray[i])
				mMeshArray[i]->DestroyComponent();
		}

		if (mMeshArray.Num() > 0)
			mMeshArray.RemoveAll([](UStaticMeshComponent*) { return true; });
	}

	mSplinePoint.RemoveAll([](FVector) { return true; });
	mSplineRotation.RemoveAll([](FRotator) { return true; });

	mSplineLength = mPatrolSpline->GetSplineLength();
	mCellDistance = mSplineLength / mDivideCount;

	FTransform ActorTr = GetActorTransform();

	for (int32 i = 0; i < mDivideCount; ++i)
	{
		FVector Point = mPatrolSpline->GetLocationAtDistanceAlongSpline(mCellDistance * i, ESplineCoordinateSpace::World);
		mSplinePoint.Add(Point);

		// ���� ���� ��ġ�� ���� ��ġ�� ���浵 �����ϴ�.
		FVector LocalPoint = ActorTr.InverseTransformPosition(Point);
		FRotator Rot = mPatrolSpline->GetRotationAtDistanceAlongSpline(mCellDistance * i, ESplineCoordinateSpace::World);
		mSplineRotation.Add(Rot);

		if (mDivideMeshVisible)
		{
			if (mDivideCount != mPrevDivideCount)
			{
				UStaticMeshComponent* MeshCom = NewObject <UStaticMeshComponent>(this);

				// Mesh�� Transform�� ���´�.
				FTransform MeshTr = MeshCom->GetRelativeTransform();

				MeshTr.SetLocation(LocalPoint);
				MeshTr.SetScale3D(FVector(0.1f, 0.1f, 0.1f));

				MeshCom->SetRelativeTransform(MeshTr);
				MeshCom->SetWorldRotation(Rot);
				MeshCom->SetStaticMesh(mDivideMesh);
				MeshCom->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				mMeshArray.Add(MeshCom);

				MeshCom->SetupAttachment(mPatrolSpline);
			}

			else
			{
				mMeshArray[i]->SetRelativeLocation(LocalPoint);
				mMeshArray[i]->SetWorldRotation(Rot);
			}
		}
	}

	mPrevDivideCount = mDivideCount;
	mPrevLength = mSplineLength;
}

