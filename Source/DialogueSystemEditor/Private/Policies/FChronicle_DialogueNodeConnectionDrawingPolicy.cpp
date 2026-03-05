#include "FChronicle_DialogueNodeConnectionDrawingPolicy.h"

#include "Assets/UChronicle_DialogueAsset.h"
#include "Graphs/UChronicle_RuleGraph.h"
#include "Nodes/Unreal/UChronicle_DialogueNode.h"
#include "Nodes/Unreal/UChronicle_DialogueResponseNode.h"
#include "Nodes/Unreal/UChronicle_RuleOutputNode.h"
#include "Utils/FChronicle_Colors.h"

struct FLineSegment
{
	FVector2f A;
	FVector2f B;
};

FChronicle_DialogueNodeConnectionDrawingPolicy::FChronicle_DialogueNodeConnectionDrawingPolicy(
	const int32 InBackLayerID,
	const int32 InFrontLayerID,
	const float InZoomFactor,
	const FSlateRect& InClippingRect,
	FSlateWindowElementList& InDrawElements
)
	: FConnectionDrawingPolicy(
		InBackLayerID,
		InFrontLayerID,
		InZoomFactor,
		InClippingRect,
		InDrawElements
	)
{
	LinkBrush = FSlateIcon("FDialogueGraphEditorStyle", "Icons.Link").GetIcon();
	KeyBrush = FSlateIcon("FDialogueGraphEditorStyle", "Icons.Key").GetIcon();
}

void FChronicle_DialogueNodeConnectionDrawingPolicy::DrawSplineWithArrow(
	const FVector2f& StartPoint,
	const FVector2f& EndPoint,
	const FConnectionParams& Params
)
{
	if (!IsTargetNodeEnabled(Params))
	{
		return;
	}

	const FVector2f PinOffset = GetPinOffset();
	
	if (IsStraightConnection(StartPoint, EndPoint))
	{
		DrawStraightConnection(StartPoint + PinOffset, EndPoint - PinOffset, Params);
	}
	else
	{
		DrawAngledConnection(StartPoint, EndPoint, PinOffset, Params);
	}
	
	DrawArrow(EndPoint, PinOffset, Params);
	
	if (HasRules(Params.AssociatedPin2, EOutputType::Requirements))
	{
		DrawKeyIcon(StartPoint, EndPoint, Params);
	}
	
	if (HasRules(Params.AssociatedPin1, EOutputType::PostCallback))
	{
		DrawLinkIcon(StartPoint, EndPoint, Params);
	}
}

void FChronicle_DialogueNodeConnectionDrawingPolicy::DrawAngledConnection(
	const FVector2f& StartPoint,
	const FVector2f& EndPoint,
	const FVector2f& PinOffset,
	const FConnectionParams& Params
) const
{
	const TArray<FLineSegment, TInlineAllocator<3>> Segments = BuildOrthogonalWire(StartPoint, EndPoint, PinOffset, Params);

	for (const auto& [A, B] : Segments)
	{
		DrawStraightConnection(A, B, Params);
	}
}

void FChronicle_DialogueNodeConnectionDrawingPolicy::DrawStraightConnection(
	const FVector2f& StartPoint,
	const FVector2f& EndPoint,
	const FConnectionParams& Params
) const
{
	TArray<FVector2f> Points;
	
	Points.Add(StartPoint);
	Points.Add(EndPoint);

	const bool bIsResponseNode = Params.AssociatedPin2
		&& Params.AssociatedPin2->GetOwningNodeUnchecked()
		&& Params.AssociatedPin2->GetOwningNodeUnchecked()->IsA<UChronicle_DialogueResponseNode>();

	const FLinearColor WireColor = bIsResponseNode ? FChronicle_Colors::ResponseConnection : FChronicle_Colors::DefaultConnection;
	int32 LayerId = bIsResponseNode ? WireLayerID + 1 : WireLayerID;
	
	FSlateDrawElement::MakeLines(
		DrawElementsList,
		LayerId,
		FPaintGeometry(),
		Points,
		ESlateDrawEffect::NoBlending,
		WireColor,
		true,
		Params.WireThickness
	);
}

void FChronicle_DialogueNodeConnectionDrawingPolicy::DrawArrow(
	const FVector2f& EndPoint,
	const FVector2f& PinOffset,
	const FConnectionParams& Params
) const
{
	const FVector2f ArrowPoint = EndPoint - ArrowRadius - PinOffset;

	const bool bIsResponseNode = Params.AssociatedPin2
		&& Params.AssociatedPin2->GetOwningNodeUnchecked()
		&& Params.AssociatedPin2->GetOwningNodeUnchecked()->IsA<UChronicle_DialogueResponseNode>();

	const FLinearColor ArrowColor = bIsResponseNode ? FChronicle_Colors::ResponseConnection : FChronicle_Colors::DefaultConnection;
	
	FSlateDrawElement::MakeBox(
		DrawElementsList,
		ArrowLayerID,
		FPaintGeometry(ArrowPoint, ArrowImage->ImageSize * ZoomFactor, ZoomFactor),
		ArrowImage,
		ESlateDrawEffect::None,
		ArrowColor
	);
}

