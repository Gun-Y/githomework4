// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BSGameStateBase.generated.h"

UCLASS()
class CHATX_API ABSGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InPlayerName);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastChatMessage(const FString& InMessage);
};

