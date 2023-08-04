#pragma once

#include "AppDefinition.h"

class SoftRenderer
{
public:

	SoftRenderer();
	~SoftRenderer() = default;

	void Initialize(class GDIHelper* InitGDIHelper);
	void UpdateFrame(void);
	bool IsInRange(i32 x, i32 y);
	void DrawPixel(i32 x, i32 y);

	void DrawLine(const i32 StartXPos, const i32 StartYPos, const i32 EndXPos, const i32 EndYPos);
	void DrawHorizontalLine(const i32 StartXPos, const i32 EndXPos, const i32 y);

	void DrawTriangle (i32 XPosA, i32 YPosA, i32 XPosB, i32 YPosB, i32 XPosC, i32 YPosC);
	void SortVerticesByY(i32 &XPosA, i32 &YPosA, i32 &XPosB,  i32 &YPosB,  i32 &XPosC,  i32 &YPosC);
	void FillTopFlatTriangle(i32 XPosA, i32 YPosA, i32 XPosB, i32 YPosB, i32 XPosC, i32 YPosC);
	void FillBottomFlatTriangle(i32 XPosA, i32 YPosA, i32 XPosB, i32 YPosB, i32 XPosC, i32 YPosC);
private:

	class GDIHelper* mGDIHelper;
	// class MathLibrary* mathLibrary;

};
