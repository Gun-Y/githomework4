// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BSPlayerState.generated.h"

UCLASS()
class CHATX_API ABSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABSPlayerState();

	void AddTryCount();

	void ResetTryCount();

	bool IsOutOfChance() const;

	int32 GetCurrentTryCount() const;

	int32 GetMaxTryCount() const;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentTryCount = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxTryCount = 3;
};

