// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class UE11_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	EOutLineColor	mOutLineColor;
	int32			mStencilValue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void OnOutLine(bool Enable)
	{
		if (Enable)
			mStencilValue += 10;

		else
			mStencilValue -= 10;

		GetMesh()->SetCustomDepthStencilValue(mStencilValue);
	}
};
