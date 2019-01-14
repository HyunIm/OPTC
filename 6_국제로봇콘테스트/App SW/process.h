// ���� bigY
// ������ smallY
// �Ʒ��� bigX
// ���� smallX

#ifndef __PROCESS_H__
#define __PROCESS_H__

#define WHITE               255
#define BLACK               0
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define WIDTH               180
#define HEIGHT              120
#define startX				0
#define endX				180
#define startY				0
#define endY				180
#define INDEXWIDTH          256
#define INDEXHEIGHT         128

/* ��ü�� ������ ����ϴ� ����ü ���� */
typedef struct Object {
	int areaSize;		// ��ü�� ���� ũ��
	int leftX;
	int rightX;
	int topY;
	int bottomY;
	int midX;			// ������ �߰�X ��
	int midY;			// ������ �߰�Y ��
}FindObject;

#endif // __PROCESS_H__