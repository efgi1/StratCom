// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "StratComPlayerState.generated.h"


class APlanet;
/**
 * 
 */
UCLASS()
class STRATCOM_API AStratComPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void InitializeHomePlanet(APlanet* NewHomePlanet);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void MarkPlanetAsExplored(APlanet* Planet);

	void DeselectCurrentlySelected();
	void SetSelected(APlanet* Planet);
	APlanet* GetSelected() const;
	APlanet* GetHomePlanet() const;
private:
	FName PlayerName;
	uint64 Gold = 0;
	uint64 TechLevel = 0;
	// Ship Clusters

	UPROPERTY(Replicated)
	TObjectPtr<APlanet> HomePlanet;
	UPROPERTY()
	TArray<TObjectPtr<APlanet>> ExploredPlanets;
	UPROPERTY()
	TObjectPtr<APlanet> SelectedPlanet = nullptr;

	
};
