#include "BTS_CheckForPlayer.h"
#include "AIController.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Kismet/GameplayStatics.h"


UBTS_CheckForPlayer::UBTS_CheckForPlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "CheckForPlayer";
	bNotifyBecomeRelevant = true;
	//bNotifyTick = true;
}

void UBTS_CheckForPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UBTS_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (const AAIController* EntityController = OwnerComp.GetAIOwner())
	{
		if (AMazeNPC* NPC = Cast<AMazeNPC>(EntityController->GetPawn()))
		{
			if (m_Player)
			{
				// set players visibility
				NPC->SetCanSee(DustyUtils::CanSeePlayer(NPC, m_Player, NPC->ValidDistance,0.25f));

			}
		}
	}
}