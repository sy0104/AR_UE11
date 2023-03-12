// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mMesh);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	Mtrl(TEXT("MaterialInstanceConstant'/Game/Materials/MTGhost_Inst.MTGhost_Inst'"));

	if (Mtrl.Succeeded())
		mGhostMaterial = Mtrl.Object;

	mGhostType = EGhostType::Time;

	mLifeTime = 2.f;
	mFadeTime = 0.f;
	mFadeTimeMax = 2.f;
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mLifeTime -= DeltaTime;

	if (mLifeTime <= 0.f)
	{
		switch (mGhostType)
		{
		case EGhostType::Time:
			Destroy();
			break;
		case EGhostType::Fade:
		{
			mFadeTime += DeltaTime;

			float	Opacity = 1.f - mFadeTime / mFadeTimeMax;

			for (auto& Mtrl : mMtrlArray)
			{
				Mtrl->SetScalarParameterValue(TEXT("Opacity"), Opacity);
			}

			if (mFadeTime >= mFadeTimeMax)
				Destroy();
		}
			break;
		}
	}
}

void AGhost::SetMesh(const FString& Path)
{
	USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(GetWorld(), *Path);

	SetMesh(Mesh);
}

void AGhost::SetMesh(USkeletalMesh* Mesh)
{
	if (!IsValid(Mesh))
		return;

	mMesh->SetSkeletalMesh(Mesh);

	if (mGhostMaterial)
	{
		// GetNumMaterials()
		// 이 메쉬 컴포넌트가 사용하고 있는 머티리얼의 개수를 반환해준다.
		// 그 머티리얼 개수를 받아와서 전부 다이나믹 머티리얼 인스턴스로 만들어준다.
		int32	Count = mMesh->GetNumMaterials();

		for (int32 i = 0; i < Count; ++i)
		{
			UMaterialInstanceDynamic* Mtrl =
				mMesh->CreateDynamicMaterialInstance(i, mGhostMaterial);

			mMtrlArray.Add(Mtrl);
		}
	}
}

void AGhost::CopyAnimation(USkeletalMeshComponent* Com)
{
	// 플레이어 모션 도중 내가 원하는 일정 순간마다 복제한 스켈레탈을 만들고 그것으로 잔상을 만든다.
	mMesh->CopyPoseFromSkeletalComponent(Com);
}

void AGhost::SetGhostMaterial(const FString& Path)
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(GetWorld(), *Path);

	SetGhostMaterial(Material);
}

void AGhost::SetGhostMaterial(UMaterialInterface* Material)
{
	if (!IsValid(Material))
		return;

	mGhostMaterial = Material;
}

