// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "QRItemArray.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QRFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HOLODEMTECT_API UQRFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		static TMap<FString, UQRItemArray*> LoadJSON(FString filename);

	
};
