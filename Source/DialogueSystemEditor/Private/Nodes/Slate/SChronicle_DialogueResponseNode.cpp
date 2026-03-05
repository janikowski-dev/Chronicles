#include "SChronicle_DialogueResponseNode.h"

#include "FChronicle_CharacterDirectory.h"
#include "Editors/FChronicle_RuleEditor.h"
#include "Nodes/Unreal/UChronicle_DialogueResponseNode.h"
#include "Utils/FChronicle_Colors.h"
#include "Utils/FChronicle_EditorStyle.h"
#include "Utils/FChronicle_SlateHelper.h"

void SChronicle_DialogueResponseNode::Construct(const FArguments&, UChronicle_DialogueResponseNode* InNode)
{
	Cache(InNode);
	UpdateGraphNode();
}

FSlateColor SChronicle_DialogueResponseNode::GetHeaderColor() const
{
	return FChronicle_Colors::Response;
}

void SChronicle_DialogueResponseNode::UpdateGraphNode()
{
	FixAssignedId();
	SChronicle_DialogueNode::UpdateGraphNode();
}

FReply SChronicle_DialogueResponseNode::OnMouseButtonDoubleClick(const FGeometry&, const FPointerEvent&)
{
	OpenNodeEditor();
	return FReply::Handled();
}

void SChronicle_DialogueResponseNode::AddBody(const TSharedRef<SVerticalBox>& Box)
{
	Box->AddSlot()
	.AutoHeight()
	.Padding(4)
	[
		FChronicle_SlateHelper::MakeCharacterDisplay(
			FChronicle_EditorStyle::Get().GetBrush("Icons.Speaker"),
			FText::FromName(FChronicle_CharacterDirectory::GetAll().GetName(TypedNode->ListenerId))
		)
	];
	
	Box->AddSlot()
	.AutoHeight()
	[
		FChronicle_SlateHelper::MakeTextField(
			TAttribute<FText>(this, &SChronicle_DialogueResponseNode::GetText),
			FOnTextCommitted::CreateSP(this, &SChronicle_DialogueResponseNode::SetText)
		)
	];
}

FText SChronicle_DialogueResponseNode::GetText() const
{
	return CastChecked<UChronicle_DialogueResponseNode>(GraphNode)->Text;
}

void SChronicle_DialogueResponseNode::SetText(const FText& NewText, ETextCommit::Type) const
{
	UChronicle_DialogueResponseNode* Node = CastChecked<UChronicle_DialogueResponseNode>(GraphNode);
	Node->Modify();
	Node->Text = NewText;
}

void SChronicle_DialogueResponseNode::OpenNodeEditor() const
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

void SChronicle_DialogueResponseNode::FixAssignedId() const
{
	const bool bHasListener = TypedGraph->SharedParticipantIds.ContainsByPredicate([this](const TSharedPtr<FGuid>& Id)
	{
		return Id && *Id == TypedNode->ListenerId;
	});
	
	if (bHasListener)
	{
		return;
	}

	const TSharedPtr<FGuid>* PlayerId = TypedGraph->SharedParticipantIds.FindByPredicate([](const TSharedPtr<FGuid>& Id)
	{
		return FChronicle_CharacterDirectory::GetPlayable().IsValid(*Id);
	});

	TypedNode->ListenerId = PlayerId ? **PlayerId : FGuid();
}
