#include "SChronicle_DialogueLineNode.h"

#include "FChronicle_CharacterDirectory.h"
#include "SGraphPanel.h"
#include "Editors/FChronicle_RuleEditor.h"
#include "Graphs/UChronicle_DialogueGraph.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"
#include "Utils/FChronicle_Colors.h"
#include "Utils/FChronicle_EditorStyle.h"
#include "Utils/FChronicle_SlateHelper.h"

void SChronicle_DialogueLineNode::Construct(const FArguments&, UChronicle_DialogueLineNode* InNode)
{
	Cache(InNode);
	UpdateGraphNode();
}

FSlateColor SChronicle_DialogueLineNode::GetHeaderColor() const
{
	return FChronicle_Colors::Line;
}

void SChronicle_DialogueLineNode::UpdateGraphNode()
{
	FixAssignedIds();
	SChronicle_DialogueNode::UpdateGraphNode();
}

FReply SChronicle_DialogueLineNode::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	OpenNodeEditor();
	return FReply::Handled();
}

void SChronicle_DialogueLineNode::AddBody(const TSharedRef<SVerticalBox>& Box)
{
	Box->AddSlot()
	.AutoHeight()
	.Padding(4)
	[
		FChronicle_SlateHelper::MakeCharacterSelector(
			FChronicle_EditorStyle::Get().GetBrush("Icons.Speaker"),
			TAttribute<FText>(this, &SChronicle_DialogueLineNode::GetSpeakerName),
			SComboBox<TSharedPtr<FGuid>>::FOnSelectionChanged::CreateSP(this, &SChronicle_DialogueLineNode::SetSpeaker),
			&TypedGraph->SharedParticipantIds
		)
	];
	
	Box->AddSlot()
	.AutoHeight()
	.Padding(4)
	[
		FChronicle_SlateHelper::MakeCharacterSelector(
			FChronicle_EditorStyle::Get().GetBrush("Icons.Listener"),
			TAttribute<FText>(this, &SChronicle_DialogueLineNode::GetListenerName),
			SComboBox<TSharedPtr<FGuid>>::FOnSelectionChanged::CreateSP(this, &SChronicle_DialogueLineNode::SetListener),
			&TypedGraph->SharedParticipantIds
		)
	];
	
	Box->AddSlot()
	.AutoHeight()
	[
		FChronicle_SlateHelper::MakeTextField(
			TAttribute<FText>(this, &SChronicle_DialogueLineNode::GetText),
			FOnTextCommitted::CreateSP(this, &SChronicle_DialogueLineNode::SetText)
		)
	];
}

FText SChronicle_DialogueLineNode::GetText() const
{
	return TypedNode->Text;
}

void SChronicle_DialogueLineNode::SetText(const FText& NewText, ETextCommit::Type) const
{
	TypedNode->Modify();
	TypedNode->Text = NewText;
}

FText SChronicle_DialogueLineNode::GetListenerName() const
{
	return FText::FromName(FChronicle_CharacterDirectory::GetAll().GetName(TypedNode->ListenerId));
}

void SChronicle_DialogueLineNode::SetListener(TSharedPtr<FGuid> Id, ESelectInfo::Type) const
{
	if (TypedNode->SpeakerId == *Id)
	{
		TypedNode->SpeakerId = TypedNode->ListenerId;
	}
	
	TypedNode->ListenerId = *Id;
}

FText SChronicle_DialogueLineNode::GetSpeakerName() const
{
	return FText::FromName(FChronicle_CharacterDirectory::GetAll().GetName(TypedNode->SpeakerId));
}

void SChronicle_DialogueLineNode::SetSpeaker(TSharedPtr<FGuid> Id, ESelectInfo::Type) const
{
	if (TypedNode->ListenerId == *Id)
	{
		TypedNode->ListenerId = TypedNode->SpeakerId;
	}
	
	TypedNode->SpeakerId = *Id;
}

void SChronicle_DialogueLineNode::FixAssignedIds() const
{
	const bool bContainsListener = TypedGraph->SharedParticipantIds.ContainsByPredicate([&](const TSharedPtr<FGuid>& Id)
	{
		return Id && *Id == TypedNode->ListenerId;
	});

	const bool bContainsSpeaker = TypedGraph->SharedParticipantIds.ContainsByPredicate([&](const TSharedPtr<FGuid>& Id)
	{
		return Id && *Id == TypedNode->SpeakerId;
	});
	
	if (!bContainsListener)
	{
		TypedNode->ListenerId = FGuid();
	}
	
	if (!bContainsSpeaker)
	{
		TypedNode->SpeakerId = FGuid();
	}

	for (const TSharedPtr<FGuid>& Id : TypedGraph->SharedParticipantIds)
	{
		if (!Id)
		{
			continue;
		}

		if (!TypedNode->ListenerId.IsValid() && *Id != TypedNode->SpeakerId)
		{
			TypedNode->ListenerId = *Id;
		}
		else if (!TypedNode->SpeakerId.IsValid() && *Id != TypedNode->ListenerId)
		{
			TypedNode->SpeakerId = *Id;
		}
	}
}

void SChronicle_DialogueLineNode::OpenNodeEditor() const
{
	if (UChronicle_DialogueNode* NodeAsset = Cast<UChronicle_DialogueNode>(GraphNode))
	{
		const TSharedRef<FChronicle_RuleEditor> Editor = MakeShared<FChronicle_RuleEditor>();

		Editor->InitNodeAssetEditor(
			EToolkitMode::Standalone,
			TSharedPtr<IToolkitHost>(),
			NodeAsset
		);
	}
}
