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
	int AddX = 0, AddY = 0; // ������ ������⿡ ���� 1 �Ǵ� -1�� �ȴ�.
	int x = StartXPos, y = StartYPos; // ���� �׸��鼭 �� ��ǥ�� ���̴�.
	int Counter = 0; // x�Ǵ� y�� ���� ������ �� ����Ѵ�.
	int W = EndXPos - StartXPos; // �ʺ� ���մϴ�.
	int H = EndYPos - StartYPos; // ���̸� ���մϴ�.

	if (W < 0) 
	{
		AddX = -1; // ��������(���ʹ���)�̶�� x�� ���� -1�� �����ǰ� ���ݴϴ�.
		W = -W; // �ʺ� -1�� ���� �ùٸ��� �����ݴϴ�.
	}
	else
	{
		AddX = 1;
	}

	if (H < 0)
	{
		AddY = -1; // ��������(�Ʒ�����)�̶�� y�� ���� -1�� �����ǰ� ���ݴϴ�.
		H = -H; // ���̿� -1�� ���� �ùٸ��� �����ݴϴ�.
	}
	else
	{
		AddY = 1;
	}

	if (W >= H) // �ʺ� �� ũ�ٸ�
	{
		for (int i = 0; i < W; i++) // �ʺ�ŭ �ݺ����ش�.
		{
			x += AddX;
			Counter += H; // ���⸦ ���ϴ� �Ͱ� ������ ����ȭ�� ���� ���ǹ��� �Բ� �ٲ����.

			if (Counter >= W) // ī������ ���� W�� �Ѵ´ٸ�
			{
				y += AddY; // y�� ���� AddY�� ������ ��
				Counter -= W; // ī���Ϳ��� �ʺ�ŭ ���ش�.
			}
			DrawPixel(x, y);
		}
	}
	else // ���̰� �� ũ�ٸ�
	{
		for (int i = 0; i < H; i++) // ���̸�ŭ �ݺ����ش�.
		{
			y += AddY;
			Counter += W;

			if (Counter >= H) // ī������ ���� H�� �Ѵ´ٸ�
			{
				x += AddX; // x�� ���� AddX�� ������ ��
				Counter -= H; // ī���Ϳ��� �ʺ�ŭ ���ش�.
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
	// ��ǥ�� �������ش�.
	SortVerticesByY(XPosA, YPosA, XPosB, YPosB, XPosC, YPosC);
	// �ܰ����� �׷��ش�.
	DrawLine(XPosA, YPosA, XPosB, YPosB);
	DrawLine(XPosA, YPosA, XPosC, YPosC);
	DrawLine(XPosC, YPosC, XPosB, YPosB);
	//�ﰢ���� ĥ���ش�.
	if((YPosA>YPosB&&YPosC==YPosA)||(YPosA<YPosB&&YPosC==YPosB))
		FillTopFlatTriangle(XPosA, YPosA, XPosB, YPosB, XPosC, YPosC);
	else if (YPosA == YPosC || YPosA == YPosB)
		FillBottomFlatTriangle(XPosA, YPosA, XPosB, YPosB, XPosC, YPosC);
	else // ������� �������� ���� �ﰢ���� ��
	{
		// �ﰢ���� �� ���� ������ �ﰢ������ ������ ���� �׸��� ���� ������ �� D�� ��ǥ�� ã���ش�.
		i32 XPosD = XPosC + ((YPosB - YPosC) / (YPosA - YPosC)) * (XPosA - XPosC); 
		i32 YPosD = YPosB;
	    // D�� �������� �ﰢ�� �� ���� ĥ���ش�.
		FillBottomFlatTriangle(XPosC, YPosC, XPosB, YPosB, XPosD, YPosD);
		FillTopFlatTriangle(XPosB, YPosB, XPosD, YPosD, XPosA, YPosA);
	}
}

void SoftRenderer::SortVerticesByY(i32 &XPosA, i32 &YPosA, i32 &XPosB, i32 &YPosB, i32 &XPosC, i32 &YPosC)
{
	// Y�� ũ�⿡ ���� �������ִ� �Լ�. A<B<C������ ����

	if (YPosC >= YPosA && YPosC >= YPosB) // C�� ���� Ŭ ��
	{
		if (YPosB < YPosA) // A�� B���� ũ�ٸ� ����
		{
			Swap(YPosA, YPosB);
			Swap(XPosA, XPosB);
			
		}
	}
	else if(YPosB >= YPosA && YPosB >= YPosC) // B�� ���� Ŭ ��
	{
		// B�� C�� ����
		Swap(YPosC, YPosB);
		Swap(XPosC, XPosB);

		if(YPosA > YPosB) // ���� A�� B���� ũ�ٸ� ����
		{
			Swap(YPosA, YPosB);
			Swap(XPosA, XPosB);
		}
	}
	else // A�� ���� Ŭ ��
	{
		// A�� C�� ����
		Swap(YPosA, YPosC);
		Swap(XPosA, XPosC);

		if (YPosA > YPosB) // ���� A�� B���� ũ�ٸ� ����
		{
			Swap(YPosA, YPosB);
			Swap(XPosA, XPosB);
		}
	}
	if (XPosB < XPosA)
	{
		// �ð�������� ������ֱ� ���ؼ�
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
	mGDIHelper->SetColor(80, 80, 200); // ������ ������ �������ݴϴ�
	mGDIHelper->Clear();

	// Draw vertical line
	mGDIHelper->SetColor(255, 0, 0); // ���� ������ �������ݴϴ�.
	// DrawLine(-200, -100, 100, 100);
	
	DrawTriangle(100, 300, -300, 100, 200, 200);
	// Buffer Swap 
	mGDIHelper->BufferSwap();
}
