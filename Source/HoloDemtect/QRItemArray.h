// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QRItem.h"
#include "UObject/NoExportTypes.h"
#include "QRItemArray.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HOLODEMTECT_API UQRItemArray : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<UQRItem*> items;
};
