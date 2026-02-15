#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowerOverrider.generated.h"

class ADustyCharacter;
class AFlowerController;


UENUM(BlueprintType)
enum class OverrideMode : uint8 {
	Activate			    UMETA(DisplayName = "Activate"),
	Deactivate		UMETA(DisplayName = "Deactivate"),
};


UCLASS()
class DUSTY_API AFlowerOverrider : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlowerOverrider();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	bool GetIsActive() const;
	void SetIsActive(bool _newIsActive);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OtherOverrider")
	AFlowerOverrider* m_OtherOverrider = nullptr;;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FlowerController")
	AFlowerController* m_FlowerController = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OverrideMode")
	OverrideMode m_OverrideMode = OverrideMode::Activate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsActive")
	bool m_IsActive = false;

	
private:
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;
	

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
