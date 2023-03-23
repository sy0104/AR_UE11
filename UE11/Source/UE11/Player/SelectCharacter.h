// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "SelectCharacter.generated.h"

UCLASS()
class UE11_API ASelectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASelectCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPlayerJob		mJob;

	int32	mStencilValue;

public:
	EPlayerJob GetJob() const
	{
		return mJob;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void NotifyActorBeginCursorOver();
	virtual void NotifyActorEndCursorOver();
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton);
	virtual void NotifyActorOnReleased(FKey ButtonReleased = EKeys::LeftMouseButton);
	virtual void NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex);
	virtual void NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex);
	virtual void NotifyActorOnInputTouchEnter(const ETouchIndex::Type FingerIndex);
	virtual void NotifyActorOnInputTouchLeave(const ETouchIndex::Type FingerIndex);
};
