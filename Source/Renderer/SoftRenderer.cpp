#include "SoftRenderer.h"
#include "Windows/GDIHelper.h"

#define Swap(a, b) {int temp; temp = a; a=b; b=temp;}

SoftRenderer::SoftRenderer()
{
	mGDIHelper = nullptr;
}

void SoftRenderer::Initialize(GDIHelper* InitGDIHelper)
{
	if (InitGDIHelper == nullptr)
	{
		return;
	}

	mGDIHelper = InitGDIHelper;

	return;
}

bool SoftRenderer::IsInRange(i32 x, i32 y)
{
	return (abs(x) < (APP_WIDTH / 2)) && (abs(y) < (APP_HEIGHT / 2));
}

void SoftRenderer::DrawPixel(i32 x, i32 y)
{
	if (!IsInRange(x, y))
	{
		return;
	}

	u32* dest = reinterpret_cast<u32*>(mGDIHelper->GetpBits());
	i32 offset = APP_WIDTH * APP_HEIGHT / 2 + APP_WIDTH / 2 + x + APP_WIDTH * -y;
	*(dest + offset) = mGDIHelper->GetCurrentColor();
}

void SoftRenderer::DrawLine(const i32 StartXPos, const i32 StartYPos, const i32 EndXPos, const i32 EndYPos)
{
	int AddX = 0, AddY = 0; // 라인의 진행방향에 따라 1 또는 -1이 된다.
	int x = StartXPos, y = StartYPos; // 선을 그리면서 쓸 좌표의 값이다.
	int Counter = 0; // x또는 y의 값을 조정할 때 사용한다.
	int W = EndXPos - StartXPos; // 너비를 구합니다.
	int H = EndYPos - StartYPos; // 높이를 구합니다.

	if (W < 0) 
	{
		AddX = -1; // 음수방향(왼쪽방향)이라면 x의 값이 -1씩 증가되게 해줍니다.
		W = -W; // 너비에 -1을 곱해 올바르게 구해줍니다.
	}
	else
	{
		AddX = 1;
	}

	if (H < 0)
	{
		AddY = -1; // 음수방향(아랫방향)이라면 y의 값이 -1씩 증가되게 해줍니다.
		H = -H; // 높이에 -1을 곱해 올바르게 구해줍니다.
	}
	else
	{
		AddY = 1;
	}

	if (W >= H) // 너비가 더 크다면
	{
		for (int i = 0; i < W; i++) // 너비만큼 반복해준다.
		{
			x += AddX;
			Counter += H; // 기울기를 더하는 것과 같지만 최적화를 위해 조건문과 함께 바뀌었다.

			if (Counter >= W) // 카운터의 값이 W를 넘는다면
			{
				y += AddY; // y의 값에 AddY를 더해준 후
				Counter -= W; // 카운터에서 너비만큼 빼준다.
			}
			DrawPixel(x, y);
		}
	}
	else // 높이가 더 크다면
	{
		for (int i = 0; i < H; i++) // 높이만큼 반복해준다.
		{
			y += AddY;
			Counter += W;

			if (Counter >= H) // 카운터의 값이 H를 넘는다면
			{
				x += AddX; // x의 값에 AddX를 더해준 후
				Counter -= H; // 카운터에서 너비만큼 빼준다.
			}
			DrawPixel(x, y);
		}
	}
}

void SoftRenderer::DrawHorizontalLine(const i32 StartXPos, const i32 EndXPos, const i32 y)
{
	int x1 = StartXPos, x2 = EndXPos;
	if (x1 > x2)
	{
		Swap(x1, x2);
	}
	for (int x = StartXPos; x <= EndXPos; x++)
	{
		DrawPixel(x, y);
	}
}

