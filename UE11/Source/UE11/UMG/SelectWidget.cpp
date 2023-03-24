// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"


void USelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartBtn"))));

	// StartBtn �� Ŭ��, ȣ��, ��ȣ�� ��Ȳ�� �߻��� �� ȣ�� ��ų ��������Ʈ ���
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
