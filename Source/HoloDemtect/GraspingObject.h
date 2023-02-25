// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "QRItem.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraspingObject.generated.h"

UCLASS()
class HOLODEMTECT_API AGraspingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraspingObject();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString className;

	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UStaticMeshComponent* node;

	//UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static AGraspingObject* SpawnGraspingObject(const UObject* WorldContextObject, FVector center, FVector extent, FRotator rotation, const UQRItem& item);
};
