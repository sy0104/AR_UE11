// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"


void USelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartBtn"))));

	// StartBtn 에 클릭, 호버, 언호버 상황이 발생할 때 호출 시킬 델리게이트 등록
	m_StartBtn->OnClicked.AddDynamic(this, &USelectWidget::OnStartBtnClicked);
}

void USelectWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void USelectWidget::OnStartBtnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Main")));
}
