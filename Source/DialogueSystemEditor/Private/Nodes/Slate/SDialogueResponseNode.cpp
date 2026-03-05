#include "SDialogueResponseNode.h"

#include "FChronicleCharacterDirectory.h"
#include "Editors/FDialogueNodeEditor.h"
#include "Nodes/Unreal/UDialogueResponseNode.h"
#include "Utils/FColors.h"
#include "Utils/FDialogueGraphEditorStyle.h"
#include "Utils/FSlateHelper.h"

void SDialogueResponseNode::Construct(const FArguments&, UDialogueResponseNode* InNode)
{
	Cache(InNode);
	UpdateGraphNode();
}

FSlateColor SDialogueResponseNode::GetHeaderColor() const
{
	return FColors::Response;
}

void SDialogueResponseNode::UpdateGraphNode()
{
	FixAssignedId();
	SDialogueNode::UpdateGraphNode();
}

FReply SDialogueResponseNode::OnMouseButtonDoubleClick(const FGeometry&, const FPointerEvent&)
{
	OpenNodeEditor();
	return FReply::Handled();
}

void SDialogueResponseNode::AddBody(const TSharedRef<SVerticalBox>& Box)
{
	Box->AddSlot()
	.AutoHeight()
	.Padding(4)
	[
		FSlateHelper::MakeCharacterDisplay(
			FDialogueGraphEditorStyle::Get().GetBrush("Icons.Speaker"),
			FText::FromName(FChronicleCharacterDirectory::GetAll().GetName(TypedNode->ListenerId))
		)
	];
	
	Box->AddSlot()
	.AutoHeight()
	[
		FSlateHelper::MakeTextField(
			TAttribute<FText>(this, &SDialogueResponseNode::GetText),
			FOnTextCommitted::CreateSP(this, &SDialogueResponseNode::SetText)
		)
	];
}

FText SDialogueResponseNode::GetText() const
{
	return CastChecked<UDialogueResponseNode>(GraphNode)->Text;
}

void SDialogueResponseNode::SetText(const FText& NewText, ETextCommit::Type) const
{
	UDialogueResponseNode* Node = CastChecked<UDialogueResponseNode>(GraphNode);
	Node->Modify();
	Node->Text = NewText;
}

void SDialogueResponseNode::OpenNodeEditor() const
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

void SDialogueResponseNode::FixAssignedId() const
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
		return FChronicleCharacterDirectory::GetPlayable().IsValid(*Id);
	});

	TypedNode->ListenerId = PlayerId ? **PlayerId : FGuid();
}
