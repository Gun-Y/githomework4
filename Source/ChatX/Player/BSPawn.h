#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BSPawn.generated.h"

UCLASS()
class CHATX_API ABSPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

};

