#include "process.h"
#include "CoDragonImageProcess.h"
#include "hImg.h"
#include "sImg.h"
#include "iImg.h"
#include <stdio.h>

/* RGB565를 LookUpTable에 맞춰 HSI를 변환 */
void rgb2Hsi(WORD *inImg, WORD *hImg, BYTE *sImg, BYTE *iImg)
{
	int i, j;

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			hImg[i * WIDTH + j] = h_lookup[inImg[i * INDEXWIDTH + j]];
			sImg[i * WIDTH + j] = s_lookup[inImg[i * INDEXWIDTH + j]];
			iImg[i * WIDTH + j] = i_lookup[inImg[i * INDEXWIDTH + j]];
		}
	}

	return;
}

/* 원래의 데이터를 buf로 복사 */
void copyImg(BYTE *pImg, BYTE *bufImg)
{
	int i, j;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			bufImg[i * WIDTH + j] = pImg[i * WIDTH + j];
		}
	}

	return;
}

void clearImg(BYTE *pImg)
{
	int i, j;

	for (i = startY; i < endY; i++)
	{
		for (j = startX; j < endX; j++)
		{
			pImg[i * WIDTH + j] = WHITE;
		}
	}

	return;
}

/* s값으로 이진화 */
void sBinary(BYTE *pImg, BYTE *sImg)
{
	int i, j, index;
	int thresholdS = 200;
	int thresholdl = 50;

	for (i = startY; i < endY; i++)
	{
		index = i*WIDTH;
		for (j = startX; j < endX; j++)
		{
			if (sImg[index + j] < thresholdS && sImg[index + j] > thresholdl)
				pImg[index + j] = BLACK;
			else
				pImg[index + j] = WHITE;
		}
	}

	return;
}

/* i값으로 이진화 */
void iBinary(BYTE *pImg, BYTE *iImg)
{
	int i, j, index;
	int thresholdI = 50;

	for (i = startY; i < endY; i++)
	{
		index = i*WIDTH;
		for (j = startX; j < endX; j++)
		{
			if (iImg[index + j] < thresholdI)
				pImg[index + j] = BLACK;
			else
				pImg[index + j] = WHITE;
		}
	}

	return;
}

/* s와 i값으로 이진화 */
// 모터 찾기
void siBinary(BYTE *pImg, BYTE *sImg, BYTE *iImg)
{
	int i, j;
	int thresholdS = 60;
	int thresholdI = 40;

	for (i = startY; i < endY; i++)
	{
		for (j = startX; j < endX; j++)
		{
			if (sImg[i * WIDTH + j] < thresholdS && iImg[i * WIDTH + j] < thresholdI)
				pImg[i * WIDTH + j] = BLACK;
			else
				pImg[i * WIDTH + j] = WHITE;
		}
	}

	return;
}

// 로봇 찾기
void hBinary(BYTE *pImg, WORD *hImg, BYTE *sImg)
{
	int i, j, index;
	int threshold1 = 115;
	int threshold2 = 145;


	for (i = startY - 5; i < endY + 5; i++)
	{
		index = i*WIDTH;
		for (j = startX - 5; j < endX + 5; j++)
		{
			if (hImg[index + j] >= threshold1 && hImg[index + j] <= threshold2 && sImg[index + j] >= 50 && pImg[index + j] >= 50 && (j>startX + 10 && j<endX + 2))
				pImg[index + j] = BLACK;
			else
				pImg[index + j] = WHITE;
		}
	}

	return;
}

/* 라플라시안 마스크 처리 -> 경계 추출 */
void laplacian(BYTE *pImg, BYTE *buf)
{
	int i, j, mr, mc;
	int mask[3][3] = { { -1, -1, -1 },
	{ -1, 8, -1 },
	{ -1, -1, -1 } };
	int newValue, max, min;

	for (i = startY; i < endY; i++)
	{
		for (j = startX; j < endX; j++)
		{
			buf[i * WIDTH + j] = BLACK;
		}
	}

	max = (int)-10e10;
	min = (int)10e10;

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			newValue = 0;
			for (mr = 0; mr < 3; mr++)
			{
				for (mc = 0; mc < 3; mc++)
				{
					newValue += pImg[(i - mr - 1) * WIDTH + (j - mc - 1)] * mask[mr][mc];
				}
			}

			if (newValue < 0)
				newValue = -newValue;

			if (max < newValue)
				max = newValue;
			if (min > newValue)
				min = newValue;

			buf[i * WIDTH + j] = newValue;
		}
	}

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			newValue = buf[i * WIDTH + j];
			//newValue = newValue * constVal1 + constVal2;
			//pImg[i * width + j] = newValue;
			newValue = (buf[i * WIDTH + j] - min) * 255 / (max - min);
			/*if(newValue > 20)
			newValue = WHITE;
			else
			newValue = BLACK;*/
			if (newValue > 255)
				newValue = 255;
			pImg[i * WIDTH + j] = newValue;
		}
	}

	//printf("min %d\nmax %d\n", min, max);

	return;
}

