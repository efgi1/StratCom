// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetSpawner.h"

#include "EngineUtils.h"
#include "Planet.h"
#include "Kismet/KismetMathLibrary.h"
#include "StratCom/Player/StratComPlayerController.h"
#include "StratCom/Player/StratComPlayerState.h"

// Sets default values
APlanetSpawner::APlanetSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlanetSpawner::BeginPlay()
{
	ensure(PlanetClass);

	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &APlanetSpawner::Spawn, 3.f, false);
	
}

void APlanetSpawner::Spawn()
{
	if (HasAuthority())
	{
		size_t Index = 0;
		FVector Min,Max;
		for (AStratComPlayerController* Controller : TActorRange<AStratComPlayerController>(GetWorld()))
		{
		    AStratComPlayerState* PlayerState = Controller->GetPlayerState<AStratComPlayerState>();
			if (Index < HomePlanetLocations.Num())
			{
				FVector Location = {3000, HomePlanetLocations[Index].X, HomePlanetLocations[Index].Y};
				APlanet* NewHomePlanet = GetWorld()->SpawnActor<APlanet>(PlanetClass, Location, FRotator::ZeroRotator);
				
				PlayerState->InitializeHomePlanet(NewHomePlanet);
				NewHomePlanet->PlanetName = FString("HOME ") + FString::FromInt(Index);
				if (Index == 0)
					Min = NewHomePlanet->GetActorLocation();
				else
					Max = NewHomePlanet->GetActorLocation();
				Index++;
			}
		}
		FRandomStream RandStream;
		UKismetMathLibrary::SetRandomStreamSeed(RandStream, 0);
		double MinY, MinZ;
		for (int i= 0; i < 20; ++i)
		{
			MinY = RandStream.RandRange(Min.Y + 100, Max.Y - 100);;
			MinZ = RandStream.RandRange(Min.Z + 100, Max.Z - 100);
			FVector Location{3000, MinY, MinZ};
			GetWorld()->SpawnActor<APlanet>(PlanetClass, Location, FRotator::ZeroRotator);
		}

		/*for (FVector2D& Location2D : PlanetLocations)
		{
			FVector Location{3000, Location2D.X, Location2D.Y};
			GetWorld()->SpawnActor<APlanet>(PlanetClass, Location, FRotator::ZeroRotator);
		}*/
	}
	APlanet* Planet = GetWorld()->GetFirstPlayerController()->GetPlayerState<AStratComPlayerState>()->GetHomePlanet();
	UStaticMeshComponent* Test = Cast<UStaticMeshComponent>(Planet->AddComponentByClass(HomePlanetDecorator, false, FTransform::Identity, false));
	Test->SetVisibility(true);
	Test->SetWorldScale3D(Planet->GetPlanetScale());
	Test->Activate();
}



