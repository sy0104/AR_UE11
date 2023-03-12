// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Ghost.generated.h"

enum class EGhostType : uint8
{
	Time,	// 잔상이 일정 시간동안 지속되다가 바로 제거됨
	Fade	// 잔상이 일정 시간동안 나와있다가 FadeOut 효과를 이용해서 투명도를 0으로 만들어 제거
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
	UMaterialInterface* mGhostMaterial;		// Ghost가 원본 머티리얼을 들고 있도록 해준다.

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
