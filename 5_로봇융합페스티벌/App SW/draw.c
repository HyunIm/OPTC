#include "draw.h"
#include "RGB.h"

/* (x,y)를 중심으로 십자모양을 그리는 함수, length = 길이, thick = 두께
 여기서 buf는 RGB565를 기준으로 한다. */
void drawCross(unsigned short *buf, int x, int y, int length, int thick, int width, int height)
{
	int i, j;
	int index;

	int leftX, topY;
	int rightX, bottomY;

	int tHick = thick>>1;
	int thickLeft, thickRight;
	int thickTop, thickBottom;

	unsigned short color = MAKE_RGB565(255, 0, 0);

	leftX = x - length;
	rightX = x + length;
	topY = y - length;
	bottomY = y + length;

	if(leftX < 0)
		leftX = 0;

	if(rightX > width)
		rightX = width;

	if(topY < 0)
		topY = 0;

	if(bottomY > height)
		bottomY = height;

	thickLeft = x - tHick;
	thickRight = x + tHick;
	thickTop = y - tHick;
	thickBottom = y + tHick;

	if(thickLeft < 0)
		thickLeft = 0;

	if(thickRight > width)
		thickRight = width;

	if(thickTop < 0)
		thickTop = 0;

	if(thickBottom > height)
		thickBottom = height;

	for(i = thickTop; i < thickBottom; i++)
	{
		index = i*width;
		for(j = leftX; j < rightX; j++)
		{
			buf[index + j] = color;
		}
	}
	
	for(i = topY; i < bottomY; i++)
	{
		index = i*width;
		for(j = thickLeft; j < thickRight; j++)
		{
			buf[index + j] = color;
		}
	}
}

void drawCross_mid(unsigned short *buf, int x, int y, int length, int thick, int width, int height)
{
	int i, j;
	int index;

	int leftX, topY;
	int rightX, bottomY;

	int tHick = thick>>1;
	int thickLeft, thickRight;
	int thickTop, thickBottom;

	unsigned short color = MAKE_RGB565(0, 255, 0);

	leftX = x - length;
	rightX = x + length;
	topY = y - length;
	bottomY = y + length;

	if(leftX < 0)
		leftX = 0;

	if(rightX > width)
		rightX = width;

	if(topY < 0)
		topY = 0;

	if(bottomY > height)
		bottomY = height;

	thickLeft = x - tHick;
	thickRight = x + tHick;
	thickTop = y - tHick;
	thickBottom = y + tHick;

	if(thickLeft < 0)
		thickLeft = 0;

	if(thickRight > width)
		thickRight = width;

	if(thickTop < 0)
		thickTop = 0;

	if(thickBottom > height)
		thickBottom = height;

	for(i = thickTop; i < thickBottom; i++)
	{
		index = i*width;
		for(j = leftX; j < rightX; j++)
		{
			buf[index + j] = color;
		}
	}
	
	for(i = topY; i < bottomY; i++)
	{
		index = i*width;
		for(j = thickLeft; j < thickRight; j++)
		{
			buf[index + j] = color;
		}
	}
}


/* (x,y): 왼쪽 상단의 꼭지점 위치
(x,y)를 시작으로 width너비에 height높이 그리고 thick의 두께의 사각형을 그린다. */
void drawRectangle(unsigned short *buf, int x, int y, int x2, int y2, int thick, int width, int height)
{
	int width_1 = x2-x;
	int height_1 = y2-y;
	int i, j;
	int index, index2;

	unsigned short color = MAKE_RGB565(255, 100, 100);

	if( x + width_1 + thick < width && y + height_1 + thick < height )
	{
		for(i=0;i<thick;i++)
		{
			index = (y+i)*width;
			index2 = (y+height_1 - 1 - i)*width;

			for(j=x;j<width_1+x;j++)
			{
				buf[index + j] = color;
				buf[index + j] = color;
			}
		}
		for(i=y;i<height_1+y;i++)
		{
			index = i*width;
	
			for(j=0;j<thick;j++)
			{
				buf[index + (x+j)] = color;
				buf[index + (x+width_1-1-j)] = color;
			}
		}
		
	}
	else
	{
		for(j=0;j<thick;j++)
		{
			index = (y+j)*width;
			index2 = (x+j);

			for(i=x;i<width;i++)
			{
				buf[index + i] = color;
			}
			for(i=y;i<height;i++)
			{
				buf[i*width + index2] = color;
			}		
		}
	}
}

/* (x,y)를 중심으로 radious의 반지름의 원을 그린다. */
void drawCircle(unsigned short *buf, int x, int y, int radious, int width, int height)
{
	int startX = (x-radious) >= 0 ? (x-radious) : 0;
	int startY = (y-radious) >= 0 ? (y-radious) : 0;

	int endX = (x+radious) < width ? (x+radious) : width-1;
	int endY = (y+radious) < height ? (y+radious) : height-1;

	int tmp;
	int i, j;
	int index;

	unsigned short color = MAKE_RGB565(255, 100, 100);

	for(i = startY ; i < endY ; i ++)
	{
		index = i*width;
		for (j = startX ; j < endX ; j ++ )
		{
			tmp = ((x-j) * (x-j)) + ((y-i) * (y-i)) - radious * radious;

			if(tmp >= 30-radious && tmp <= 30+radious)
			{
				buf[index + j] = color;
			}
		}
	}
}
