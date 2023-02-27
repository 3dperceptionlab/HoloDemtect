// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QRItem.h"
#include "GraspingObject.h"
#include "UObject/NoExportTypes.h"
#include "QRItemDictionary.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HOLODEMTECT_API UQRItemDictionary : public UObject
{
	GENERATED_BODY()

	TMap<FString, TArray<UQRItem*>> items;

public:
	UQRItemDictionary();

	UFUNCTION(BlueprintCallable)
		static UQRItemDictionary* CreateInstance(FString filename);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		void SpawnQRItems(const UObject* WorldContextObject, FString key, FVector center, FRotator rotation, FVector extent);

	UFUNCTION(BlueprintPure)
		const bool KeyExists(FString key) { return items.Contains(key); }

};
