// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Ghost.generated.h"

enum class EGhostType : uint8
{
	Time,
	Fade
};

UCLASS()
class UE11_API AGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhost();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UPoseableMeshComponent* mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UMaterialInterface* mGhostMaterial;

	TArray<UMaterialInstanceDynamic*>	mMtrlArray;

	EGhostType	mGhostType;

	float		mLifeTime;
	float		mFadeTime;
	float		mFadeTimeMax;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetGhostType(EGhostType Type)
	{
		mGhostType = Type;
	}
	void SetMesh(const FString& Path);
	void SetMesh(USkeletalMesh* Mesh);
	void CopyAnimation(USkeletalMeshComponent* Com);
	void SetGhostMaterial(const FString& Path);
	void SetGhostMaterial(UMaterialInterface* Material);
};