/* 영상을 침식 */
void dilation(BYTE *pImg, BYTE *buf)
{
	int i, j, mr, mc;
	int flag;

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			if (pImg[i * WIDTH + j] == BLACK)
			{
				flag = 9;
				for (mr = 0; mr < 3; mr++)
				{
					for (mc = 0; mc < 3; mc++)
					{
						if (pImg[(i - mr - 1) * WIDTH + (j - mc - 1)] == WHITE)
							flag--;
					}
				}
				if (flag < 5)
					buf[i * WIDTH + j] = WHITE;
				else
					buf[i * WIDTH + j] = BLACK;
			}
			else
				buf[i * WIDTH + j] = WHITE;
		}
	}

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			pImg[i * WIDTH + j] = buf[i * WIDTH + j];
		}
	}

	return;
}

/* 영상을 팽창 */
void erosion(BYTE *pImg, BYTE *buf)
{
	int i, j, mr, mc;
	int flag;

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			if (pImg[i * WIDTH + j] == WHITE)
			{
				flag = 9;
				for (mr = 0; mr < 3; mr++)
				{
					for (mc = 0; mc < 3; mc++)
					{
						if (pImg[(i - mr - 1) * WIDTH + (j - mc - 1)] == BLACK)
							flag--;
					}
				}
				if (flag < 8)
					buf[i * WIDTH + j] = BLACK;
				else
					buf[i * WIDTH + j] = WHITE;
			}
			else
				buf[i * WIDTH + j] = BLACK;
		}
	}

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			pImg[i * WIDTH + j] = buf[i * WIDTH + j];
		}
	}

	return;
}


void someMask(BYTE *pImg, BYTE *buf)
{
	int mr, mc, i, j, newValue, max, min;
	int	mask[3][3] = { -2, -2, 0,
		-3, 19, -2,
		-2, -2, -3 };

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			newValue = 0;
			for (mr = 0; mr < 3; mr++)
			{
				for (mc = 0; mc < 3; mc++)
				{
					newValue += pImg[(i - mr - 1) * WIDTH + (j - mc - 1)] * mask[mr][mc];
				}

			}
			if (newValue < 0)
				newValue = -newValue;

			if (max < newValue)
				max = newValue;
			if (min > newValue)
				min = newValue;

			buf[i * WIDTH + j] = newValue;
		}
	}

	for (i = startY + 1; i < endY - 1; i++)
	{
		for (j = startX + 1; j < endX - 1; j++)
		{
			pImg[i * WIDTH + j] = buf[i * WIDTH + j];
		}
	}

	return;
}

/* 영상의 정 가운데에 +을 출력 */
void drawCross(WORD *img)
{
	int i, j;
	int red, green, blue;

	for (i = (HEIGHT >> 1) - 10; i < (HEIGHT >> 1) + 10; i++)
	{
		red = (255 >> 3) & 0x1F;
		green = (0 >> 2) & 0x3F;
		blue = (0 >> 3) & 0x1F;

		img[i * INDEXWIDTH + (WIDTH >> 1)] = ((red << 11) | (green << 5) | (blue));
	}

	for (j = (WIDTH >> 1) - 10; j < (WIDTH >> 1) + 10; j++)
	{
		red = (255 >> 3) & 0x1F;
		green = (0 >> 2) & 0x3F;
		blue = (0 >> 3) & 0x1F;
		img[(HEIGHT >> 1) * INDEXWIDTH + j] = ((red << 11) | (green << 5) | (blue));
	}

	return;
}

