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

public:
	UShoppingTask();

	void initialize() override;

	void eval() override;
	
};
