#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DustyLookAtTrigger.generated.h"

class UShapeComponent;
class ADustyCharacter;
class UDustyAnimInstance;

UCLASS()
class DUSTY_API ADustyLookAtTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ADustyLookAtTrigger();

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:	
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "Trigger data")
	UShapeComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = "Trigger data", meta = (MakeEditWidget = true))
	FTransform TransformToLook;

	UPROPERTY(EditAnywhere, Category = "Trigger data", meta = (MakeEditWidget = true))
	AActor* ActorToLook;

private:
	UPROPERTY(Transient)
	ADustyCharacter* m_DustyCharacter;

	UPROPERTY(Transient)
	UDustyAnimInstance* m_DustyPlayerAnimInstance;

	bool m_TriggerActive;
};
