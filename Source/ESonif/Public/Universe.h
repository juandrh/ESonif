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
	int numAgents = 10; 
	float UniverseSize = 1000.0f;

	FVector InitLocation;
	FRotator InitRotation;
	float value; // auxiliar uses
	UPROPERTY(EditAnywhere, Category = "Custom")
	FVector impulse=FVector(0.0f,0.0f,0.0f);




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialization();
	void Update();
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "Set Impulse"))
	void SetImpulse(FVector newImpulse);
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "Get Impulse"))
	FVector GetImpulse();
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "Set Number of Agents"))
	void SetNumAgents(int32 number);
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "Get Number of Agents"))
	int32 GetNumAgents();
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "Restart Universe"))
	void restartUniverse();

};
