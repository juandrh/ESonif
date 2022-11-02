// Fill out your copyright notice in the Description page of Project Settings.

#include "Universe.h"
#include "Engine/Engine.h"
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

	static ConstructorHelpers::FObjectFinder<UClass> AgentBP(TEXT("Blueprint'/Game/BluePrints/BP_Agent.BP_Agent_C'"));
	if (AgentBP.Object)
	{
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
	
	if(!isRestarting) Update();
}

void AUniverse::Initialization()
{
	isRestarting = true;
	FActorSpawnParameters SpawnInfo;

	for (int i = 0; i < numAgents; i++)
	{
		
		if (distribution == 0.0f)
		{
			// Uniform distribution
			InitLocation = FVector(
				(UniverseSize)*FMath::FRand() + 0.0f ,
				(UniverseSize)*FMath::FRand() + 0.0f ,
				50.0f);
		}
		else
		{
			// Centre-weighted distribution
			//float mx = FMath::FRand() * UniverseSize / 2;
			InitLocation = FVector(
				((FMath::FRand() + FMath::FRand() + FMath::FRand()) / 3) * UniverseSize ,
				((FMath::FRand() + FMath::FRand() + FMath::FRand()) / 3) * UniverseSize,
				50.0f);
		}

		InitRotation = FRotator(0.0f, 0.0f, 0.0f);
		// InitRotation = FRotator(360*FMath::FRand(), 360*FMath::FRand(),0.0f );
		// Agents.Add(GetWorld()->SpawnActor<AAgent>(AgentClass, InitLocation, InitRotation, SpawnInfo));
		Agents[i] = GetWorld()->SpawnActor<AAgent>(AgentClass, InitLocation, InitRotation, SpawnInfo);
	}

	/* for (AActor* actor : Agents)
	{
		 Agent = Cast<AAgent>(actor);
		value = FMath::FRand();
		//

		//Agent->SetColor(FVector(FMath::FRand()*0.6+0.3,FMath::FRand()*0.1f,0.0f));
		value = FMath::FRand()*0.2+0.05;
		//Agent->SetSize(FVector(value,value,value));

	} */
	scaleRadii();
	isRestarting = false;
}

void AUniverse::restartUniverse()
{
	isRestarting = true;
	FString Msg;

	for (int i = 0; i < numAgents; i++)
	{
		// UE_LOG(LogTemp, Warning,TEXT("t->   %d : %f "), num);
		Agents[i]->Destroy();
		Agents[i] = NULL;
	}

	numAgents = numAgentsChanged;

	Initialization();
}
void AUniverse::Update()
{
	
		
	countNeighbours();
	moveAgents();
	//colorAgents();
}

void AUniverse::SetImpulse(FVector newImpulse)
{
	impulse = newImpulse;
}
FVector AUniverse::GetImpulse()
{
	return impulse;
}
void AUniverse::SetNumAgents(int32 number)
{
	numAgentsChanged = number;
}

int32 AUniverse::GetNumAgents()
{
	return numAgents;
}

float AUniverse::scope(int val, int max)
{
	val %= max;
	if (val < 0)
		val += max;
	return val;
}

float AUniverse::sign(float x)
{
	return x ? x < 0 ? -1.0f : 1.0f : 0.0f;
}

