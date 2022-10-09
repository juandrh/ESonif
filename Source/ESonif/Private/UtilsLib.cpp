// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilsLib.h"
#include <random>
#include <cmath>
#include "Math/UnrealMathUtility.h"


UtilsLib::UtilsLib()
{
}

UtilsLib::~UtilsLib()
{
}

float UtilsLib::NormalDistGen(float mu,float sigma )
{
    std::random_device rd{};
    std::mt19937 gen{ rd() };
    std::normal_distribution<float> d{ mu, sigma };
    return d(gen); 

}

/* Debug Helpers
UE_LOG(LogTemp, Warning,TEXT("V->   %f : "), value);  
GEngine->AddOnScreenDebugMessage(1,100.0f, FColor::Black, FString::FPrintf(TEXT("V->   %f : "), value));

DrawDebugLine( 
#include "DrawDebugHelpers.h"
DrawDebugSphere( GetWorld(),GetActorLocation()+FVector(0.0f,0.0f,200.0f), 20.0f, 20, FColor::Orange,true,100.0f,-1.0f,2.0f);
			
*/ 