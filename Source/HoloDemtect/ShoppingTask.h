// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task.h"
#include "Kismet/GameplayStatics.h"
#include "ShoppingTask.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HOLODEMTECT_API UShoppingTask : public UTask
{
	GENERATED_BODY()

	TDoubleLinkedList<FString> shopping_list;

public:
	UShoppingTask();

	void initialize(AGraspingObject* evaluation_point_, FTaskInfo taskInfo_) override;

	TArray<AGraspingObject*> evaluate() override;

	bool IsTaskFinished() override;

	bool AreObjectsValid(TArray<AGraspingObject*> objs) override;

	TArray<FString> GetTaskItems(TArray<AGraspingObject*> objs) override;
	
};