void AUniverse::SetDensity(float number)
{
	density = number;
	float area = (pi * radius * radius);						   // Area covered by each particle
	area = (area * numAgents) / density;						   // Total area required
	float scale = FMath::Sqrt(UniverseSize * UniverseSize / area); // Scaled to area available

	r = radius * scale;	 // Radius in pixels
	r2 = r * r;			 // Radius squared
	v = r * gamma / 100; // Step size in pixels
	s = r * size / 100;	 // Particle size (circles)
}
float AUniverse::GetDensity()
{
	return density;
}
void AUniverse::SetDistribution(float number)
{
	distribution = number;
}
float AUniverse::GetDistribution()
{
	return distribution;
}
void AUniverse::SetSize(float number)
{
	size = number;
	s = r * size / 100;	 // Particle size (circles)
}
float AUniverse::GetSize()
{
	return size;
}
void AUniverse::SetAlpha(float number)
{
	alpha = number;
	
}
float AUniverse::GetAlpha()
{
	return alpha;
}
void AUniverse::SetBeta(float number)
{
	beta = number;
}
float AUniverse::GetBeta()
{
	return beta;
}
void AUniverse::SetGamma(float number)
{
	
	gamma = number;
	v = r * gamma / 100; // Step size in pixels

}
float AUniverse::GetGamma()
{
	return gamma;
}
void AUniverse::SetRadius(float number)
{
	radius = number;
	float area = (pi * radius * radius);						   // Area covered by each particle
	area = (area * numAgents) / density;						   // Total area required
	float scale = FMath::Sqrt(UniverseSize * UniverseSize / area); // Scaled to area available

	r = radius * scale;	 // Radius in pixels
	r2 = r * r;			 // Radius squared
	v = r * gamma / 100; // Step size in pixels
	s = r * size / 100;	 // Particle size (circles)
}
float AUniverse::GetRadius()
{
	return radius;
}

void AUniverse::countNeighbours()
{
	// Create new grid

	int xCells = FMath::Floor(UniverseSize / radius);
	int xSize = UniverseSize / xCells;
	TArray<int> grid[800][800];

	// Assign each particle to a grid cell
	for (int i = 0; i < numAgents; i++)
	{
		FVector location = Agents[i]->GetActorLocation();
		float xCoor=FMath::Floor((location.X) / xSize);
		float yCoor=FMath::Floor((location.Y ) / xSize);
		if(xCoor<0) xCoor=0;
		if(yCoor<0) yCoor=0;
		if(xCoor>=800) xCoor=799;
		if(yCoor>=800) yCoor=799;


		grid[(int)(xCoor)][(int)(yCoor)].Add(i);
		Agent = Cast<AAgent>(Agents[i]);
		Agent->L = 0;
		Agent->R = 0;
		Agent->N = 0;
	}

	for (int gx = 0; gx < xCells; gx++)
	{ // Step through the grid cells (left to right)
		for (int gy = 0; gy < xCells; gy++)
		{ // Step through the grid cells (top to bottom)
			for (int i : grid[gx][gy])
			{ // Step through the particles in each grid cell
				// for (int gp=0; gp<grid[gx][gy].Num(); gp++) {		                     // Step through the particles in each grid cell
				//  int i=grid[gx][gy][gp];
				AAgent *Agenti = Cast<AAgent>(Agents[i]);
				FVector locationi = Agenti->GetActorLocation();

				// Now we have a particle index and we're in the right cell
				for (int cx = gx - 1; cx <= gx + 1; cx++)
				{ // Iterate through the 9 neighbouring cells (left to right)
					for (int cy = gy - 1; cy <= gy + 1; cy++)
					{														// Iterate through the 9 neighbouring cells (top to bottom)
						int sx = scope(cx, xCells), sy = scope(cy, xCells); // Wrap screen edges, Pac-Man style
						for (int j : grid[sx][sy])
						{
							// for (int sp=0; sp<grid[sx][sy].length; sp++) {                                 // Iterate through the particles in each cell
							//   int j=grid[sx][sy][sp];
							AAgent *Agentj = Cast<AAgent>(Agents[j]);
							FVector locationj = Agentj->GetActorLocation(); // Finally we have both particle indices
							if (i < j)
							{																									   // No need to check twice
								float sepX = scope(locationj.X - locationi.X + UniverseSize / 2, UniverseSize) - UniverseSize / 2; // Calculate x-axis separation
								float sepY = scope(locationj.Y - locationi.Y + UniverseSize / 2, UniverseSize) - UniverseSize / 2; // Calculate y-axis separation
								float sepD = (sepX * sepX) + (sepY * sepY);														   // Calculate the square of the separation distance
								if (sepD <= r2)
								{ // If separation distance is less than particle i radius
									if (sepX * Agenti->phiSin - sepY * Agenti->phiCos > 0)
										Agenti->L++; // Particle j is to the left of i
									else
										Agenti->R++; // Particle j is to the right of i
									Agenti->N++;
									if (sepX * Agentj->phiSin - sepY * Agentj->phiCos < 0)
										Agentj->L++; // Particle i is to the left of j
									else
										Agentj->R++; // Particle i is to the right of j
									Agentj->N++;	 // Increase total neighbour count for particle i
								}
							}
						}
					}
				}
			}
		}
	}
}

