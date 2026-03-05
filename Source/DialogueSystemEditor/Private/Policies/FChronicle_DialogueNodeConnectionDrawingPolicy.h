#pragma once

#include "ConnectionDrawingPolicy.h"
#include "Graphs/UChronicle_RuleGraph.h"

class UChronicle_DialogueAsset;
struct FLineSegment;

class DIALOGUESYSTEMEDITOR_API FChronicle_DialogueNodeConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:
	FChronicle_DialogueNodeConnectionDrawingPolicy(
		int32 InBackLayerID,
		int32 InFrontLayerID,
		float InZoomFactor,
		const FSlateRect& InClippingRect,
		FSlateWindowElementList& InDrawElements
	);

protected:
	virtual void DrawSplineWithArrow(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FConnectionParams& Params
	) override;

private:
	void DrawStraightConnection(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FConnectionParams& Params
	) const;

	void DrawAngledConnection(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FVector2f& PinOffset,
		const FConnectionParams& Params
	) const;

	void DrawArrow(
		const FVector2f& EndPoint,
		const FVector2f& PinOffset,
		const FConnectionParams& Params
	) const;

	void DrawLinkIcon(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FConnectionParams& Params
	) const;
	
	void DrawKeyIcon(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FConnectionParams& Params
	) const;

	TArray<FLineSegment, TInlineAllocator<3>> BuildOrthogonalWire(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint,
		const FVector2f& PinOffset,
		const FConnectionParams& Params
	) const;

	FVector2f GetPinOffset() const;

	bool IsStraightConnection(
		const FVector2f& StartPoint,
		const FVector2f& EndPoint
	) const;

	bool IsTargetNodeEnabled(const FConnectionParams& Params) const;
	bool HasRules(const UEdGraphPin* Pin, const EOutputType Type) const;

private:
	const FSlateBrush* LinkBrush = nullptr;
	const FSlateBrush* KeyBrush = nullptr;
};