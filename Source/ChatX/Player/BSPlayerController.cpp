// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BSPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "UI/BSUserWidget.h"
#include "ChatX/BSChatX.h"
#include "Game/BSGameModeBase.h"

void ABSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass))
	{
		ChatInputWidgetInstance = CreateWidget<UBSUserWidget>(this, ChatInputWidgetClass);

		if (IsValid(ChatInputWidgetInstance))
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void ABSPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController())
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ABSPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	BSChatXFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ABSPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ABSPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	ABSGameModeBase* BSGameModeBase = GetWorld()->GetAuthGameMode<ABSGameModeBase>();
	if (IsValid(BSGameModeBase) == false)
	{
		return;
	}

	BSGameModeBase->ProcessChatMessage(this, InChatMessageString);
}


