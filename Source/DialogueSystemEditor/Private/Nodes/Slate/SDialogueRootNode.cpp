#include "SDialogueRootNode.h"

#include "FChronicleCharacterDirectory.h"
#include "Nodes/Unreal/UDialogueRootNode.h"
#include "Utils/FColors.h"
#include "Utils/FDialogueGraphEditorStyle.h"

void SDialogueRootNode::Construct(const FArguments&, UDialogueRootNode* InNode)
{
	Cache(InNode);
	UpdateGraphNode();
}

FSlateColor SDialogueRootNode::GetHeaderColor() const
{
	return FColors::Root;
}

void SDialogueRootNode::AddBody(const TSharedRef<SVerticalBox>& Box)
{
    AddCurrentParticipantList(Box);
    AddNewParticipantButton(Box);
}

void SDialogueRootNode::AddCurrentParticipantList(const TSharedRef<SVerticalBox>& Box) const
{
	for (TSharedPtr ParticipantId : TypedGraph->SharedParticipantIds)
	{
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
				.Image(FDialogueGraphEditorStyle::Get().GetBrush("Icons.Participant"))
				.DesiredSizeOverride(FVector2D(16.0f, 16.0f))
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromName(FChronicleCharacterDirectory::GetAll().GetName(*ParticipantId)))
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

void SDialogueRootNode::AddNewParticipantButton(const TSharedRef<SVerticalBox>& Box) const
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
		.OnClicked(this, &SDialogueRootNode::OpenAddParticipantWindow)
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

FReply SDialogueRootNode::OpenAddParticipantWindow() const
{
	const TSharedRef<SVerticalBox> MissingParticipants = SNew(SVerticalBox);
    
    for (const TSharedPtr<FGuid>& CharacterId : FChronicleCharacterDirectory::GetAll().GetSharedIds())
    {
        if (TypedGraph->HasParticipant(CharacterId))
        {
            continue;
        }
    
        const FName CharacterName = FChronicleCharacterDirectory::GetAll().GetName(*CharacterId);
    
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
