// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task.h"
#include "ShoppingTask.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HOLODEMTECT_API UShoppingTask : public UTask
{
	GENERATED_BODY()

		TQueue<FString> shopping_list;

public:
	UShoppingTask();

	void initialize(TArray<AGraspingObject*> SpawnedObjects_, AGraspingObject* evaluation_point_, FTaskInfo taskInfo_) override;

	TArray<AGraspingObject*> evaluate() override;
	
};
