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
	
public:
	ULayTableTask();

	void initialize() override;

	void evaluate() override;
};
