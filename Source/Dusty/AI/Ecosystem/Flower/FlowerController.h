#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowerController.generated.h"

class AFlowerNPC;
class AFlowerDetector;
class ADustyCharacter;

UCLASS()
class DUSTY_API AFlowerController : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlowerController();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Init(ADustyCharacter* _player);

private:
	void CheckDetectors();

	bool RefChecker();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CheckingRate")
	float m_CheckingRate = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CoolDown")
	float m_CoolDown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MaxDetectionDistance")
	float m_DistanceToActivate = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DistanceFactor")
	float m_DistanceFactor = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MaxReactionTime")
	float m_MaxReactionTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FlowerRotationSpeed")
	float m_FlowerRotationSpeed = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Flowers")
	TArray<AFlowerNPC*> m_Flowers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Detectors")
	TArray<AFlowerDetector*> m_Detectors;

private:
	UPROPERTY(Transient)
	AFlowerDetector* DetectionResponsible = nullptr;

	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;
	
	float m_CheckingRateTimer = 0.f;
	float m_CoolDownTimer = 0.f;

	float m_CurrentDistanceToActivate = 0.f;
	
};
