// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameModeBase.h"

AStartGameModeBase::AStartGameModeBase()
{
	// UI_StartHUD 블루프린트 클래스의 UClass 정보를 가져온다.
	ConstructorHelpers::FClassFinder<UUserWidget>
		finder(TEXT("WidgetBlueprint'/Game/Blueprints/UIClass/UMG/UI_StartHUD.UI_StartHUD_C'"));

	if (finder.Succeeded())
		mStartHUDClass = finder.Class;
}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 블루프린트 UClass 정보를 이용해서 객체를 만들어낸다.
	if (IsValid(mStartHUDClass))
	{
		// 생성한 객체의 주소를 mMainHUD 에 받아둔다.
		mStartHUD = Cast<UUserWidget>(CreateWidget(GetWorld(), mStartHUDClass));
		if (IsValid(mStartHUD))
			mStartHUD->AddToViewport();
	}
}

void AStartGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
