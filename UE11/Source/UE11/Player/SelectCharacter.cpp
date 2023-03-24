// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacter.h"
#include "../SelectGameMode.h"
#include "../UMG/SelectWidget.h"
#include "../UE11GameInstance.h"

// Sets default values
ASelectCharacter::ASelectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mStencilValue = 1;

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

// Called when the game starts or when spawned
void ASelectCharacter::BeginPlay()
{
	Super::BeginPlay();


	GetMesh()->SetCustomDepthStencilValue(mStencilValue);
}

// Called every frame
void ASelectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASelectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASelectCharacter::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();

	mStencilValue += 10;

	GetMesh()->SetCustomDepthStencilValue(mStencilValue);
}

void ASelectCharacter::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	mStencilValue -= 10;

	GetMesh()->SetCustomDepthStencilValue(mStencilValue);
}

void ASelectCharacter::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	ASelectGameMode* GameMode = GetWorld()->GetAuthGameMode<ASelectGameMode>();

	if (IsValid(GameMode))
	{
		GameMode->GetSelectWidget()->SetStartButtonVisibility(true);
	}

	UUE11GameInstance* GameInst = GetWorld()->GetGameInstance<UUE11GameInstance>();

	if (IsValid(GameInst))
		GameInst->SetSelectJob(mJob);
}

void ASelectCharacter::NotifyActorOnReleased(FKey ButtonReleased)
{
	Super::NotifyActorOnReleased(ButtonReleased);
}

void ASelectCharacter::NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchBegin(FingerIndex);
}

void ASelectCharacter::NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchEnd(FingerIndex);
}

void ASelectCharacter::NotifyActorOnInputTouchEnter(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchEnter(FingerIndex);
}

void ASelectCharacter::NotifyActorOnInputTouchLeave(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchLeave(FingerIndex);
}

