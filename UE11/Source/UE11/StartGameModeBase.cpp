// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameModeBase.h"

AStartGameModeBase::AStartGameModeBase()
{
	// UI_StartHUD 블루프린트 클래스 의 UClass 정보를 가져온다.
	ConstructorHelpers::FClassFinder<UUserWidget>
		finder(TEXT("WidgetBlueprint'/Game/Blueprints/JeeHun/UMG/UI_StartHUD.UI_StartHUD_C'"));

	if (finder.Succeeded())
	{
		m_StartHUDClass = finder.Class;
	}
}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 블루프린트 UClass 정보를 이용해서 객체를 만들어낸다.
	if (IsValid(m_StartHUDClass))
	{
		// 생성한 객체의 주소를 m_MainHUD 에 받아둔다.
		m_StartHUD = Cast<UUserWidget>(CreateWidget(GetWorld(), m_StartHUDClass));
		if (IsValid(m_StartHUD))
		{
			m_StartHUD->AddToViewport();			
		}
	}
}

void AStartGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
