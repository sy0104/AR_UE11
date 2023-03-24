// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectGameMode.h"


ASelectGameMode::ASelectGameMode()
{
	// UI_StartHUD �������Ʈ Ŭ���� �� UClass ������ �����´�.
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

	// ���� �������Ʈ UClass ������ �̿��ؼ� ��ü�� ������.
	if (IsValid(mSelectHUDClass))
	{
		// ������ ��ü�� �ּҸ� m_MainHUD �� �޾Ƶд�.
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
