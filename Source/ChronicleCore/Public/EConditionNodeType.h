#pragma once

UENUM()
enum class EConditionNodeType : uint8
{
	Raw,
	And,
	Or,
	Not,
	Output
};