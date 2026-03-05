#include "FChronicle_EditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr<FSlateStyleSet> FChronicle_EditorStyle::StyleInstance;

void FChronicle_EditorStyle::Initialize()
{
	if (!TryInitializing())
	{
		return;
	}

	InitializeContentDirectory();
	InitializeIcons();
	InitializeBoxes();
	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
}

void FChronicle_EditorStyle::Shutdown()
{
	if (!StyleInstance.IsValid())
	{
		return;
	}

	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	StyleInstance.Reset();
}

const ISlateStyle& FChronicle_EditorStyle::Get()
{
	return *StyleInstance;
}

bool FChronicle_EditorStyle::TryInitializing()
{
	if (StyleInstance.IsValid())
	{
		return false;
	}

	StyleInstance = MakeShared<FSlateStyleSet>("FDialogueGraphEditorStyle");
	return true;
}

void FChronicle_EditorStyle::InitializeContentDirectory()
{
	const FString PluginDirectory = IPluginManager::Get().FindPlugin(TEXT("Chronicle"))->GetBaseDir();
	const FString ContentDirectory = PluginDirectory / TEXT("Resources");
	StyleInstance->SetContentRoot(ContentDirectory);
}

void FChronicle_EditorStyle::InitializeIcons()
{
	StyleInstance->Set(
		"Icons.Key",
		new FSlateImageBrush(
			StyleInstance->RootToContentDir(TEXT("Key.png")),
			FVector2D(24.0f, 24.0f)
		)
	);

	StyleInstance->Set(
		"Icons.Link",
		new FSlateImageBrush(
			StyleInstance->RootToContentDir(TEXT("Link.png")),
			FVector2D(24.0f, 24.0f)
		)
	);

	StyleInstance->Set(
		"Icons.Listener",
		new FSlateImageBrush(
			StyleInstance->RootToContentDir(TEXT("Listener.png")),
			FVector2D(24.0f, 24.0f)
		)
	);

	StyleInstance->Set(
		"Icons.Speaker",
		new FSlateImageBrush(
			StyleInstance->RootToContentDir(TEXT("Speaker.png")),
			FVector2D(24.0f, 24.0f)
		)
	);

	StyleInstance->Set(
		"Icons.Other",
		new FSlateImageBrush(
			StyleInstance->RootToContentDir(TEXT("Other.png")),
			FVector2D(24.0f, 24.0f)
		)
	);

	StyleInstance->Set(
		"Icons.Player",
		new FSlateImageBrush(
			StyleInstance->RootToContentDir(TEXT("Player.png")),
			FVector2D(24.0f, 24.0f)
		)
	);
}

void FChronicle_EditorStyle::InitializeBoxes()
{
	StyleInstance->Set("Node.Highlight",
		new FSlateBoxBrush(
			StyleInstance->RootToContentDir(TEXT("Highlight.png")),
			FMargin(0.28125f),
			FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)
		)
	);
}
