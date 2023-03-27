// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraspingObject.h"
#include "UObject/NoExportTypes.h"
#include "Task.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTaskInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
		FString text;
	UPROPERTY(BlueprintReadOnly, Category = "Data")
		FString className;
	UPROPERTY(BlueprintReadOnly, Category = "Data")
		TArray<FString> items;

	void onClickedButton();
};

UCLASS(Abstract, BlueprintType)
class HOLODEMTECT_API UTask : public UObject
{
	GENERATED_BODY()

public:
	UTask();

	TArray<AGraspingObject*> *SpawnedObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
		UQRItem* eval_point_info;


	UFUNCTION(BlueprintCallable)
		virtual void initialize();

	UFUNCTION(BlueprintCallable)
		virtual void evaluate();

	UFUNCTION(BlueprintPure)
		static UClass* GetTaskClassByName(const FString& name);

	UFUNCTION(BlueprintCallable)
		static TMap<FString, FTaskInfo> LoadTasksFromJSON(const FString& filename);
};