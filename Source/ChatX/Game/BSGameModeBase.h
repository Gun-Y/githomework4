// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BSGameModeBase.generated.h"

class ABSPlayerController;

UCLASS()
class CHATX_API ABSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABSGameModeBase();

	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	void ProcessChatMessage(ABSPlayerController* InPlayerController, const FString& InMessage);

protected:
	void GenerateAnswerNumber();

	bool IsValidGuess(const FString& InGuess) const;

	FString JudgeGuess(const FString& InGuess) const;

	void BroadcastMessage(const FString& InMessage);

	void CheckGameResult(ABSPlayerController* InPlayerController, const FString& InJudgeResult);

	void HandleWinner(ABSPlayerController* InWinnerController);

	void CheckDraw();

	void HandleDraw();

	void ResetGame();

protected:
	UPROPERTY()
	FString AnswerNumber;

	UPROPERTY()
	bool bIsGameEnded = false;

	FTimerHandle ResetTimerHandle;
};

