
#include "BookmarkComponent.h"
#include "BookmarkWidget.h"
#include "BookActorTextures.h"

void UBookmarkComponent::Init()
{
	GetOwner();
	UUserWidget* widget = GetWidget();
	SetCastShadow(false);

	if (widget && widget->IsA<UBookmarkWidget>() && GetOwner()->IsA<ABookActorTextures>())
	{
		Cast<UBookmarkWidget>(widget)->InitBookmark(Cast<ABookActorTextures>(GetOwner()), m_bookmark);
	}
}