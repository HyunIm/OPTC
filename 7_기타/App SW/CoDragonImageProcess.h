
#ifndef __CODRAGONIMAGEPROCESS_H__
#define __CODRAGONIMAGEPROCESS_H__

/* 영상처리에 사용되는 함수들 선언 */
void rgb2Hsi(WORD *inImg, WORD *hImg, BYTE *sImg, BYTE *iImg);
void differenceImg(BYTE *firstImg, BYTE *secondImg, BYTE *pImg);
void checkArea(BYTE *pImg, int *left, int *right, int *top, int *bottom);
void copyImg(BYTE *pImg, BYTE *bufImg);
void clearImg(BYTE *pImg);
void sBinary(BYTE *pImg, BYTE *iImg);
void iBinary(BYTE *pImg, BYTE *iImg);
void siBinary(BYTE *pImg, BYTE *sImg, BYTE *iImg);
void hBinary(BYTE *pImg, WORD *hImg, BYTE *sImg);
void laplacian(BYTE *in, BYTE *buf);
void dilation(BYTE *in, BYTE *buf);
void eroion(BYTE *pImg, BYTE *buf);
void drawCross(WORD *img);
void drawQuadangle(WORD *outImg, int labelNum, FindObject *object);
int checkGreen(BYTE *pImg, int labelNum, FindObject *object, WORD *outImg);
int distance(int x, int y);
int outputImg(BYTE *pImg, WORD *outImg);
void someMask(BYTE *pImg, BYTE *buf);

#endif	// __CODRAGONIMAGEPROCESS_H__
