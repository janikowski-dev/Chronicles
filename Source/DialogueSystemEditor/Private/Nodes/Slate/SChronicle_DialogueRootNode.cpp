#include "SChronicle_DialogueRootNode.h"

#include "FChronicle_CharacterDirectory.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"
#include "Utils/FChronicle_Colors.h"
#include "Utils/FChronicle_EditorStyle.h"

void SChronicle_DialogueRootNode::Construct(const FArguments&, UChronicle_DialogueRootNode* InNode)
{
	Cache(InNode);
	UpdateGraphNode();
}

FSlateColor SChronicle_DialogueRootNode::GetHeaderColor() const
{
	return FChronicle_Colors::Root;
}

void SChronicle_DialogueRootNode::AddBody(const TSharedRef<SVerticalBox>& Box)
{
    AddCurrentParticipantList(Box);
    AddNewParticipantButton(Box);
}

void SChronicle_DialogueRootNode::AddCurrentParticipantList(const TSharedRef<SVerticalBox>& Box) const
{
	for (TSharedPtr ParticipantId : TypedGraph->SharedParticipantIds)
	{
		const bool bIsPlayer = FChronicle_CharacterDirectory::GetPlayable().IsValid(*ParticipantId);
		
		Box->AddSlot()
		.AutoHeight()
		.Padding(4.0f, 2.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 5.0f, 0.0f)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(FChronicle_EditorStyle::Get().GetBrush(
					bIsPlayer ? "Icons.Player" : "Icons.Other")
				)
				.DesiredSizeOverride(FVector2D(16.0f, 16.0f))
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromName(FChronicle_CharacterDirectory::GetAll().GetName(*ParticipantId)))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
				.OnClicked_Lambda([this, ParticipantId]
				{
					TypedGraph->RemoveParticipant(ParticipantId);
					return FReply::Handled();
				})
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("Icons.Minus"))
				]
			]
		];
	}
}

void SChronicle_DialogueRootNode::AddNewParticipantButton(const TSharedRef<SVerticalBox>& Box) const
{
	Box->AddSlot()
	.AutoHeight()
	.Padding(4.0f, 4.0f, 4.0f, 0.0f)
	[
		SNew(SSeparator)
		.Thickness(1.0f)
	];
	
	Box->AddSlot()
	.AutoHeight()
	.Padding(4.0f)
	.HAlign(HAlign_Center)
	[
		SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
		.OnClicked(this, &SChronicle_DialogueRootNode::OpenAddParticipantWindow)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Icons.Plus"))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(4.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Add Participant"))
			]
		]
	];
}

FReply SChronicle_DialogueRootNode::OpenAddParticipantWindow() const
{
	const TSharedRef<SVerticalBox> MissingParticipants = SNew(SVerticalBox);
	
	const bool bHasPlayer = TypedGraph->SharedParticipantIds.ContainsByPredicate([](const TSharedPtr<FGuid>& Id)
	{
		return FChronicle_CharacterDirectory::GetPlayable().IsValid(*Id);
	});
    
    for (const TSharedPtr<FGuid>& CharacterId : FChronicle_CharacterDirectory::GetAll().GetSharedIds())
    {
        if (TypedGraph->HasParticipant(CharacterId))
        {
            continue;
        }

    	const bool bIsPlayer = FChronicle_CharacterDirectory::GetPlayable().IsValid(*CharacterId);

    	if (bHasPlayer && bIsPlayer)
    	{
    		continue;
    	}
    
        const FName CharacterName = FChronicle_CharacterDirectory::GetAll().GetName(*CharacterId);
    
        MissingParticipants->AddSlot()
        .AutoHeight()
        [
            SNew(SButton)
            .ButtonStyle(FAppStyle::Get(), "Menu.Button")
            .ContentPadding(FMargin(16.0f, 6.0f))
            .HAlign(HAlign_Left)
            .OnClicked_Lambda([this, CharacterId]
            {
                TypedGraph->AddParticipant(CharacterId);
                FSlateApplication::Get().DismissAllMenus();
                return FReply::Handled();
            })
            [
            	SNew(STextBlock)
				.Text(FText::FromName(CharacterName))
				.TextStyle(FAppStyle::Get(), "Menu.Label")
            ]
        ];
    }

	const TSharedRef<SBorder> Menu =
	SNew(SBorder)
	.BorderImage(FAppStyle::GetBrush("Menu.Background"))
	[
		SNew(SVerticalBox)
	
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(12.0f, 8.0f, 12.0f, 6.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Add Participant"))
			.TextStyle(FAppStyle::Get(), "Menu.Heading")
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
		]
	
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSeparator)
			.Thickness(1.0f)
		]
	
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.MinDesiredWidth(120.0f)
			.MaxDesiredHeight(200.0f)
			[
				MissingParticipants->GetChildren()->Num() > 0
				? StaticCastSharedRef<SWidget>(
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					[
						MissingParticipants
					]
				)
				: StaticCastSharedRef<SWidget>(
					SNew(SBox)
					.Padding(12.0f, 8.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString("No characters to add"))
						.TextStyle(FAppStyle::Get(), "SmallText")
						.ColorAndOpacity(FSlateColor::UseSubduedForeground())
					]
				)
			]
		]
	];

	FSlateApplication::Get().PushMenu(
		FSlateApplication::Get().GetActiveTopLevelWindow().ToSharedRef(),
		FWidgetPath(),
		Menu,
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect::ContextMenu
	);

	return FReply::Handled();
}
