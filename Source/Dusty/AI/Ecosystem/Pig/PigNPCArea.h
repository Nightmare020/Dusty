#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PigNPCArea.generated.h"

UENUM(BlueprintType)
enum class AreaType : uint8 {
	Dig			    UMETA(DisplayName = "Digg"),
	Eat		UMETA(DisplayName = "Eat"),
};

UCLASS()
class DUSTY_API APigNPCArea : public AActor
{
private:
	GENERATED_BODY()
	
public:	
	APigNPCArea();
	virtual void BeginPlay() override;

	bool IsFree() const;
	void SetIsFree(bool _isFree);
	
	AreaType GetType() const;

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	bool m_IsFree = true;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger;
	
	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "AreaType")
	AreaType m_AreaType = AreaType::Dig;

};
