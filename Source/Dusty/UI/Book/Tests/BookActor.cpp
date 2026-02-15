// Fill out your copyright notice in the Description page of Project Settings.


#include "BookActor.h"
#include "../Book3D/LeftPage.h"
#include "../Book3D/RightPage.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "../Book2D/PageWidget.h"
#include "Components\Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Dusty/Utils/DustyUtils.h"

//Placeholder for B_Quit() function
#include "Dusty/Audio/AudioSubsytem.h"
#include "Kismet\KismetSystemLibrary.h"

// Sets default values
ABookActor::ABookActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	m_bookHardback = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Book Hardback"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> book_meshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/UI/Resources/Book3D/book_hardback.book_hardback'"));
	//m_bookHardback->SetStaticMesh(book_meshAsset.Object);
	m_bookHardback->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	m_bookPages = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Book Pages"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> pages_meshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/UI/Resources/Book3D/pages.pages'"));
	//m_bookPages->SetStaticMesh(pages_meshAsset.Object);
	m_bookPages->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);

	m_singlePage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Page"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> page_meshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/UI/Resources/Book3D/page.page'"));
	//m_singlePage->SetStaticMesh(page_meshAsset.Object);
	m_singlePage->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ABookActor::BeginPlay()
{
	Super::BeginPlay();
	
	LB_CreatePages();

	//Buttons
	Resume_Button->OnClicked.AddDynamic(this, &ABookActor::B_Resume);
	LogBook_Button->OnClicked.AddDynamic(this, &ABookActor::B_Logbook);
	Settings_Button->OnClicked.AddDynamic(this, &ABookActor::B_Settings);
	Quit_Button->OnClicked.AddDynamic(this, &ABookActor::B_Quit);
	NextPage_Button->OnClicked.AddDynamic(this, &ABookActor::B_NextPage);
	PreviusPage_Button->OnClicked.AddDynamic(this, &ABookActor::B_PreviusPage);
	GoPause_Button->OnClicked.AddDynamic(this, &ABookActor::B_GoPause);
}

// Called every frame
void ABookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABookActor::OpenBook(LG_Bookmarks _bookmark)
{
	m_isOpen = true;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetPause(true);

	//PLACEHOLDER, should be an animation or something like that
	if (this->IsHidden())
	{
		SetHidden(false);
	}

	m_leftPage->OpenBookmark(_bookmark);
	m_rightPage->OpenBookmark(_bookmark);


	switch (_bookmark)
	{
	//case LG_Bookmarks::Pause:
	//	Resume_Button->SetFocus();
	//	break;

	case LG_Bookmarks::LogBook:
		m_currenPage = 0;
		NextPage_Button->SetFocus();
		PreviusPage_Button->SetIsEnabled(false);
		NextPage_Button->SetIsEnabled(true);
		break;

	case LG_Bookmarks::Settings:
		//Se abren unas settings wapardas
		//Settings_Button->SetFocus();
		break;

	default:
		break;
	}

}

void ABookActor::CloseBook()
{
	m_isOpen = false;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetPause(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	//PLACEHOLDER, should be an animation or something like that
	SetHidden(true);
}

void ABookActor::ToggleBook()
{
	if (m_isOpen)
	{
		CloseBook();
	}
	else
	{
		//OpenBook(LG_Bookmarks::Pause);
	}
}

#pragma region LogBook Functions
void ABookActor::LB_NextPage()
{
	int nextpage = m_currenPage + 2;
	//if (nextpage < m_totalUnlockedPages)
	if (nextpage < m_maxPageNumberUnlocked)
	{

		if(m_pageSound)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_pageSound);
		}
		m_currenPage += 2;

		/*m_rightPage->SetActiveWidgetIndex(m_currenPage / 2);
		m_leftPage->SetActiveWidgetIndex(m_currenPage / 2);

		UPageWidget* right = Cast<UPageWidget>(m_rightPage->GetActiveWidget());
		UPageWidget* left = Cast<UPageWidget>(m_leftPage->GetActiveWidget());*/

		//Disable the nextPage button in case there is no next page
		nextpage = m_currenPage + 2;
		if (nextpage >= m_maxPageNumberUnlocked)
		{
			NextPage_Button->SetIsEnabled(false);
		}

		//Enable Previus page
		if (!PreviusPage_Button->GetIsEnabled())
		{
			PreviusPage_Button->SetIsEnabled(true);
		}
	}
	else
	{
		//DustyUtils::PrintMessage("ERROR: No more pages", FColor::Red, 30);
	}

}

