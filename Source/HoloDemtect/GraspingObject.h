// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString qr_text;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	//	FGuid qr_guid;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	//	int32 qr_version;

	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UStaticMeshComponent* node;
};
