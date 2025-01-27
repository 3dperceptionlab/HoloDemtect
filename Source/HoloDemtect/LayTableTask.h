// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task.h"
#include "LayTableTask.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HOLODEMTECT_API ULayTableTask : public UTask
{
	GENERATED_BODY()

		TDoubleLinkedList<FString> table_elements;
	
public:
	ULayTableTask();

	void initialize(AGraspingObject* evaluation_point_, FTaskInfo taskInfo_) override;

	TArray<AGraspingObject*> evaluate() override;

	bool AreObjectsValid(TArray<AGraspingObject*> objs) override;

	TArray<FString> GetTaskItems(TArray<AGraspingObject*> objs) override;

	bool IsTaskFinished() override;
};
