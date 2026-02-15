#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowerDetector.generated.h"

enum class FlowerNPCMode : uint8;
class ADustyCharacter;
class AFlowerNPC;

UCLASS()
class DUSTY_API AFlowerDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlowerDetector();
	
	virtual void BeginPlay() override;
	void Init(ADustyCharacter* _player);
	
	float CheckForPlayer(float DetectionDistance);

#pragma region Setters / Getters
	
	void SetOrderedFlowers(const TArray<AFlowerNPC*>& _flowers);
	void SetFlowersMode(FlowerNPCMode _newMode);
	void SetReactionTime(float _reactionTime);

#pragma endregion
	
private:
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;

	TArray<AFlowerNPC*> m_Flowers;
	
	FVector m_TraceStartLocation = FVector::ZeroVector;
	FVector m_TargetLocation = FVector::ZeroVector;

	float m_ReactionTime = 0.f;

	UPROPERTY(Transient)
	FVector m_ClosestFlowerLocation = FVector::ZeroVector;

	UPROPERTY(Transient)
	FVector m_FarthestFlowerLocation = FVector::ZeroVector;

public: //for testing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	TArray<UMaterialInstance*> Materials;
	
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_TestBox = nullptr;

};