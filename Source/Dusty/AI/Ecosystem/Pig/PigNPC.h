#pragma once

#include "CoreMinimal.h"
#include "PigNPCArea.h"
#include "GameFramework/Character.h"
#include "PigNPC.generated.h"

class APigNPCAreaController;
enum class AreaType : uint8;
class APigNPCArea;
class APigAIController;
class USoundBase;

UENUM(BlueprintType)
enum class PigNPCMode : uint8 {
	Walking		UMETA(DisplayName = "Walking"),
	Targeting	UMETA(DisplayName = "Targeting"),
	Herding		UMETA(DisplayName = "Herding"),
	Digging		UMETA(DisplayName = "Digging"),
	Fleeing		UMETA(DisplayName = "Fleeing")
};

UENUM(BlueprintType)
enum class PigTargets : uint8 {
	Earth		UMETA(DisplayName = "Earth"),
	Plant	UMETA(DisplayName = "Plant"),
	Fire		UMETA(DisplayName = "Fire"),
	Water		UMETA(DisplayName = "Water"),
	All		UMETA(DisplayName = "All")
};

	
UCLASS()
class DUSTY_API APigNPC : public ACharacter
{
	GENERATED_BODY()

public:
	APigNPC();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetCurrentState(PigNPCMode _newState);
	void CleanArea();
	
	PigNPCMode GetCurrentState() const;

#pragma region Setters / Getters
	
	float GetDetectionDistance() const;
	void SetDetectionDistance(float _newDetectionDistance);

#pragma endregion

protected:
	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "AreaController")
	APigNPCAreaController* m_AreaController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TimeHerding")
	float m_TimeHerding = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "RandomVariationTimeHerding")
	float m_RandomVariationTimeHerding = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TimeDigging")
	float m_TimeDigging = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "RandomVariationTimeDigging")
	float m_RandomVariationTimeDigging = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TimeWalking")
	float m_TimeWalking = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "RandomVariationTimeWalking")
	float m_RandomVariationTimeWalking = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DetectionDistance")
	float m_DetectionDistance = 250.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ChillSpeed")
	float m_ChillSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FleeSpeed")
	float m_FleeSpeed= 500.f;
	
private:
	UPROPERTY(Transient)
	APigAIController* m_PigAIController = nullptr;
	
	float m_ControlTimer = 0.f;
	float m_MontageTimer = 0.f;

	bool m_MontageCalled = false;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Design")
	PigNPCMode CurrentState = PigNPCMode::Herding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Design")
	PigTargets AreasTypesToTarget = PigTargets::All;
	
	AreaType CurrentSubState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", DisplayName = "Montage")
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "AcceptanceDistance")
	float AcceptanceDistance = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "AccelerationRate")
	float AccelerationRate = 50.f;

	UPROPERTY(Transient)
	APigNPCArea* CurrentTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "PigSound")
	USoundBase* m_sound;
	
	
};
