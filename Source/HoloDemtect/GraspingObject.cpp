// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspingObject.h"

// Sets default values
AGraspingObject::AGraspingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));

	this->className = "DefaultMesh";
	std::string nameStr = std::string(TCHAR_TO_UTF8(*(this->className)));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
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

AGraspingObject* AGraspingObject::SpawnGraspingObject(const UObject* WorldContextObject, FVector center, FVector extent, FRotator rotation, UQRItem *item)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	if (world == nullptr)
		return nullptr;

	AGraspingObject* grasping_object = world->SpawnActor<AGraspingObject>(SpawnInfo);
	grasping_object->node->SetStaticMesh(item->mesh);

	// Global Location/Rotation (extracted from QR)
	grasping_object->SetActorLocation(center + item->location);
	grasping_object->SetActorRotation(rotation + item->rotation);

	// Scale
	//float scale = (extent.Y + extent.Z) / 50; // Calculated from QR
	grasping_object->SetActorScale3D(FVector(item->scale, item->scale, item->scale)); // Different for each object

	// Not working code
	// grasping_object->node->SetRelativeLocation(item->location);
	// grasping_object->node->SetRelativeRotation(item->rotation);

	return grasping_object;
}

