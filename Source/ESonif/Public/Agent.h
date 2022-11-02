// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UCLASS()
class ESONIF_API AAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties ,BlueprintReadWrite
	AAgent();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Custom")
	UStaticMeshComponent* AgentMesh;

	float phi;
	float phiSin;
	float phiCos;
	int L=0; int R=0; int N=0;
	float pi = 3.1415926535897932384626433832795f; 
	float tau = 2* pi; 
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMaterialInterface* Material;
	UMaterialInstanceDynamic* DynMaterial;	
	TArray<USceneComponent*> MeshChildren;
	FVector Color;
	FVector Size;	
	FVector ForwardDirection;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Update();
	void SetColor(FVector value);
	FVector GetColor();
	void SetSize(FVector value);
	FVector GetSize();
	void SetForwardDirection(FVector value);
	FVector GetForwardDirection();
	void move(FVector value);
	void Rotate(FRotator rotation);

	
			

};
