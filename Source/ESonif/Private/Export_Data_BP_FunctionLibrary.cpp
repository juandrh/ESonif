// Fill out your copyright notice in the Description page of Project Settings.

#include "Export_Data_BP_FunctionLibrary.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"


bool UExport_Data_BP_FunctionLibrary::SaveArrayText (FString SaveDirectory, FString Filename, TArray <FString> SaveText, bool AllowOverWrinting = false)
{
    SaveDirectory += "\\";
    SaveDirectory += Filename;

    if(!AllowOverWrinting)
    {
        if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)){
            return false;
        }
    }

    FString FinalString = "";

    for (FString& Each:SaveText)
    {
       FinalString += Each;
       FinalString += LINE_TERMINATOR; 
    }
    
    return FFileHelper::SaveStringToFile(FinalString ,*SaveDirectory );
}