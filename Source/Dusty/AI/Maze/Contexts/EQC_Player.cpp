#include "EQC_Player.h"

#include "Dusty/Character/DustyCharacter.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"


void UEQC_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);
	
	if (ADustyCharacter* const Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(Player));
	}
}

