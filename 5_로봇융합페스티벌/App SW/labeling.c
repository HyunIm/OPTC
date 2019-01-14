#include <stdio.h>

#include "process.h"
#include "labeling.h"

void labeling(int iHeight, int iWidth, int *label_num, BYTE *pImg, FindObject *object)
{
	int i, j;
//	int count = 0;	// 접근 횟수를 알아보기 위한 숫자 필요 X
	int iFlag = 0, jFlag = 0;

	/* 물체의 정보 초기화 */
	object[(*label_num)].areaSize = 1;
	object[(*label_num)].leftX = endX;
	object[(*label_num)].rightX = startX;
	object[(*label_num)].bottomY = endY;
	object[(*label_num)].topY = startY;

	for(i = startY + 1; i < endY - 1; i++)
	{
		for(j = startX + 1; j < endX - 1; j++)
		{
		//	count++;
			iFlag = 0;
			jFlag = 0;
			if(pImg[i * WIDTH + j] == (*label_num))
			{
				/* 기준점에 대해 아래쪽 검사 */
				if(pImg[(i - 1) * WIDTH + (j)] == BLACK)
				{
					pImg[(i - 1) * WIDTH + (j)] = (*label_num);
					object[(*label_num)].areaSize++;
					
					if(object[(*label_num)].bottomY > (i - 1))
						object[(*label_num)].bottomY = i - 1;
					if(object[(*label_num)].topY < (i - 1))
						object[(*label_num)].topY = i - 1;
					if(object[(*label_num)].leftX > (j))
						object[(*label_num)].leftX = j;
					if(object[(*label_num)].rightX < (j))
						object[(*label_num)].rightX = j;

					iFlag = 1;
				}

				/* 기준점에 대해 위쪽 검사 */
				if(pImg[(i + 1) * WIDTH + (j)] == BLACK)
				{
					pImg[(i + 1) * WIDTH + (j)] = (*label_num);
					object[(*label_num)].areaSize++;

					if(object[(*label_num)].bottomY > (i + 1))
						object[(*label_num)].bottomY = i + 1;
					if(object[(*label_num)].topY < (i + 1))
						object[(*label_num)].topY = i + 1;
					if(object[(*label_num)].leftX > (j))
						object[(*label_num)].leftX = j;
					if(object[(*label_num)].rightX < (j))
						object[(*label_num)].rightX = j;
				}

				/* 기준점에 대해 왼쪽 검사 */
				if(pImg[(i) * WIDTH + (j - 1)] == BLACK)
				{
					pImg[(i) * WIDTH + (j - 1)] = (*label_num);
					object[(*label_num)].areaSize++;

					if(object[(*label_num)].bottomY > (i))
						object[(*label_num)].bottomY = i;
					if(object[(*label_num)].topY < (i))
						object[(*label_num)].topY = i;
					if(object[(*label_num)].leftX > (j - 1))
						object[(*label_num)].leftX = j - 1;
					if(object[(*label_num)].rightX < (j - 1))
						object[(*label_num)].rightX = j - 1;

					jFlag = 1;
				}

				/* 기준점에 대해 오른쪽 검사 */
				if(pImg[(i) * WIDTH + (j + 1)] == BLACK)
				{
					pImg[(i) * WIDTH + (j + 1)] = (*label_num);
					object[(*label_num)].areaSize++;

					if(object[(*label_num)].bottomY > (i))
						object[(*label_num)].bottomY = i;
					if(object[(*label_num)].topY < (i))
						object[(*label_num)].topY = i;
					if(object[(*label_num)].leftX > (j + 1))
						object[(*label_num)].leftX = j + 1;
					if(object[(*label_num)].rightX < (j - 1))
						object[(*label_num)].rightX = j - 1;
				}
				
				if(iFlag)
				{
					i = i - 1;
					j = j - 1;
				}
				if(jFlag)
					j = j - 2;

				if(i < startY)
					i = startY;
				if(j < startX)
					j = startX - 1;

			}
		}
	}

	/* 물체로 인식된 영역이 30픽셀보다 작으면 물체로 인식하지 않는다. */
	//if(object[(*label_num)].areaSize < 20 || object[(*label_num)].areaSize > 700)
	if(object[(*label_num)].areaSize < 50)
	{
		/* 라벨 영역이라고 취급하지 않으면 WHITE로 변경 */
		/*for(i = startY + 1; i < endY - 1; i++)
		{
			for(j = startX + 1; j < endX - 1; j++)
			{
				if(pImg[i * WIDTH + j] == (*label_num))
					pImg[i * WIDTH + j] = WHITE;
			}
		}*/
		(*label_num)--;
	}

	else
	{
		printf("%d %d\n", *label_num, object[(*label_num)].areaSize);
		//printf("lx %d, rx %d,ty %d, by %d\n", object[(*label_num)].leftX, object[(*label_num)].rightX, object[(*label_num)].topY, object[(*label_num)].bottomY);
		object[(*label_num)].midX = (object[(*label_num)].rightX + object[(*label_num)].leftX) >> 1;
		object[(*label_num)].midY = (object[(*label_num)].topY + object[(*label_num)].bottomY) >> 1;
	}

	return ;
}

/* 라벨링할 영역의 번호를 조절하는 함수 */
int callLabel(int iHeight, int iWidth, BYTE *pImg, FindObject *object)
{
	int i, j;
	int labelNum = 1;

	for(i = startY + 1; i < endY - 1; i++)
	{
		for(j = startX + 1; j < endX - 1; j++)
		{
			if(pImg[i * iWidth + j] == BLACK)
			{
				pImg[i * iWidth + j] = labelNum;
				/* 번호 매기는 함수 호출 */
				labeling(iHeight, iWidth, &labelNum, pImg, object);
				/* 라벨번호 증가 */
				labelNum++;
			}
		}
	}

	return labelNum;
}