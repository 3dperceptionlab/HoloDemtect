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
	UQRItem() {};
	UQRItem(FString meshName_, float_t x, float_t y, float_t z, float_t rx, float_t ry, float_t rz, float_t scale_);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UStaticMesh* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString meshName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FVector location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FRotator rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float scale;
};
