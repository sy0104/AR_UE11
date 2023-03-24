// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components\Button.h>
#include <Components\EditableTextBox.h>
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"



UCLASS()
class UE11_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UButton* mSendBtn;
	UEditableTextBox* mInputText;
	class CPacketQueue* mQueue;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime);

public:
	UFUNCTION()
	void OnSendBtnClicked();
};
