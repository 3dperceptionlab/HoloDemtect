// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QRItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQRItem 
{
	GENERATED_BODY()

public:


	FQRItem();

	UStaticMesh* mesh;

	FString meshName;

	FVector location;

	FRotator rotation;

	float scale;

	void SetParams(FString meshName_, FVector loc, FRotator rot, float scale_);

};
