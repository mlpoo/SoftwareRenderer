#pragma once

class MathLibrary
{
	class Vector2
	{
	private:
		float x;
		float y;

	public:
		Vector2()
		{
			x = 0.f;
			y = 0.f;
		};
		Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		};

		Vector2 set(float x, float y) // vectorÀÇ °ªÀ» ¼³Á¤ÇÏ´Â ÇÔ¼ö
		{
			this->x = x;
			this->y = y;
		}
		float getX()
		{
			return this->x;
		}
		float getY()
		{
			return this->y;
		}
		Vector2 add(float x, float y) // vectorÀÇ µ¡¼À
		{
			this->x += x;
			this->y += y;
		}
		Vector2 sub(float x, float y) // vectorÀÇ »¬¼À
		{
			this->x -= x;
			this->y -= y;
		}
		Vector2 mul(float scalar) // vectorÀÇ °ö¼À
		{
			this->x *= scalar;
			this->y *= scalar;
		}
	};
};

