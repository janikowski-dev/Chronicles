#include "UChronicle_DialogueLineNode.h"

void UChronicle_DialogueLineNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  TEXT("DialoguePin"), NAME_None, nullptr, TEXT("In"));
	CreatePin(EGPD_Output, TEXT("DialoguePin"), NAME_None, nullptr, TEXT("Out"));
}

FText UChronicle_DialogueLineNode::GetTitle() const
{
	return  FText::Format(FText::FromString("Line #{0}"), LineIndex);
}

FText UChronicle_DialogueLineNode::GetText() const
{
	return Text;
}

FText UChronicle_DialogueLineNode::GetSubtitle() const
{
	return Text;
}

bool UChronicle_DialogueLineNode::QualifiesForInnerGraph() const
{
	for (const UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction != EGPD_Input)
		{
			continue;
		}

		for (const UEdGraphPin* InnerPin : Pin->LinkedTo[0]->GetOwningNode()->Pins)
		{
			if (InnerPin->Direction != EGPD_Output)
			{
				continue;
			}

			return InnerPin->LinkedTo.Num() > 1;
		}
	}

	return false;
}
