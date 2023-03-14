// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHUDBase.h"

void UStartHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	mStartButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartButton"))));
	mEndButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("EndButton"))));

	// StartButton �� Ŭ��, ȣ��, ��ȣ�� ��Ȳ�� �߻��� �� ȣ�� ��ų ��������Ʈ ���
	mStartButton->OnClicked.AddDynamic(this, &UStartHUDBase::OnStartBtnClicked);
	mStartButton->OnHovered.AddDynamic(this, &UStartHUDBase::OnStartBtnHover);
	mStartButton->OnUnhovered.AddDynamic(this, &UStartHUDBase::OnStartBtnUnhover);

	// StartHUD �� �����ϰ� �ִ� ��� ���� �ִϸ��̼��� ã�Ƴ���.
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	for (int i = 0; i < WidgetClass->Animations.Num(); ++i)
	{
		FString name = WidgetClass->Animations[i]->GetName();
		mMapWidgetAnim.Add(name, WidgetClass->Animations[i]);
	}
}

void UStartHUDBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UStartHUDBase::OnStartBtnClicked()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Main")));

	if (!mNextLevelName.IsNone())
		UGameplayStatics::OpenLevel(GetWorld(), mNextLevelName);
}

void UStartHUDBase::OnStartBtnHover()
{
	UWidgetAnimation* pAnim = mMapWidgetAnim.FindRef(TEXT("StartBtnHoverAnim_INST"));
	if (IsValid(pAnim))
		PlayAnimation(pAnim);

	PlayAnimation(StartBtnHoverAnim);
}

void UStartHUDBase::OnStartBtnUnhover()
{
	UWidgetAnimation* pAnim = mMapWidgetAnim.FindRef(TEXT("StartBtnUnhoverAnim_INST"));
	if (IsValid(pAnim))
		PlayAnimation(pAnim);
}
