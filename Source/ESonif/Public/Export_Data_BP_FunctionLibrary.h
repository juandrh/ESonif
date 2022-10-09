// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Export_Data_BP_FunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ESONIF_API UExport_Data_BP_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable,Category = "Custom", meta = (keywords = "save"))
	static bool SaveArrayText (FString SaveDirectory, FString Filename, TArray <FString> SaveText, bool AllowOverWrinting);
};
