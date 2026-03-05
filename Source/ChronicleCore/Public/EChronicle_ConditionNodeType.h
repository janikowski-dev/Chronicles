#pragma once

UENUM()
enum class EChronicle_ConditionNodeType : uint8
{
	Raw,
	And,
	Or,
	Not,
	Output
};