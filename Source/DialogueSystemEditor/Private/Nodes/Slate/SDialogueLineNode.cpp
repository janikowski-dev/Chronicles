#include "SDialogueLineNode.h"

#include "FChronicleCharacterDirectory.h"
#include "SGraphPanel.h"
#include "Editors/FDialogueNodeEditor.h"
#include "Graphs/UDialogueGraph.h"
#include "Nodes/Unreal/UDialogueLineNode.h"
#include "Nodes/Unreal/UDialogueRootNode.h"
#include "Utils/FColors.h"
#include "Utils/FDialogueGraphEditorStyle.h"
#include "Utils/FSlateHelper.h"

void SDialogueLineNode::Construct(const FArguments&, UDialogueLineNode* InNode)
{
	Cache(InNode);
	UpdateGraphNode();
}

FSlateColor SDialogueLineNode::GetHeaderColor() const
{
	return FColors::Line;
}

void SDialogueLineNode::UpdateGraphNode()
{
	FixAssignedIds();
	SDialogueNode::UpdateGraphNode();
}

FReply SDialogueLineNode::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	OpenNodeEditor();
	return FReply::Handled();
}

void SDialogueLineNode::AddBody(const TSharedRef<SVerticalBox>& Box)
{
	Box->AddSlot()
	.AutoHeight()
	.Padding(4)
	[
		FSlateHelper::MakeCharacterSelector(
			FDialogueGraphEditorStyle::Get().GetBrush("Icons.Speaker"),
			TAttribute<FText>(this, &SDialogueLineNode::GetSpeakerName),
			SComboBox<TSharedPtr<FGuid>>::FOnSelectionChanged::CreateSP(this, &SDialogueLineNode::SetSpeaker),
			&TypedGraph->SharedParticipantIds
		)
	];
	
	Box->AddSlot()
	.AutoHeight()
	.Padding(4)
	[
		FSlateHelper::MakeCharacterSelector(
			FDialogueGraphEditorStyle::Get().GetBrush("Icons.Listener"),
			TAttribute<FText>(this, &SDialogueLineNode::GetListenerName),
			SComboBox<TSharedPtr<FGuid>>::FOnSelectionChanged::CreateSP(this, &SDialogueLineNode::SetListener),
			&TypedGraph->SharedParticipantIds
		)
	];
	
	Box->AddSlot()
	.AutoHeight()
	[
		FSlateHelper::MakeTextField(
			TAttribute<FText>(this, &SDialogueLineNode::GetText),
			FOnTextCommitted::CreateSP(this, &SDialogueLineNode::SetText)
		)
	];
}

FText SDialogueLineNode::GetText() const
{
	return TypedNode->Text;
}

void SDialogueLineNode::SetText(const FText& NewText, ETextCommit::Type) const
{
	TypedNode->Modify();
	TypedNode->Text = NewText;
}

FText SDialogueLineNode::GetListenerName() const
{
	return FText::FromName(FChronicleCharacterDirectory::GetAll().GetName(TypedNode->ListenerId));
}

void SDialogueLineNode::SetListener(TSharedPtr<FGuid> Id, ESelectInfo::Type) const
{
	if (TypedNode->SpeakerId == *Id)
	{
		TypedNode->SpeakerId = TypedNode->ListenerId;
	}
	
	TypedNode->ListenerId = *Id;
}

FText SDialogueLineNode::GetSpeakerName() const
{
	return FText::FromName(FChronicleCharacterDirectory::GetAll().GetName(TypedNode->SpeakerId));
}

void SDialogueLineNode::SetSpeaker(TSharedPtr<FGuid> Id, ESelectInfo::Type) const
{
	if (TypedNode->ListenerId == *Id)
	{
		TypedNode->ListenerId = TypedNode->SpeakerId;
	}
	
	TypedNode->SpeakerId = *Id;
}

void SDialogueLineNode::FixAssignedIds() const
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

	if (!bContainsListener && !bContainsSpeaker && TypedGraph->SharedParticipantIds.Num() > 1)
	{
		TypedNode->ListenerId = *TypedGraph->SharedParticipantIds[0];
		TypedNode->SpeakerId = *TypedGraph->SharedParticipantIds[1];
	}
}

void SDialogueLineNode::OpenNodeEditor() const
{
	if (UDialogueNode* NodeAsset = Cast<UDialogueNode>(GraphNode))
	{
		const TSharedRef<FDialogueNodeEditor> Editor = MakeShared<FDialogueNodeEditor>();

		Editor->InitNodeAssetEditor(
			EToolkitMode::Standalone,
			TSharedPtr<IToolkitHost>(),
			NodeAsset
		);
	}
}
