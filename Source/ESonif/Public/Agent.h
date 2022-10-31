// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UCLASS()
class ESONIF_API AAgent : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties ,BlueprintReadWrite
	AAgent();
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Custom")
	UStaticMeshComponent* AgentMesh;

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
	
			

};
