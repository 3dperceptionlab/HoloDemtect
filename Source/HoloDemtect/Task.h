// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraspingObject.h"
#include "UObject/NoExportTypes.h"
#include "Task.generated.h"

/**
 * 
 **/

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

/**
*
*/
UCLASS(Abstract, BlueprintType)
class HOLODEMTECT_API UTask : public UObject
{
	GENERATED_BODY()

protected:
	TArray<AGraspingObject*> SpawnedObjects;

	FTaskInfo taskInfo;



public:
	UTask();

	UPROPERTY(BlueprintReadOnly, Category = "Data")
		FQRItem eval_point_info;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
		AGraspingObject* evaluation_point;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
		int32 totalErrors = 0;

	int32 currentErrors = 0;

	UFUNCTION(BlueprintCallable)
		virtual void initialize(AGraspingObject* evaluation_point_p, FTaskInfo taskInfo_);

	UFUNCTION(BlueprintCallable)
		virtual TArray<AGraspingObject*> evaluate();

	UFUNCTION(BlueprintPure)
		virtual bool AreObjectsValid(TArray<AGraspingObject*> objs);

	UFUNCTION(BlueprintPure)
		virtual TArray<FString> GetTaskItems(TArray<AGraspingObject*> objs);

	UFUNCTION(BlueprintPure)
		virtual bool IsTaskFinished();

	UFUNCTION(BlueprintPure)
		static UClass* GetTaskClassByName(const FString& name);

	UFUNCTION(BlueprintCallable)
		static TMap<FString, FTaskInfo> LoadTasksFromJSON(const FString& filename);
};