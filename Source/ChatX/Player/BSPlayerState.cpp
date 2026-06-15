// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BSPlayerState.h"

ABSPlayerState::ABSPlayerState()
{
	CurrentTryCount = 0;
	MaxTryCount = 3;
}

void ABSPlayerState::AddTryCount()
{
	if (CurrentTryCount < MaxTryCount)
	{
		++CurrentTryCount;
	}
}

void ABSPlayerState::ResetTryCount()
{
	CurrentTryCount = 0;
}

bool ABSPlayerState::IsOutOfChance() const
{
	return CurrentTryCount >= MaxTryCount;
}

int32 ABSPlayerState::GetCurrentTryCount() const
{
	return CurrentTryCount;
}

int32 ABSPlayerState::GetMaxTryCount() const
{
	return MaxTryCount;
}

