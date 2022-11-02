// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"
#include "UtilsLib.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/UnrealMathVectorCommon.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	
	AgentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Agent Mesh"));
	AgentMesh->SetSimulatePhysics(true);
	RootComponent = AgentMesh;

	     
 /*  	phiSin = FMath::Asin(phi);  
 	phiCos = FMath::Acos(phi);  */

}


// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();
	AgentMesh->GetChildrenComponents(false,MeshChildren);	
	Material = AgentMesh->GetMaterial(0);
	DynMaterial = UMaterialInstanceDynamic::Create(Material, NULL);	
	SetColor(FVector(0.5,0.1,0.8));
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAgent::Update()
{

}
void AAgent::SetColor(FVector value)
{
	Color = value;
	DynMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector(Color)));
	AgentMesh->SetMaterial(0, DynMaterial); 	
	
}
FVector AAgent::GetColor()
{
	return Color;
}
void AAgent::SetSize(FVector value)
{
	Size = value;
}
FVector AAgent::GetSize()
{
	return Size;
}
void AAgent::SetForwardDirection(FVector value)
{
	ForwardDirection = value;
}
FVector AAgent::GetForwardDirection()
{
	return ForwardDirection;
}
void AAgent::move(FVector value)
{
	FVector Forward = value;
	UStaticMeshComponent* RootMeshComponent = Cast<UStaticMeshComponent>(this->GetRootComponent());
	//RootMeshComponent->GetChildrenComponents(false,MeshChildren);	

	RootMeshComponent->AddImpulse(Forward );
}


void AAgent::Rotate(FRotator rotation)    
{
	//MeshRotator = FRotator (0.0f, Velocity.Rotation().Yaw,0.0f);	
	this->SetActorRotation(rotation)	;			
		
}