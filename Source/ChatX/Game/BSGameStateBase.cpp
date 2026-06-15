// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BSGameStateBase.h"

#include "ChatX/BSChatX.h"

void ABSGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InPlayerName)
{
	const FString LoginMessage = FString::Printf(TEXT("%s joined the game."), *InPlayerName);
	BSChatXFunctionLibrary::MyPrintString(this, LoginMessage, 10.f);
}

void ABSGameStateBase::MulticastRPCBroadcastChatMessage_Implementation(const FString& InMessage)
{
	BSChatXFunctionLibrary::MyPrintString(this, InMessage, 10.f);
}

