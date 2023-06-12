// Fill out your copyright notice in the Description page of Project Settings.


#include "StratComPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "StratCom/Gameplay/Planet.h"


void AStratComPlayerState::InitializeHomePlanet(APlanet* NewHomePlanet)
{
	HomePlanet = NewHomePlanet;
	HomePlanet->SetMaxSlots(8);
	HomePlanet->SetGrade(Rich);
}

void AStratComPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStratComPlayerState, HomePlanet);
}

void AStratComPlayerState::MarkPlanetAsExplored(APlanet* Planet)
{
	ExploredPlanets.AddUnique(Planet);
}

void AStratComPlayerState::DeselectCurrentlySelected()
{
	if (SelectedPlanet)
	{
		SelectedPlanet->HandleDeselect();
		SelectedPlanet = nullptr;
	}
}

void AStratComPlayerState::SetSelected(APlanet* Planet)
{
	if (SelectedPlanet != Planet)
	{
		DeselectCurrentlySelected();
	}
	SelectedPlanet = Planet;
	SelectedPlanet->HandleSelect();
}

APlanet* AStratComPlayerState::GetSelected() const
{
	return SelectedPlanet;
}

APlanet* AStratComPlayerState::GetHomePlanet() const
{
	return HomePlanet;
}