void drawQuadangle(WORD *outImg, int labelNum, FindObject *object)
{
	int i, j, k;
	int red, green, blue;

	for (k = 1; k < labelNum; k++)
	{
		for (i = object[k].bottomY; i < object[k].topY; i++)
		{
			red = (255 >> 3) & 0x1F;
			green = (0 >> 2) & 0x3F;
			blue = (0 >> 3) & 0x1F;
			outImg[i * INDEXWIDTH + object[k].leftX] = ((red << 11) | (green << 5) | (blue));
			outImg[i * INDEXWIDTH + object[k].rightX] = ((red << 11) | (green << 5) | (blue));
		}
		for (j = object[k].leftX; j < object[k].rightX; j++)
		{
			red = (255 >> 3) & 0x1F;
			green = (0 >> 2) & 0x3F;
			blue = (0 >> 3) & 0x1F;
			outImg[object[k].bottomY * INDEXWIDTH + j] = ((red << 11) | (green << 5) | (blue));
			outImg[object[k].topY * INDEXWIDTH + j] = ((red << 11) | (green << 5) | (blue));
		}
	}

	return;
}

int checkGreen(BYTE *pImg, int labelNum, FindObject *object, WORD *outImg)
{
	int i, j, k;
	int red, green, blue;
	int count = 0;
	int min = 0x0FFFFFFF, returnPoint = 0;

	for (k = 1; k < labelNum; k++)
	{
		count = 0;
		for (i = object[k].bottomY; i < object[k].topY + 5; i++)
		{
			for (j = object[k].leftX - 5; j < object[k].rightX + 5; j++)
			{
				if (pImg[i * WIDTH + j] == BLACK)
					count++;
			}
		}

		if (count > 35 || count * 100 / object[k].areaSize > 15)
		{
			int d = distance(object[k].midX, object[k].midY); // 이미지 중심에서 라벨의 중심
			if (min > d)
			{
				min = d;
				returnPoint = k;
			}
		}
	}

	for (i = object[returnPoint].bottomY; i < object[returnPoint].topY; i++)
	{
		for (j = object[returnPoint].leftX; j < object[returnPoint].rightX; j++)
		{
			red = (255 >> 3) & 0x1F;
			green = (0 >> 2) & 0x3F;
			blue = (0 >> 3) & 0x1F;
			outImg[i * INDEXWIDTH + j] = ((red << 11) | (green << 5) | (blue));
		}
	}

	return returnPoint;
}

int distance(int x, int y)
{
	int mX, mY;

	mX = (startX + endX) >> 1;
	mY = (startY + endY) >> 1;

	return (x - mX) * (x - mX) + (y - mY) *  (y - mY);
}

/* 처리된 영상을 LCD로 출력하기 위해 처리하는 함수 */
int outputImg(BYTE *pImg, WORD *outImg)
{
	int i, j, k;
	int red, green, blue;
	int midX, mid;
	int sole[5] = { 0, };

	midX = 0;
	k = 0;

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (pImg[i*WIDTH + j] == BLACK)
			{
				k++;
				midX += i;
				if (j >= 125) // 발바닥 어디있니  
				{
					if (j >= 170) sole[0]++;
					else if (j >= 160) sole[1]++;
					else if (j >= 150) sole[2]++;
					else if (j >= 140) sole[3]++;
					else if (j >= 125) sole[4]++;

				}
			}

			red = (pImg[i * WIDTH + j] >> 3) & 0x1F;
			green = (pImg[i * WIDTH + j] >> 2) & 0x3F;
			blue = (pImg[i * WIDTH + j] >> 3) & 0x1F;

			outImg[i * INDEXWIDTH + j] = ((red << 11) | (green << 5) | (blue));
		}

	}
	midX /= k;

	//printf("[X :%d]\n", midX);

	if (midX < 35) k += 500000;
	else if (midX < 50) k += 400000;
	else if (midX > 110) k += 100000;
	else if (midX > 95) k += 200000;
	else  k += 300000;

	if (sole[0] >= 5) k += 1000000;
	else if (sole[1] >= 5) k += 2000000;
	else if (sole[2] >= 5) k += 3000000;
	else if (sole[3] >= 5) k += 4000000;
	else if (sole[4] >= 5) k += 5000000;

	return k;
}