void AUniverse::scaleRadii()
{

	float area = (pi * radius * radius);						   // Area covered by each particle
	area = (area * numAgents) / density;						   // Total area required
	float scale = FMath::Sqrt(UniverseSize * UniverseSize / area); // Scaled to area available

	r = radius * scale;	 // Radius in pixels
	r2 = r * r;			 // Radius squared
	v = r * gamma / 100; // Step size in pixels
	s = r * size / 100;	 // Particle size (circles)
	alphaRadians=(alpha/180)*pi;
  	betaRadians=(beta/180)*pi;
	// species[i].opacity=Math.min(1, cw/(160*species[i].s));  // Bokeh effect for large particles
}

void AUniverse::moveAgents()
{
	
	v=r*gamma/100;
	
	for (int i = 0; i < numAgents; i++)
	{
		/*     let ps=species[i%species.length];
			let alpha=ps.alphaRadians;
			let beta=ps.betaRadians;
			let v=ps.v; */
  
		// Apply changes in orientation
		Agent = Cast<AAgent>(Agents[i]);
		float deltaphi = alphaRadians + (betaRadians * Agent->N * sign(Agent->R - Agent->L)); // deltaphi = alpha + beta × N × sign(R - L)
		Agent->phi = scope(Agent->phi + deltaphi, tau);							// Turn clockwise deltaphi
		Agent->phiSin = FMath::Sin(Agent->phi);
		Agent->phiCos = FMath::Cos(Agent->phi);

		// Move forward v and wrap screen edges, Pac-Man style

		FVector location = Agent->GetActorLocation();
		float xCoor=scope(location.X + (v * Agent->phiCos),  UniverseSize);
		float yCoor=scope(location.Y + (v * Agent->phiSin),  UniverseSize);
		if(xCoor<0) xCoor=0;
		if(yCoor<0) yCoor=0;
		Agent->SetActorLocation(FVector(xCoor,yCoor,20.0f));	
		
	}
}


void AUniverse::colorAgents() {
 /*  // Clear canvas (or leave trails)
  if (trails==0) context.fillStyle='#000';
  else context.fillStyle='rgba(0,0,0,'+(1-(trails/11))+')';
  context.fillRect(0, 0, cw, ch);
  
  // Group particles by species and number of neighbours
  let group=new Array();
  for (let i=0; i<species.length; i++) {
    group[i]=new Array();
    for (let j=i; j<number; j+=species.length) {
      if (group[i][particle[j].N]===undefined) group[i][particle[j].N]=new Array();
      group[i][particle[j].N].push(j);
    }
  }
  
  // Draw particle groups
  for (let i=0; i<species.length; i++) {
    for (let j=0; j<group[i].length; j++) {
      if (group[i][j]!==undefined) {
        context.beginPath();
        for (let k=0; k<group[i][j].length; k++) {
          let ip=particle[group[i][j][k]];
          if (shape=='circles') {
            // Draw a circle
            context.moveTo(ip.x, ip.y);
            context.arc(ip.x, ip.y, species[i].s, 0, tau);
          } else {
            // Draw a small square
            context.rect(ip.x-species[i].s, ip.y-species[i].s, species[i].s2, species[i].s2);
          }
        }
        context.fillStyle='hsla('+hsl(i,j)+','+species[i].opacity+')';
        context.fill();
      }
    }
  } */
}