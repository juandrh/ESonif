// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Agent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Universe.generated.h"

UCLASS()
class ESONIF_API AUniverse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUniverse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TSubclassOf<class AAgent> AgentClass;		
	TArray<AActor*> Agents;	
	AAgent* Agent;
	UPROPERTY(EditAnywhere, Category = "Custom")
	int Num_Agents = 500; 
	float UniverseSize = 1000.0f;

	FVector InitLocation;
	FRotator InitRotation;
	float value; // auxiliar uses
	UPROPERTY(EditAnywhere, Category = "Custom")
	FVector Impulse=FVector(0.0f,200.0f,0.0f);




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialization();
	void Update();

};
