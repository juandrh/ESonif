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
	float area = (pi * radius * radius);						   // Area covered by each particle
	area = (area * numAgents) / density;						   // Total area required
	float scale = FMath::Sqrt(UniverseSize * UniverseSize / area); // Scaled to area available

	r = radius * scale;	 // Radius in pixels
	r2 = radius * radius;		 // Radius squared
	v = r * gamma / 100; // Step size in pixels
	s = r * size / 100;	 // Particle size (circles)
	alphaRadians=(alpha/180)*pi;
  	betaRadians=(beta/180)*pi;

	for (int i = 0; i < numAgents; i++)
	{
		if (distribution == 0.0f)
		{
			// Uniform distribution
			InitLocation = FVector(
				(UniverseSize)*FMath::FRand() + 0.0f ,
				(UniverseSize)*FMath::FRand() + 0.0f ,
				20.0f);
		}
		else
		{
			// Centre-weighted distribution
			InitLocation = FVector(
				((FMath::FRand() + FMath::FRand() + FMath::FRand()) / 3) * UniverseSize ,
				((FMath::FRand() + FMath::FRand() + FMath::FRand()) / 3) * UniverseSize,
				20.0f);
		}
		InitRotation = FRotator(0.0f, 0.0f, 0.0f);
		Agents[i] = GetWorld()->SpawnActor<AAgent>(AgentClass, InitLocation, InitRotation, SpawnInfo);
	}

	
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

	if(ticksElapsed==0)	colorAgents();

	ticksElapsed++;
	if(ticksElapsed>20) ticksElapsed=0;
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
	r2 = radius * radius;		 // Radius squared
	v = r * gamma / 100; // Step size in pixels
	s = r * size / 100;	 // Particle size (circles)
}

void AUniverse::SetDistribution(float number)
{
	distribution = number;
}

void AUniverse::SetSize(float number)
{
	size = number;
	s = r * size / 100;	 // Particle size (circles)
}

void AUniverse::SetAlpha(float number)
{
	alpha = number;
	alphaRadians=(alpha/180)*pi;
  	betaRadians=(beta/180)*pi;
	
}

void AUniverse::SetBeta(float number)
{
	beta = number;
	alphaRadians=(alpha/180)*pi;
  	betaRadians=(beta/180)*pi;
}

void AUniverse::SetGamma(float number)
{
	
	gamma = number;
	v = r * gamma / 100; // Step size in pixels
	

}

void AUniverse::SetRadius(float number)
{
	radius = number;

	float area = (pi * radius * radius);						   // Area covered by each particle
	area = (area * numAgents) / density;						   // Total area required
	float scale = FMath::Sqrt(UniverseSize * UniverseSize / area); // Scaled to area available

	r = radius * scale;	 // Radius in pixels
	r2 = radius * radius;			 // Radius squared
	v = r * gamma / 100; // Step size in pixels
	s = r * size / 100;	 // Particle size (circles)
}


void AUniverse::countNeighbours()
{
	// Create new grid

	int xCells = FMath::Floor(UniverseSize / radius);
	int xSize = UniverseSize / xCells;
	TArray<int> grid[500][500];

	// Assign each particle to a grid cell
	for (int i = 0; i < numAgents; i++)
	{
		FVector location = Agents[i]->GetActorLocation();
		float xCoor=FMath::Floor((location.X) / xSize);
		float yCoor=FMath::Floor((location.Y ) / xSize);
		if(xCoor<0) xCoor=0;
		if(yCoor<0) yCoor=0;
		if(xCoor>=500) xCoor=499;
		if(yCoor>=500) yCoor=499;


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
		Agent = Cast<AAgent>(Agents[i]);
		float deltaphi = alphaRadians + (betaRadians * Agent->N * sign(Agent->R - Agent->L)); // deltaphi = alpha + beta × N × sign(R - L)
		//float deltaphi = alpha + (beta * Agent->N * sign(Agent->R - Agent->L));
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
    
  for (int i = 0; i < numAgents; i++){
	Agent = Cast<AAgent>(Agents[i]);
	
	proportionalNeib = 10*Agent->N/(float)(numAgents);
	float rc,gc,bc; 
	if (proportionalNeib<0.15){
		rc=  0;
		gc = 1- proportionalNeib*2.0f;
		bc= 0;
	}else if (proportionalNeib<0.35){
		rc=  proportionalNeib;
		gc = 1- proportionalNeib*4.0f;
		bc= 0;
	}else if (proportionalNeib<0.55){
		rc=  1-proportionalNeib*4.0f;;
		gc = 0;
		bc= proportionalNeib;;
	} else {
		rc=  1;
		gc = 1;
		bc= 1;
	}

	UE_LOG(LogTemp, Warning,TEXT("t->   : %f "), proportionalNeib);

	Agent->SetColor(FVector( rc, gc, bc));
	//Agent->SetColor(FVector(n*n/(maxN)+0.3,1-(n*n/maxN),n/maxN-0.5));
	
	

  }
}

FVector AUniverse::HSVToRGB( float H, float S, float V)
{
	float Min;
	float Chroma;
	float Hdash;
	float X;
	FVector RGB;
 
	Chroma = S * V;
	Hdash = H / 60.0;
	X = Chroma * (1.0 - FMath::Abs(((int)Hdash % 2) - 1.0));
 
	if(Hdash < 1.0)
	{
		RGB.X = Chroma;
		RGB.Y = X;
	}
	else if(Hdash < 2.0)
	{
		RGB.X = X;
		RGB.Y = Chroma;
	}
	else if(Hdash < 3.0)
	{
		RGB.Y = Chroma;
		RGB.Z = X;
	}
	else if(Hdash < 4.0)
	{
		RGB.Y= X;
		RGB.Z = Chroma;
	}
	else if(Hdash < 5.0)
	{
		RGB.X = X;
		RGB.Z= Chroma;
	}
	else if(Hdash <= 6.0)
	{
		RGB.X = Chroma;
		RGB.Z = X;
	}
 
	Min = V - Chroma;
 
	RGB.X += Min;
	RGB.Y += Min;
	RGB.Z += Min;
	
	
	return RGB;
}