// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BSGameModeBase.h"

#include "Game/BSGameStateBase.h"
#include "Player/BSPlayerController.h"
#include "Player/BSPlayerState.h"

#include "EngineUtils.h"
#include "TimerManager.h"

ABSGameModeBase::ABSGameModeBase()
{
	GameStateClass = ABSGameStateBase::StaticClass();
	PlayerStateClass = ABSPlayerState::StaticClass();
}

void ABSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateAnswerNumber();
}

void ABSGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	APlayerState* PS = NewPlayer ? NewPlayer->PlayerState : nullptr;
	const FString PlayerName = IsValid(PS) ? PS->GetPlayerName() : TEXT("Unknown Player");

	ABSGameStateBase* BSGameStateBase = GetGameState<ABSGameStateBase>();
	if (IsValid(BSGameStateBase))
	{
		BSGameStateBase->MulticastRPCBroadcastLoginMessage(PlayerName);
	}
}

void ABSGameModeBase::GenerateAnswerNumber()
{
	TArray<int32> Numbers;

	while (Numbers.Num() < 3)
	{
		const int32 RandomNumber = FMath::RandRange(1, 9);

		if (Numbers.Contains(RandomNumber) == false)
		{
			Numbers.Add(RandomNumber);
		}
	}

	AnswerNumber = FString::Printf(TEXT("%d%d%d"), Numbers[0], Numbers[1], Numbers[2]);

	UE_LOG(LogTemp, Warning, TEXT("Number Baseball Answer : %s"), *AnswerNumber);
}

bool ABSGameModeBase::IsValidGuess(const FString& InGuess) const
{
	if (InGuess.Len() != 3)
	{
		return false;
	}

	TSet<TCHAR> UsedNumbers;

	for (TCHAR Ch : InGuess)
	{
		if (FChar::IsDigit(Ch) == false)
		{
			return false;
		}

		if (Ch == TEXT('0'))
		{
			return false;
		}

		if (UsedNumbers.Contains(Ch))
		{
			return false;
		}

		UsedNumbers.Add(Ch);
	}

	return true;
}

FString ABSGameModeBase::JudgeGuess(const FString& InGuess) const
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;

	for (int32 Index = 0; Index < 3; ++Index)
	{
		if (InGuess[Index] == AnswerNumber[Index])
		{
			++StrikeCount;
		}
		else if (AnswerNumber.Contains(FString::Chr(InGuess[Index])))
		{
			++BallCount;
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABSGameModeBase::ProcessChatMessage(ABSPlayerController* InPlayerController, const FString& InMessage)
{
	if (IsValid(InPlayerController) == false || bIsGameEnded)
	{
		return;
	}

	ABSPlayerState* BSPlayerState = InPlayerController->GetPlayerState<ABSPlayerState>();
	if (IsValid(BSPlayerState) == false)
	{
		return;
	}

	const FString Guess = InMessage.TrimStartAndEnd();

	if (BSPlayerState->IsOutOfChance())
	{
		InPlayerController->ClientRPCPrintChatMessageString(TEXT("이미 모든 기회를 사용했습니다."));
		return;
	}

	if (IsValidGuess(Guess) == false)
	{
		InPlayerController->ClientRPCPrintChatMessageString(TEXT("\uB2E4\uC2DC \uC785\uB825\uD558\uC138\uC694"));
		return;
	}

	BSPlayerState->AddTryCount();

	const FString PlayerName = BSPlayerState->GetPlayerName();
	const FString JudgeResult = JudgeGuess(Guess);
	const FString ResultMessage = FString::Printf(
		TEXT("%s : %s -> [%d / %d] %s"),
		*PlayerName,
		*Guess,
		BSPlayerState->GetCurrentTryCount(),
		BSPlayerState->GetMaxTryCount(),
		*JudgeResult
	);

	BroadcastMessage(ResultMessage);

	CheckGameResult(InPlayerController, JudgeResult);
}

void ABSGameModeBase::BroadcastMessage(const FString& InMessage)
{
	ABSGameStateBase* BSGameStateBase = GetGameState<ABSGameStateBase>();
	if (IsValid(BSGameStateBase))
	{
		BSGameStateBase->MulticastRPCBroadcastChatMessage(InMessage);
	}
}

void ABSGameModeBase::CheckGameResult(ABSPlayerController* InPlayerController, const FString& InJudgeResult)
{
	if (InJudgeResult == TEXT("3S0B"))
	{
		HandleWinner(InPlayerController);
		return;
	}

	CheckDraw();
}

void ABSGameModeBase::HandleWinner(ABSPlayerController* InWinnerController)
{
	if (IsValid(InWinnerController) == false || bIsGameEnded)
	{
		return;
	}

	bIsGameEnded = true;

	APlayerState* WinnerPlayerState = InWinnerController->PlayerState;
	const FString WinnerName = IsValid(WinnerPlayerState) ? WinnerPlayerState->GetPlayerName() : TEXT("Unknown Player");

	BroadcastMessage(FString::Printf(TEXT("===== Winner : %s ====="), *WinnerName));

	GetWorldTimerManager().SetTimer(
		ResetTimerHandle,
		this,
		&ABSGameModeBase::ResetGame,
		3.0f,
		false
	);
}

void ABSGameModeBase::CheckDraw()
{
	if (bIsGameEnded)
	{
		return;
	}

	bool bHasPlayer = false;
	bool bAllPlayersOutOfChance = true;

	for (TActorIterator<ABSPlayerController> It(GetWorld()); It; ++It)
	{
		ABSPlayerController* BSPlayerController = *It;
		if (IsValid(BSPlayerController) == false)
		{
			continue;
		}

		ABSPlayerState* BSPlayerState = BSPlayerController->GetPlayerState<ABSPlayerState>();
		if (IsValid(BSPlayerState) == false)
		{
			continue;
		}

		bHasPlayer = true;

		if (BSPlayerState->IsOutOfChance() == false)
		{
			bAllPlayersOutOfChance = false;
			break;
		}
	}

	if (bHasPlayer && bAllPlayersOutOfChance)
	{
		HandleDraw();
	}
}

void ABSGameModeBase::HandleDraw()
{
	if (bIsGameEnded)
	{
		return;
	}

	bIsGameEnded = true;
	BroadcastMessage(TEXT("===== Draw ====="));

	GetWorldTimerManager().SetTimer(
		ResetTimerHandle,
		this,
		&ABSGameModeBase::ResetGame,
		3.0f,
		false
	);
}

void ABSGameModeBase::ResetGame()
{
	for (TActorIterator<ABSPlayerController> It(GetWorld()); It; ++It)
	{
		ABSPlayerController* BSPlayerController = *It;
		if (IsValid(BSPlayerController) == false)
		{
			continue;
		}

		ABSPlayerState* BSPlayerState = BSPlayerController->GetPlayerState<ABSPlayerState>();
		if (IsValid(BSPlayerState))
		{
			BSPlayerState->ResetTryCount();
		}
	}

	bIsGameEnded = false;
	GenerateAnswerNumber();

	BroadcastMessage(TEXT("===== New Game Start ====="));
}

