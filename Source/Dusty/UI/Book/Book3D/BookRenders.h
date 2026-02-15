// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BookRenders.generated.h"

class USceneCaptureComponent2D;
class UWidgetComponent;

UCLASS()
class DUSTY_API ABookRenders : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, DisplayName = "Main Render Target")
		USceneCaptureComponent2D* m_mainRender;
	UPROPERTY(EditAnywhere, DisplayName = "Auxiliar Render Target")
		USceneCaptureComponent2D* m_auxRender;
	UPROPERTY(EditAnywhere, DisplayName = "Main Book")
		UWidgetComponent* m_mainBook;
	UPROPERTY(EditAnywhere, DisplayName = "Auxiliar Book")
		UWidgetComponent* m_auxBook;

	UPROPERTY(EditDefaultsOnly, category = "Capture Settings")
	float m_framesToCapture = 3;

	float m_currentFrames = 0;

	bool m_isOpen = false;

public:	
	ABookRenders();

	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;

	void RenderOpenBook();
	void RenderCloseBook();

	void ActivateAuxiliarRender(bool _value);
};
