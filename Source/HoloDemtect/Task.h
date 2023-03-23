// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Task.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class HOLODEMTECT_API UTask : public UObject
{
	GENERATED_BODY()

public:

	UTask();

	virtual void initialize();

	virtual void eval();

	
};