void SoftRenderer::DrawTriangle(i32 XPosA, i32 YPosA, i32 XPosB, i32 YPosB, i32 XPosC, i32 YPosC)
{
	// 좌표를 정렬해준다.
	SortVerticesByY(XPosA, YPosA, XPosB, YPosB, XPosC, YPosC);
	// 외곽선을 그려준다.
	DrawLine(XPosA, YPosA, XPosB, YPosB);
	DrawLine(XPosA, YPosA, XPosC, YPosC);
	DrawLine(XPosC, YPosC, XPosB, YPosB);
	//삼각형을 칠해준다.
	if((YPosA>YPosB&&YPosC==YPosA)||(YPosA<YPosB&&YPosC==YPosB))
		FillTopFlatTriangle(XPosA, YPosA, XPosB, YPosB, XPosC, YPosC);
	else if (YPosA == YPosC || YPosA == YPosB)
		FillBottomFlatTriangle(XPosA, YPosA, XPosB, YPosB, XPosC, YPosC);
	else // 어느곳도 평평하지 않은 삼각형일 때
	{
		// 삼각형을 두 개의 평평한 삼각형으로 나눠줄 선을 그리기 위해 임의의 점 D의 좌표를 찾아준다.
		i32 XPosD = XPosC + ((YPosB - YPosC) / (YPosA - YPosC)) * (XPosA - XPosC); 
		i32 YPosD = YPosB;
	    // D를 기준으로 삼각형 두 개를 칠해준다.
		FillBottomFlatTriangle(XPosC, YPosC, XPosB, YPosB, XPosD, YPosD);
		FillTopFlatTriangle(XPosB, YPosB, XPosD, YPosD, XPosA, YPosA);
	}
}

void SoftRenderer::SortVerticesByY(i32 &XPosA, i32 &YPosA, i32 &XPosB, i32 &YPosB, i32 &XPosC, i32 &YPosC)
{
	// Y의 크기에 따라 정렬해주는 함수. A<B<C순으로 정렬

	if (YPosC >= YPosA && YPosC >= YPosB) // C가 가장 클 때
	{
		if (YPosB < YPosA) // A가 B보다 크다면 스왑
		{
			Swap(YPosA, YPosB);
			Swap(XPosA, XPosB);
			
		}
	}
	else if(YPosB >= YPosA && YPosB >= YPosC) // B가 가장 클 때
	{
		// B와 C를 스왑
		Swap(YPosC, YPosB);
		Swap(XPosC, XPosB);

		if(YPosA > YPosB) // 만약 A가 B보다 크다면 스왑
		{
			Swap(YPosA, YPosB);
			Swap(XPosA, XPosB);
		}
	}
	else // A가 가장 클 때
	{
		// A와 C를 스왑
		Swap(YPosA, YPosC);
		Swap(XPosA, XPosC);

		if (YPosA > YPosB) // 만약 A가 B보다 크다면 스왑
		{
			Swap(YPosA, YPosB);
			Swap(XPosA, XPosB);
		}
	}
	if (XPosB < XPosA)
	{
		// 시계방향으로 만들어주기 위해서
		Swap(YPosC, YPosB);
		Swap(XPosC, XPosB);
	}
	
}

void SoftRenderer::FillTopFlatTriangle(i32 XPosA, i32 YPosA, i32 XPosB, i32 YPosB, i32 XPosC, i32 YPosC)
{
	float slope1 = (float)(XPosB - XPosA) / (float)(YPosB - YPosA);
	float slope2 = (float)(XPosC - XPosA) / (float)(YPosC - YPosA);

	int x1, x2;

	for (int y = YPosA; y <= YPosB; y++) 
	{
		x1 = XPosA + (int)(slope1 * (y - YPosA));
		x2 = XPosA + (int)(slope2 * (y - YPosA));
		DrawHorizontalLine(x1, x2, y);
	}
}

void SoftRenderer::FillBottomFlatTriangle(i32 XPosA, i32 YPosA, i32 XPosB, i32 YPosB, i32 XPosC, i32 YPosC)
{
	float slope1 = (float)(XPosC - XPosA) / (float)(YPosC - YPosA);
	float slope2 = (float)(XPosC - XPosB) / (float)(YPosC - YPosB);

	int x1, x2;

	for (int y = YPosC; y > YPosA; y--)
	{
		x1 = XPosC - (int)(slope1 * (y - YPosC));
		x2 = XPosC - (int)(slope2 * (y - YPosC));
		DrawHorizontalLine(x1, x2, y);
	}
}

void SoftRenderer::UpdateFrame()
{
	// Buffer Clear
	mGDIHelper->SetColor(80, 80, 200); // 바탕의 색깔을 결정해줍니다
	mGDIHelper->Clear();

	// Draw vertical line
	mGDIHelper->SetColor(255, 0, 0); // 선의 색깔을 지정해줍니다.
	// DrawLine(-200, -100, 100, 100);
	
	DrawTriangle(100, 300, -300, 100, 200, 200);
	// Buffer Swap 
	mGDIHelper->BufferSwap();
}
