// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Transform.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AUniverse::AUniverse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> AgentBP (TEXT("Blueprint'/Game/BluePrints/BP_Agent.BP_Agent_C'")); 
		if (AgentBP.Object){
        AgentClass = AgentBP.Object;    
		}	

}

// Called when the game starts or when spawned
void AUniverse::BeginPlay()
{
	Super::BeginPlay();
	this->Initialization();
	
}

// Called every frame
void AUniverse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUniverse::Initialization()
{
	Agents.Empty();

	FActorSpawnParameters SpawnInfo; 
	for (int i=0; i < Num_Agents; i++ )
	{	
		InitLocation = FVector(
			(UniverseSize-400)*FMath::FRand()+200,
			(UniverseSize-400)*FMath::FRand()+200,
			1000.0f); 	
		InitRotation = FRotator(0.0f, 0.0f,0.0f );
		//InitRotation = FRotator(360*FMath::FRand(), 360*FMath::FRand(),0.0f );
		Agents.Add(GetWorld()->SpawnActor<AAgent>(AgentClass, InitLocation, InitRotation, SpawnInfo)); 
	}

	for (AActor* actor : Agents)	
	{
		/* Agent = Cast<AAgent>(actor);			
		value = FMath::FRand();	
		//UE_LOG(LogTemp, Warning,TEXT("V->   %f : "), PercentPredator);   
		
		//Agent->SetColor(FVector(FMath::FRand()*0.6+0.3,FMath::FRand()*0.1f,0.0f));		
		value = FMath::FRand()*0.2+0.05;
		//Agent->SetSize(FVector(value,value,value));	 */
		
	}


}
void AUniverse::Update()
{

}