void FChronicle_DialogueNodeConnectionDrawingPolicy::DrawLinkIcon(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FConnectionParams& Params
) const
{
	const FVector2f LinkPoint = FVector2f(
		StartPoint.X + (EndPoint.X - StartPoint.X) * 0.25f - LinkBrush->ImageSize.X * ZoomFactor * 0.25f,
		StartPoint.Y - LinkBrush->ImageSize.X * ZoomFactor * 0.5f
	);
	
	FSlateDrawElement::MakeBox(
		DrawElementsList,
		ArrowLayerID,
		FPaintGeometry(LinkPoint, LinkBrush->ImageSize * ZoomFactor, ZoomFactor),
		LinkBrush,
		ESlateDrawEffect::None,
		Params.WireColor
	);
}

void FChronicle_DialogueNodeConnectionDrawingPolicy::DrawKeyIcon(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FConnectionParams& Params
) const
{
	const FVector2f KeyPoint = FVector2f(
		StartPoint.X + (EndPoint.X - StartPoint.X) * 0.75f - KeyBrush->ImageSize.X * ZoomFactor * 0.75f,
		EndPoint.Y - KeyBrush->ImageSize.X * ZoomFactor * 0.5f
	);
	
	FSlateDrawElement::MakeBox(
		DrawElementsList,
		ArrowLayerID,
		FPaintGeometry(KeyPoint, KeyBrush->ImageSize * ZoomFactor, ZoomFactor),
		KeyBrush,
		ESlateDrawEffect::None,
		Params.WireColor
	);
}

TArray<FLineSegment, TInlineAllocator<3>> FChronicle_DialogueNodeConnectionDrawingPolicy::BuildOrthogonalWire(
	const FVector2f& StartPoint,
	const FVector2f& EndPoint,
	const FVector2f& PinOffset,
	const FConnectionParams& Params
) const
{
	TArray<FLineSegment, TInlineAllocator<3>> Segments;

	const FVector2f HorizontalAnchorOffset(
		Params.WireThickness * 0.5f,
		0.0f
	);
	
	const FVector2f VerticalAnchorOffset(
		0.0f,
		Params.WireThickness * 0.5f
	);
	
	const FVector2f StartAnchor(
		(StartPoint.X + EndPoint.X) * 0.5f,
		StartPoint.Y
	);

	const FVector2f EndAnchor(
		(StartPoint.X + EndPoint.X) * 0.5f,
		EndPoint.Y
	);

	FVector2f BottomAnchor;
	FVector2f TopAnchor;

	if (StartAnchor.Y > EndAnchor.Y)
	{
		BottomAnchor = EndAnchor;
		TopAnchor = StartAnchor;
	}
	else
	{
		BottomAnchor = StartAnchor;
		TopAnchor = EndAnchor;
	}

	Segments.Add(FLineSegment(StartPoint + PinOffset, StartAnchor + HorizontalAnchorOffset));
	Segments.Add(FLineSegment(TopAnchor + VerticalAnchorOffset, BottomAnchor - VerticalAnchorOffset));
	Segments.Add(FLineSegment(EndAnchor - HorizontalAnchorOffset, EndPoint - PinOffset));
	
	return Segments;
}

FVector2f FChronicle_DialogueNodeConnectionDrawingPolicy::GetPinOffset() const
{
	const FVector2f PinOffset(
		16.0f,
		0.0f
	);

	return PinOffset * ZoomFactor;
}

bool FChronicle_DialogueNodeConnectionDrawingPolicy::IsStraightConnection(
	const FVector2f& StartPoint,
	const FVector2f& EndPoint
) const
{
	return StartPoint.Y == EndPoint.Y;
}

bool FChronicle_DialogueNodeConnectionDrawingPolicy::IsTargetNodeEnabled(const FConnectionParams& Params) const
{
	UEdGraphNode* Node = Params.AssociatedPin2->GetOwningNode();
	const UChronicle_DialogueNode* TypedNode = Cast<UChronicle_DialogueNode>(Node);
	return !TypedNode->bIsHidden;
}

bool FChronicle_DialogueNodeConnectionDrawingPolicy::HasRules(const UEdGraphPin* Pin, const EOutputType Type) const
{
	if (!Pin)
	{
		return false;
	}
	
	const UEdGraphNode* Node = Pin->GetOwningNode();
	
	if (!Node)
	{
		return false;
	}

	const UChronicle_DialogueNode* TypedNode = Cast<UChronicle_DialogueNode>(Node);

	if (!TypedNode)
	{
		return false;
	}
	
	const UChronicle_DialogueAsset* Owner = Pin->GetOwningNode()->GetGraph()->GetTypedOuter<UChronicle_DialogueAsset>();
	const TObjectPtr<UChronicle_RuleGraph>* RuleGraphPointer = Owner->InnerGraphsByNode.Find(TypedNode);
	
	if (!RuleGraphPointer)
	{
		return false;
	}
	
	const UChronicle_RuleGraph* RuleGraph = RuleGraphPointer->Get();
	
	if (!RuleGraph)
	{
		return false;
	}
	
	return RuleGraph->GetRules(Type).Num() > 1;
}
