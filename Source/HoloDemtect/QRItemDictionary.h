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


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<AGraspingObject*> spawned_objects;

	TMap<FString, TArray<FQRItem>> items;


	UQRItemDictionary();

	UFUNCTION(BlueprintCallable)
		static UQRItemDictionary* CreateInstance(FString filename);

	UFUNCTION(BlueprintCallable)
	FString getItems() {
		//USED FOR DEBUGGING
		FString text = "Item names:";
		for(auto& i : items){
			for( auto &a : i.Value)
				text += a.meshName + ", ";
			text += "\n";
		}
		return(text); 
	}
	//get TArray item from key
	

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		void SpawnQRItems(const UObject* WorldContextObject, FString key, FVector center, FRotator rotation, FVector extent);

	UFUNCTION(BlueprintPure)
		const bool KeyExists(FString key) { return items.Contains(key); }

};
