#pragma once

class FChronicle_SlateHelper
{
public:
	static TSharedRef<SWidget> MakeTextField(
		const TAttribute<FText>& Getter,
		const FOnTextCommitted& Setter
	);

	static TSharedRef<SWidget> MakeTextField(
		const TAttribute<FText>& Getter
	);

	static TSharedRef<SWidget> MakeCharacterSelector(
		const FSlateBrush* Icon,
		const TAttribute<FText>& NameGetter,
		const SComboBox<TSharedPtr<FGuid>>::FOnSelectionChanged& OnSelectionChanged,
		const TArray<TSharedPtr<FGuid>>* CharacterIds
	);

	static TSharedRef<SWidget> MakeCharacterDisplay(
		const FSlateBrush* Icon,
		const TAttribute<FText>& NameGetter
	);
};

