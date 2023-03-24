// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectGameMode.h"


ASelectGameMode::ASelectGameMode()
{
	// UI_StartHUD 블루프린트 클래스 의 UClass 정보를 가져온다.
	ConstructorHelpers::FClassFinder<USelectWidget>
		finder(TEXT("WidgetBlueprint'/Game/Blueprints/JeeHun/UMG/UI_Select.UI_Select_C'"));

	if (finder.Succeeded())
	{
		mSelectHUDClass = finder.Class;
	}
}

void ASelectGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 블루프린트 UClass 정보를 이용해서 객체를 만들어낸다.
	if (IsValid(mSelectHUDClass))
	{
		// 생성한 객체의 주소를 m_MainHUD 에 받아둔다.
		mSelectHUD = Cast<USelectWidget>(CreateWidget(GetWorld(), mSelectHUDClass));
		if (IsValid(mSelectHUD))
		{
			mSelectHUD->AddToViewport();
		}
	}
}

void ASelectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
