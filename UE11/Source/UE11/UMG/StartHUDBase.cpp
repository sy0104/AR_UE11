// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHUDBase.h"

void UStartHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	mStartButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartButton"))));
	mEndButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("EndButton"))));

	// StartButton 에 클릭, 호버, 언호버 상황이 발생할 때 호출 시킬 델리게이트 등록
	mStartButton->OnClicked.AddDynamic(this, &UStartHUDBase::OnStartBtnClicked);
	mStartButton->OnHovered.AddDynamic(this, &UStartHUDBase::OnStartBtnHover);
	mStartButton->OnUnhovered.AddDynamic(this, &UStartHUDBase::OnStartBtnUnhover);

	// StartHUD 가 보유하고 있는 모든 위젯 애니메이션을 찾아낸다.
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