void ABookActor::LB_PreviusPage()
{
	int nextpage = m_currenPage - 2;
	if (nextpage >= 0)
	{
		m_currenPage -= 2;

		if(m_pageSound)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_pageSound);
		}

		/*m_rightPage->SetActiveWidgetIndex(m_currenPage / 2);
		m_leftPage->SetActiveWidgetIndex(m_currenPage / 2);

		UPageWidget* right = Cast<UPageWidget>(m_rightPage->GetActiveWidget());
		UPageWidget* left = Cast<UPageWidget>(m_leftPage->GetActiveWidget());*/

		//Disable the previusPage button in case there is no next page
		nextpage = m_currenPage - 2;
		if (nextpage < 0)
		{
			PreviusPage_Button->SetIsEnabled(false);
		}

		//Enable Previus page
		if (!NextPage_Button->GetIsEnabled())
		{
			NextPage_Button->SetIsEnabled(true);
		}
	}
	else
	{
		//DustyUtils::PrintMessage("ERROR: No more pages", FColor::Red, 30);
	}
}

void ABookActor::LB_CreatePages()
{
	//m_totalUnlockedPages = 0;
	m_maxPageNumberUnlocked = 0;

	for (int i = 0; i < pagesClasses.Num(); i++)
	{
		UPageWidget* pageToAdd = CreateWidget<UPageWidget>(GetWorld(), pagesClasses[i]);
		pageToAdd->PageInit();

		//There is no page 0
		pageToAdd->SetPageNumber(i + 1);
		//if (pageToAdd->GetPageNumber() % 2 == 0)
		//{
		//	m_rightPage->AddNewPage(pageToAdd);
		//}
		//else
		//{
		//	m_leftPage->AddNewPage(pageToAdd);
		//}

		if (!pageToAdd->GetIsLocked())
		{
			//++m_totalUnlockedPages;
			if (pageToAdd->GetPageNumber() > m_maxPageNumberUnlocked)
			{
				m_maxPageNumberUnlocked = pageToAdd->GetPageNumber();
			}
		}
		else
		{
			m_pagesLockedList.Add(pageToAdd);
		}
	}
}

void ABookActor::LB_UnlockPage(int _pageNumber)
{
	for (int i = 0; i < m_pagesLockedList.Num(); i++)
	{
		if (m_pagesLockedList[i]->GetPageNumber() == _pageNumber)
		{
			//++m_totalUnlockedPages;
			if (_pageNumber > m_maxPageNumberUnlocked)
			{
				m_maxPageNumberUnlocked = _pageNumber;

				//DustyUtils::PrintMessage("New MaxPage: %i", m_maxPageNumberUnlocked, FColor::Green, 30);
			}

			m_pagesLockedList[i]->Unlock();
			return;
		}
	}
}

void ABookActor::LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock)
{
	for (int i = 0; i < m_pagesLockedList.Num(); i++)
	{
		if (m_pagesLockedList[i]->GetPageClass() == _pageToUnlock)
		{
			//++m_totalUnlockedPages;
			if (m_pagesLockedList[i]->GetPageNumber() > m_maxPageNumberUnlocked)
			{
				m_maxPageNumberUnlocked = m_pagesLockedList[i]->GetPageNumber();

				//DustyUtils::PrintMessage("New MaxPage: %i", m_maxPageNumberUnlocked, FColor::Green, 30);
			}

			m_pagesLockedList[i]->Unlock();
			return;
		}
	}
}
#pragma endregion

#pragma region ButtonFunctions
void ABookActor::B_Resume()
{
	CloseBook();
}

void ABookActor::B_Logbook()
{
	OpenBook(LG_Bookmarks::LogBook);
}

void ABookActor::B_Settings()
{
	OpenBook(LG_Bookmarks::Settings);
}

void ABookActor::B_Quit()
{
	// PLACEHOLDER
	//This should call a manager that handles how we quit the game
	UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Quit, true);
}

void ABookActor::B_NextPage()
{
	LB_NextPage();
}

void ABookActor::B_PreviusPage()
{
	LB_PreviusPage();
}

void ABookActor::B_GoPause()
{
	//OpenBook(LG_Bookmarks::Pause);
}
#pragma endregion