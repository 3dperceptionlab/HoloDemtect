// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpResponse.h"
#include "HTTPModule.h"
#include <string>
#include <chrono>
#include "Kismet/GameplayStatics.h"
#include "ImageUtils.h"
#include "Serialization/BufferArchive.h"
#include "DatasetGenerator.generated.h"

#define PSCREEN(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT(x));}
#define PSCREENE(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(x));}
#define SERVER_ENDPOINT "http://clarke:5000"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTimeseriesRow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FString time;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FString RightHandObject;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FString LeftHandObject;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector RightHandPos;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FRotator RightHandRot;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector LeftHandPos;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FRotator LeftHandRot;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector HeadPos;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FRotator HeadRot;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		int32 ObjectPut;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector EyeTrackerPos;

};

UCLASS(BlueprintType)
class HOLODEMTECT_API UDatasetGenerator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray< FTimeseriesRow> timeseries;

	int id=-1;
	FString task_type = "";
	FString task_name = "";
	FString total_time = "";

	UDatasetGenerator();
	~UDatasetGenerator();

	UFUNCTION(BlueprintCallable)
		bool sendTimeseries();
	UFUNCTION(BlueprintCallable)
		bool sendSummaryRow();
	UFUNCTION(BlueprintCallable)
		void setSummaryVariables(FString task_type_p, FString task_name_p, FString total_time_p) { task_type = task_type_p; task_name = task_name_p; total_time = total_time_p;}

	FString getTimeseriesString();
	FString getRowSummaryString();
	void OnResponseTimeseriesReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);
	void OnResponseSummaryReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

};
