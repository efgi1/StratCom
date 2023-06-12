// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlanetSpawner.generated.h"

class APlanet;

UCLASS()
class STRATCOM_API APlanetSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn();

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Planets | Home")
	TArray<FVector2D> HomePlanetLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Planets | General")
	TArray<FVector2D> PlanetLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Planets")
	TSubclassOf<APlanet> PlanetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UStaticMeshComponent> HomePlanetDecorator;
	

};
