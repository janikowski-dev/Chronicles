#pragma once

struct FChronicle_Colors
{
	static constexpr FLinearColor TurquoiseLinear{0.0f, 1.0f, 1.0f};
	static constexpr FLinearColor PurpleLinear{0.7f, 0.3f, 1.0f};
	static constexpr FLinearColor TealLinear{0.0f, 0.5f, 0.5f};
	static constexpr FLinearColor WhiteLinear{1.0f, 1.0f, 1.0f};
	static constexpr FLinearColor RedLinear{1.0f, 0.0f, 0.0f};
	static constexpr FLinearColor DarkRedLinear{8.0f, 0.0f, 0.0f};
	static constexpr FLinearColor BlackLinear{0.0f, 0.0f, 0.0f};
	static constexpr FLinearColor BlueLinear{0.0f, 0.0f, 1.0f};

	static constexpr FLinearColor ResponseConnection{TealLinear};
	static constexpr FLinearColor DefaultConnection{WhiteLinear};

	inline static const FSlateColor Root{BlackLinear};
	inline static const FSlateColor Link{DarkRedLinear};
	inline static const FSlateColor Line{RedLinear};
	inline static const FSlateColor Response{BlueLinear};
	
	inline static const FSlateColor Condition{TurquoiseLinear};
	inline static const FSlateColor Callback{PurpleLinear};
};