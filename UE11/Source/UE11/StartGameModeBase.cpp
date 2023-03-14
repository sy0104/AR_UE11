// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameModeBase.h"

AStartGameModeBase::AStartGameModeBase()
{
	// UI_StartHUD �������Ʈ Ŭ������ UClass ������ �����´�.
	ConstructorHelpers::FClassFinder<UUserWidget>
		finder(TEXT("WidgetBlueprint'/Game/Blueprints/UIClass/UMG/UI_StartHUD.UI_StartHUD_C'"));

	if (finder.Succeeded())
		mStartHUDClass = finder.Class;
}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// ���� �������Ʈ UClass ������ �̿��ؼ� ��ü�� ������.
	if (IsValid(mStartHUDClass))
	{
		// ������ ��ü�� �ּҸ� mMainHUD �� �޾Ƶд�.
		mStartHUD = Cast<UUserWidget>(CreateWidget(GetWorld(), mStartHUDClass));
		if (IsValid(mStartHUD))
			mStartHUD->AddToViewport();
	}
}

void AStartGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
