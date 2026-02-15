#include "BTS_CanSeePlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Utils/DustyUtils.h"


UBTS_CanSeePlayer::UBTS_CanSeePlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "CheckForPlayer";
	bNotifyBecomeRelevant = true;
	//bNotifyTick = true;
}

void UBTS_CanSeePlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_EntityController = OwnerComp.GetAIOwner();

	if (m_EntityController)
	{
		m_NPC = Cast<AMazeNPC>(m_EntityController->GetPawn());
	}
}

void UBTS_CanSeePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (m_Player && m_NPC && m_EntityController)
	{
		// get players visibility
		bool const CanSeePlayer = DustyUtils::CanSeePlayer(m_NPC, m_Player, m_NPC->ValidDistance,0.25f);
		bool const TooClose = FVector::Dist(m_Player->GetActorLocation(), m_NPC->GetActorLocation()) < MinDistanceForOverride;
		
		if (TooClose)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
		}
		else
		{
			// set is on wait based on can see value
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), CanSeePlayer);
		}

		// set can see so BT knows if its doing visibility wait control or distance wait control
		m_NPC->SetCanSee(CanSeePlayer);
		
		if ((CanSeePlayer || TooClose) && CanOverrideMovement) // if can see player then stop for player
		{
			m_NPC->SetCanMove(false);
		}
	}
}
