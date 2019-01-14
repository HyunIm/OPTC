#ifndef __LABELING_H__
#define __LABELING_H__

void labeling(int iHeight, int iWidth, int *label_num, BYTE *pImg, FindObject *object);
int callLabel(int iHeight, int iWidth, BYTE *pImg, FindObject *object);

#endif // __LABELING_H__
