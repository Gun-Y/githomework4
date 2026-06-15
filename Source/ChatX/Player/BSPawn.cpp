// BSPawn.cpp


#include "BSPawn.h"
#include "BSChatX.h"


void ABSPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = BSChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("BSPawn::BeginPlay() %s [%s]"), *BSChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	BSChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ABSPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = BSChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("BSPawn::PossessedBy() %s [%s]"), *BSChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	BSChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

