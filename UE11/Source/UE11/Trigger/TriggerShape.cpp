// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerShape.h"

// Sets default values
ATriggerShape::ATriggerShape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �Ϲ� �� ������Ʈ�� ��Ʈ�� ��Ƴ��� ���ϵ�� �浹ü ������Ʈ�� ��Ƴ��� ������ �����Ѵ�.
	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	mShape = ETriggerShape::Box;
	
	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mBox->SetupAttachment(mRoot);

	mStart = false;
}

void ATriggerShape::ChangeShape(ETriggerShape Shape)
{
	// ���� ���� �ٲ� ����� ���ٸ� ����� �ٲ��� �ʿ䰡 ����
	if (mShape == Shape)
		return;

	// �ٲ��� ������� ��ü�� �������ְ�, root�� �ٿ��ش�.
	switch (Shape)
	{
	case ETriggerShape::Box:
		mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());
		mBox->SetupAttachment(mRoot);
		break;
	case ETriggerShape::Sphere:
		mSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
		mSphere->SetupAttachment(mRoot);
		break;
	case ETriggerShape::Capsule:
		mCapsule = NewObject<UCapsuleComponent>(this, UBoxComponent::StaticClass());
		mCapsule->SetupAttachment(mRoot);
		break;
	}

	// ���� �ִ� ����� root���� �����ְ� �����ش�.
	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox->DetachFromParent();
		mBox->DestroyComponent();
		mBox = nullptr;
		break;
	case ETriggerShape::Sphere:
		mSphere->DetachFromParent();
		mSphere->DestroyComponent();
		mSphere = nullptr;
		break;
	case ETriggerShape::Capsule:
		mCapsule->DetachFromParent();
		mCapsule->DestroyComponent();
		mCapsule = nullptr;
		break;
	}

	// ��������� �ٲ� ����� ��� �ִ´�.
	mShape = Shape;

	if (mStart)
	{
		switch (mShape)
		{
		case ETriggerShape::Box:
			mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
			mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
			break;
		case ETriggerShape::Sphere:
			mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
			mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
			break;
		case ETriggerShape::Capsule:
			mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
			mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
			break;
		}
	}
}

void ATriggerShape::ChangeCollisionProfile(const FString& Name)
{
	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox->SetCollisionProfileName(*Name);
		break;
	case ETriggerShape::Sphere:
		mSphere->SetCollisionProfileName(*Name);
		break;
	case ETriggerShape::Capsule:
		mCapsule->SetCollisionProfileName(*Name);
		break;
	}
}

void ATriggerShape::EnableCollision(bool Enable)
{
	switch (mShape)
	{
	case ETriggerShape::Box:
		if (!Enable)
			mBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		else
			mBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ETriggerShape::Sphere:
		if (!Enable)
			mSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		else
			mSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ETriggerShape::Capsule:
		if (!Enable)
			mCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		else
			mCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

// Called when the game starts or when spawned
void ATriggerShape::BeginPlay()
{
	Super::BeginPlay();
	
	mStart = true;

	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Sphere:
		mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Capsule:
		mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	}
}

// Called every frame
void ATriggerShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerShape::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerBegin(SweepResult);
}

void ATriggerShape::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TriggerEnd();
}

void ATriggerShape::TriggerBegin(const FHitResult& SweepResult)
{
}

void ATriggerShape::TriggerEnd()
{
}

