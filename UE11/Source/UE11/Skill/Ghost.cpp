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
	mMesh->CopyPoseFromSkeletalComponent(Com);
}

void AGhost::SetGhostMaterial(const FString& Path)
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(
		GetWorld(), *Path);

	SetGhostMaterial(Material);
}

void AGhost::SetGhostMaterial(UMaterialInterface* Material)
{
	if (!IsValid(Material))
		return;

	mGhostMaterial = Material;
}

