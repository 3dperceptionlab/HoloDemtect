// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QRItem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HOLODEMTECT_API UQRItem : public UObject
{
	GENERATED_BODY()

public:
	UQRItem();

	UStaticMesh* mesh;

	FString meshName;

	FVector location;

	FRotator rotation;

	float scale;
	UFUNCTION(BlueprintCallable)
		void SetParams(FString meshName_, FVector loc, FRotator rot, float scale_);

};
