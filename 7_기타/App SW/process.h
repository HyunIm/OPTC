// 왼쪽 bigY
// 오른쪽 smallY
// 아래쪽 bigX
// 위쪽 smallX

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

/* 물체의 정보를 기록하는 구조체 선언 */
typedef struct Object {
	int areaSize;		// 물체의 영역 크기
	int leftX;
	int rightX;
	int topY;
	int bottomY;
	int midX;			// 영역의 중간X 값
	int midY;			// 영역의 중간Y 값
}FindObject;

#endif // __PROCESS_H__