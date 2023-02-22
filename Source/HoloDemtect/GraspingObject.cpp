// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspingObject.h"
#include <string>

// Sets default values
AGraspingObject::AGraspingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));

	this->className = "DefaultMesh";
	// TODO: Parametrizar esto con qr_text
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_object(TEXT("StaticMesh'/Game/YCB/16K/011_banana/011_banana'"));
	std::string nameStr = std::string(TCHAR_TO_UTF8(*(this->className)));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	node->SetStaticMesh(mesh_object.Object);
	//node->SetRelativeScale3D(FVector(0.05, 0.05, 0.05));
	node->SetRelativeRotation(FRotator(-90, 0, 0));
	node->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AGraspingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGraspingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AGraspingObject* AGraspingObject::SpawnGraspingObject(const UObject* WorldContextObject, FString qr_text_)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	if (world == nullptr)
		return nullptr;

	AGraspingObject* grasping_object = world->SpawnActor<AGraspingObject>(SpawnInfo);
	grasping_object->qr_text = qr_text_;
	return grasping_object;
}

void AGraspingObject::SetActorPosition(FVector center, FVector extent, FRotator rotation)
{
	SetActorLocation(center);
	SetActorRotation(rotation);
	float scale = (extent.Y + extent.Z) / 50;
	SetActorScale3D(FVector(scale, scale, scale));

}

