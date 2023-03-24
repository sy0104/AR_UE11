// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHUDBase.h"

void UStartHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartBtn"))));
	m_EndBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("EndBtn"))));

	// StartBtn �� Ŭ��, ȣ��, ��ȣ�� ��Ȳ�� �߻��� �� ȣ�� ��ų ��������Ʈ ���
	m_StartBtn->OnClicked.AddDynamic(this, &UStartHUDBase::OnStartBtnClicked);
	m_StartBtn->OnHovered.AddDynamic(this, &UStartHUDBase::OnStartBtnHover);
	m_StartBtn->OnUnhovered.AddDynamic(this, &UStartHUDBase::OnStartBtnUnhover);
	
	// StartHUD �� �����ϰ� �ִ� ��� ���� �ִϸ��̼��� ã�Ƴ���.
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	for (int i = 0; i < WidgetClass->Animations.Num(); ++i)
	{
		FString name = WidgetClass->Animations[i]->GetName();
		m_mapWidgetAnim.Add(name, WidgetClass->Animations[i]);
	}
}

void UStartHUDBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);


}

void UStartHUDBase::OnStartBtnClicked()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Main")));

	if (!m_NextLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), m_NextLevelName);
	}
}

void UStartHUDBase::OnStartBtnHover()
{
	/*UWidgetAnimation* pAnim = m_mapWidgetAnim.FindRef(TEXT("StartBtnHoverAnim_INST"));
	if (IsValid(pAnim))
	{
		PlayAnimation(pAnim);
	}*/

	PlayAnimation(StartBtnHoverAnim);
}

void UStartHUDBase::OnStartBtnUnhover()
{
	UWidgetAnimation* pAnim = m_mapWidgetAnim.FindRef(TEXT("StartBtnUnhoverAnim_INST"));
	if (IsValid(pAnim))
	{
		PlayAnimation(pAnim);
	}
}