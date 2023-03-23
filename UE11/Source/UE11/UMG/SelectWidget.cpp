// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"

void USelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mStartBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartBtn"))));
	mStartBtn->OnClicked.AddDynamic(this, &USelectWidget::OnStartBtnClicked);
}

void USelectWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void USelectWidget::OnStartBtnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Main")));
}
