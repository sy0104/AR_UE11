// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameModeBase.h"

AStartGameModeBase::AStartGameModeBase()
{
	// UI_StartHUD �������Ʈ Ŭ���� �� UClass ������ �����´�.
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

	// ���� �������Ʈ UClass ������ �̿��ؼ� ��ü�� ������.
	if (IsValid(m_StartHUDClass))
	{
		// ������ ��ü�� �ּҸ� m_MainHUD �� �޾Ƶд�.
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
