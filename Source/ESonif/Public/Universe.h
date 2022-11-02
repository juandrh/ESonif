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
		
	//TArray<AActor*> Agents;
	AActor *Agents[10000];
	AAgent* Agent;
	UPROPERTY(EditAnywhere, Category = "General")
	int numAgents = 1000; 
	int numAgentsChanged = 500; 
	UPROPERTY(EditAnywhere, Category = "General")
	float density = 5.0F;
	UPROPERTY(EditAnywhere, Category = "General")
	float distribution = 0.0F;

	UPROPERTY(EditAnywhere, Category = "Graphics settings")
	float size= 10.0F;
	UPROPERTY(EditAnywhere, Category = "Graphics settings")
	float framerate= 15.0F;; 		
	UPROPERTY(EditAnywhere, Category = "Graphics settings")
	float tint= 90.0F;; 
	

	UPROPERTY(EditAnywhere, Category = "Species parameters")
	float alpha = 114.0F;
	UPROPERTY(EditAnywhere, Category = "Species parameterss")
	float beta = -4.0F;
	UPROPERTY(EditAnywhere, Category = "Species parameters")
	float gamma = 5.4F;
	UPROPERTY(EditAnywhere, Category = "Species parameters")
	float radius = 270.0F; 	


	float UniverseSize = 5000.0f;
	bool isRestarting = true;
	

	FVector InitLocation;
	FRotator InitRotation;
	float value; // auxiliar uses
	UPROPERTY(EditAnywhere, Category = "Custom")
	FVector impulse=FVector(0.0f,0.0f,0.0f);
	float pi = 3.1415926535897932384626433832795f; 
	float tau = 2* pi; 

	float r;                   // Radius in pixels
    float r2;                // Radius squared
    float v;         // Step size in pixels
    float s;        // Particle size

	float alphaRadians;
  	float betaRadians;
	float proportionalNeib;
    int ticksElapsed;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialization();
	void Update();
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: Set Impulse"))
	void SetImpulse(FVector newImpulse);
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: Get Impulse"))
	FVector GetImpulse();
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: Set Number of Agents"))
	void SetNumAgents(int32 number);
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: SetDensity"))
	void SetDensity(float number);
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: SetDistribution"))
	void SetDistribution(float number);
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: SetSize"))
	void SetSize(float number);
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: SetAlpha"))
	void SetAlpha(float number);	
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: SetBeta"))
	void SetBeta(float number);	
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: SetGamma"))
	void SetGamma(float number);	
	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: SetRadius"))
	void SetRadius(float number);
	

	UFUNCTION (BlueprintCallable,meta = (DisplayName = "ESonif: Restart Universe"))
	void restartUniverse();

	void countNeighbours();
	void scaleRadii();
	float scope(int val,int max); 
	float sign(float x);	
	void moveAgents();
	void colorAgents();
	FVector HSVToRGB( float H, float S, float V);

};