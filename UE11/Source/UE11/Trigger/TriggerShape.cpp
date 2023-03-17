// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerShape.h"

// Sets default values
ATriggerShape::ATriggerShape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 일반 씬 컴포넌트로 루트를 잡아놓고 차일드로 충돌체 컴포넌트를 잡아놓는 식으로 설계한다.
	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	mShape = ETriggerShape::Box;
	
	//mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	//mBox->SetupAttachment(mRoot);
	//mBox->SetCollisionProfileName(TEXT("PlayerTrigger"));
}

void ATriggerShape::ChangeShape(ETriggerShape Shape)
{
	// 현재 모양과 바꿀 모양이 같다면 모양을 바꿔줄 필요가 없다
	if (mShape == Shape)
		return;

	if (mBox)
	{
		mBox->DetachFromParent();
		mBox->DestroyComponent();
		mBox = nullptr;
	}

	else if (mSphere)
	{
		mSphere->DetachFromParent();
		mSphere->DestroyComponent();
		mSphere = nullptr;
	}

	else if (mCapsule)
	{
		mCapsule->DetachFromParent();
		mCapsule->DestroyComponent();
		mCapsule = nullptr;
	}

	switch (Shape)
	{
	case ETriggerShape::Box:
		mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Box"));
		mBox->RegisterComponent();
		mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Sphere:
		mSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Sphere"));
		mSphere->RegisterComponent();
		mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Capsule:
		mCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("Capsule"));
		mCapsule->RegisterComponent();
		mCapsule->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	}

	mShape = Shape;
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

// 블루프린트의 Construction과 같은 역할을 한다.
// 에디터에서 바뀌자마자 호출된다. (위치를 바꿀때도 호출된다)
// 실시간으로 갱신되어야 할 때 사용한다.
void ATriggerShape::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (mBox && mShape == ETriggerShape::Box)
		return;

	else if (mSphere && mShape == ETriggerShape::Sphere)
		return;

	else if (mCapsule && mShape == ETriggerShape::Capsule)
		return;

	if (mBox)
	{
		mBox->DetachFromParent();
		mBox->DestroyComponent();
		mBox = nullptr;
	}

	else if (mSphere)
	{
		mSphere->DetachFromParent();
		mSphere->DestroyComponent();
		mSphere = nullptr;
	}

	else if (mCapsule)
	{
		mCapsule->DetachFromParent();
		mCapsule->DestroyComponent();
		mCapsule = nullptr;
	}

	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Box"));
		mBox->RegisterComponent();
		mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Sphere:
		mSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Sphere"));
		mSphere->RegisterComponent();
		mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Capsule:
		mCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("Capsule"));
		mCapsule->RegisterComponent();
		mCapsule->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	}

	ChangeCollisionProfile(TEXT("PlayerTrigger"));
}

// Called when the game starts or when spawned
void ATriggerShape::BeginPlay()
{
	Super::BeginPlay();
	
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

