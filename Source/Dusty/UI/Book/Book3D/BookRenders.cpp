// Fill out your copyright notice in the Description page of Project Settings.


#include "BookRenders.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/WidgetComponent.h"
#include "BookPageWidget.h"

#include "BookPageWidget.h"

//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Dusty/Utils/DustyUtils.h"
#include "../../../Character/DustyCharacter.h"

// Sets default values
ABookRenders::ABookRenders()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	m_mainRender = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Main Render Target"));
	m_mainRender->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
	m_mainRender->bAlwaysPersistRenderingState = true;
	m_auxRender = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Auxiliar Render Target"));
	m_auxRender->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
	m_auxRender->bAlwaysPersistRenderingState = true;

	m_mainBook = CreateDefaultSubobject<UWidgetComponent>(TEXT("Main Book"));
	m_mainBook->AttachToComponent(m_mainRender, FAttachmentTransformRules::KeepRelativeTransform);
	m_auxBook = CreateDefaultSubobject<UWidgetComponent>(TEXT("Auxiliar Book"));
	m_auxBook->AttachToComponent(m_auxRender, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABookRenders::BeginPlay()
{
	Super::BeginPlay();
	m_mainRender->CaptureScene();
	m_auxRender->CaptureScene();

	ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();

	if (dusty)
	{
		UBookPageWidget* main = Cast<UBookPageWidget>(m_mainBook->GetWidget());
		UBookPageWidget* aux = Cast<UBookPageWidget>(m_auxBook->GetWidget());
		dusty->SetResourcesToBook(main, aux, this);
		RenderCloseBook();
	}
}

void ABookRenders::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (m_isOpen)
	{
		if (m_currentFrames >= m_framesToCapture)
		{
			m_mainRender->CaptureScene();

			if (m_auxRender->IsActive())
			{
				m_auxRender->CaptureScene();
			}
			m_currentFrames = 0;
		}
		else
		{
			++m_currentFrames;
		}
	}
}

void ABookRenders::RenderOpenBook()
{
	m_isOpen = true;
	m_mainRender->SetActive(true);
	m_mainBook->SetActive(true);
	m_auxRender->SetActive(true);
	m_auxBook->SetActive(true);
	m_mainRender->CaptureScene();
	m_auxRender->CaptureScene();
}

void ABookRenders::RenderCloseBook()
{
	m_isOpen = false;
	m_mainRender->SetActive(false);
	m_auxRender->SetActive(false);
	m_mainBook->SetActive(false);
	m_auxBook->SetActive(false);
}

void ABookRenders::ActivateAuxiliarRender(bool _value)
{
	/*if (this != nullptr && m_auxRender != nullptr && m_auxBook != nullptr)
	{
		m_auxRender->SetActive(_value);

		if (_value)
		{
			m_auxRender->CaptureScene();
		}
	}*/
}
