// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectGameMode.h"

ASelectGameMode::ASelectGameMode()
{
	ConstructorHelpers::FClassFinder<USelectWidget>
		finder(TEXT("WidgetBlueprint'/Game/Blueprints/UIClass/UMG/UI_Select.UI_Select_C'"));

	if (finder.Succeeded())
		mSelectHUDClass = finder.Class;
}

void ASelectGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(mSelectHUDClass))
	{
		mSelectHUD = Cast<USelectWidget>(CreateWidget(GetWorld(), mSelectHUDClass));

		if (IsValid(mSelectHUD))
			mSelectHUD->AddToViewport();
	}
}

void ASelectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
