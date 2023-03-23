// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task.h"
#include "PairMatchingTask.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HOLODEMTECT_API UPairMatchingTask : public UTask
{
	GENERATED_BODY()
	
public:
	UPairMatchingTask();

	void initialize() override;

	void evaluate() override;
};
