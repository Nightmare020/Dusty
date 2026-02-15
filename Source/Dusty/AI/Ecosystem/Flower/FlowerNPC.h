#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowerNPC.generated.h"

class UCapsuleComponent;

UENUM(BlueprintType)
enum class FlowerNPCMode : uint8 {
	Hiding			    UMETA(DisplayName = "Hiding"),
	Looking		UMETA(DisplayName = "Looking")
};


UCLASS()
class DUSTY_API AFlowerNPC : public AActor
{
private:
	GENERATED_BODY()
	
public:	
	AFlowerNPC();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

#pragma region Setters / Getters
	
	FlowerNPCMode GetMode() const;
	void SetMode(FlowerNPCMode _newMode, float _time);
	
#pragma endregion

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "SkeletalMesh")
	USkeletalMeshComponent* m_SkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Capsule")
	UCapsuleComponent* m_Capsule = nullptr;

private:

	float m_Timer = 0.f;
	FlowerNPCMode TempMode = FlowerNPCMode::Looking;

	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Design")
	FlowerNPCMode FlowerMode = FlowerNPCMode::Hiding;
	
};
