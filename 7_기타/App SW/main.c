#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>

#include "process.h"
#include "uart_api.h"
#include "cam_disp.h"
#include "CoDragonRobot_protocol.h"
#include "CoDragonImageProcess.h"
#include "labeling.h"


// 통신 문제시 리붓 3초로 증가
#define DISABLE_IMG_IRQ		0x24402
#define RD_IMG_DATA			0x24403
#define DISPLAY_IMG_DATA	0x24404
#define DISPLAY_IMG_DRAW	0x24405	
#define CLEAR_SCREEN		0x24407
#define GRAPHIC_FLIP		0x24410

/* 영상회전을 위한 구조체 선언 */
typedef struct __draw_raw_value 
{
	int InitDX;
	int InitDY;
	int EndX;
	int EndY;
	int InitSX;
	int InitSY;
	int dxSx;
	int dxSy;
	int dySx;
	int dySy;
	int imgbuf_en;
	WORD *imgbuf;
} DrawRaw_value;

/* 색상값을 저장하기 위한 구조체 선언 */
struct __shade_color
{
	BYTE r;
	BYTE g;
	BYTE b;
};

static DrawRaw_value draw_value;

FindObject _object[255];	// 물체의 정보를 기록할 배열 선언

int devfb;

/* 영상처리에 사용 될 변수 선언 */
WORD *in_Img;				// FPGA영상을 받는 변수
BYTE *buf_Img;				// 영상처리 할 때 사용되는 버퍼
WORD *h_Img;				// in_Img -> h
BYTE *s_Img;				// in_Img -> s
BYTE *i_Img;				// in_Img -> i
BYTE *p_Img;				// 영상처리된 내용을 저장
WORD *out_Img[5];				// 처리된 영상을 LCD로 출력


//280, 384
int InitSX[7] = { -3072, 0, 0, 512, 0, 0, -117760 };
int InitSY[7] = { 63488, -2048, -67584, 163840, -133120, -198666, 163840 };
int dxSx[7] = { 0, 256, 256, 0, 280, 512 ,0 };
int dxSy[7] = { -203, 0, 0, -511, 0, 0, -511 };
int dySx[7] = { 203, 0, 0, 511, 0, 0, 511 };
int dySy[7] = { 0, 256, 256, 0, 384, 512, 0 };

void memoryAllocate(void);
int draw_rotate_value(int cdx, int cdy, int ctx, int cty, float zoom, DWORD angle);
int draw_rotate_img(int InitSX, int InitSY, int dxSx, int dxSy, int dySx, int dySy);
void draw_img_to_lcd(WORD *Buffer, int lcdNumber);
int draw_img_from_buffer(WORD *Buffer, int CenterX, int CenterY, float zoom, DWORD rotate_angle);
void ClearScreen(BYTE r, BYTE g, BYTE b);
static int gFlip(void);
int ImageProcess_Open(void);
void ReadImageFromFPGA(void);
void ImgDisplayQuit(void);

int main(void)
{
	int ret;
	int labelNum = 0;
	int i, k;
	int reverse = 0;
	int direction = 0;
	int distance = 0;
	int size = 0;
	//BYTE buf[HOVIS_SENSOR_RECEIVE_PACKET];
	short accX, accY, accZ;
	
	printf("main start\r\n");
	memoryAllocate();		// 영상처리에 사용되는 메모리 할당

	eagle_camera_off();		// 커널에서 동작되는 Camera OFF

	ret = uart_open();
	if(ret < 0)
		return EXIT_FAILURE;

	uart_config(UART1, 115200, 8, UART_PARNONE, 1);
	
	init_robot();			// 로봇에 toque를 거는 함수
	
	usleep(5000000);
	printf("toque end\r\n");

	ret = ImageProcess_Open();
	if(ret < 0)
		return EXIT_FAILURE;

	i = 1;
	sleep(1);

	//모션 때려박기
	/*Send_Play_Motion(0);
	usleep(200000);

	Send_Play_Motion(11);
	usleep(200000);
	Send_Play_Motion(29);
	usleep(200000);
	Send_Play_Motion(15);
	usleep(200000);
	Send_Play_Motion(15);
	usleep(200000);
	Send_Play_Motion(15);*/



	//회전

	while (1)
	{
		ReadImageFromFPGA();	// FPGA에서 Img를 불러 온다. (inImg)
		ClearScreen(WHITE, WHITE, WHITE);			// 화면 전체를 하얗게 뿌려준다.

		rgb2Hsi(in_Img, h_Img, s_Img, i_Img);	// RGB565를 HSI로 바꿔준다.

		copyImg(i_Img, p_Img);  //p 이진화 영상이 저장될 이미지
		hBinary(p_Img, h_Img, s_Img); //h로 이진화
		
		erosion(p_Img, buf_Img);
		dilation(p_Img, buf_Img);
		erosion(p_Img, buf_Img);

		k = outputImg(p_Img, out_Img[0]); //out_Img에 처리된 영상 저장


		//가까운데부터 1,2,3,4,5  멀면 0
		distance = k / 1000000;
		//왼쪽부터 1,2,3,4,5  오른쪽까지 중간:중간
		direction = k % 1000000 / 100000;
		size = k % 100000;

		printf("크기 : %d\r\ndistance : %d\r\ndirection : %d\r\n",size, distance, direction);

		/* 전투 종료 */
		draw_img_to_lcd(in_Img, 1);
		draw_img_to_lcd(out_Img[0], 2);
		gFlip();
		

		/* 전투 시작 */
		if (size < 10)
		{
			//고개돌리기
			if (i % 3 == 1) Send_Play_Motion(34);
			else if (i % 3 == 2) Send_Play_Motion(35);
			else if (i % 3 == 0) Send_Play_Motion(11);

			i++;
		}
		else
		{
			if (distance >= 1 && direction == 3)	//공격
			{
				if (i % 3 == 1){	//중앙
					if (distance == 1){
						Send_Play_Motion(24);
					}
					else if (distance == 2){
						Send_Play_Motion(26);
					}
					else if (distance == 3){
						Send_Play_Motion(25);
					}
					else if (distance >= 4){
						Send_Play_Motion(33);
					}
					else if (distance == 5){
						printf("WalkAttack\r\n");
					}
				}
				else if (i % 3 == 2 ){	//왼쪽
					if (distance <= 2) Send_Play_Motion(27);
					else Send_Play_Motion(29);
				}
				else if (i % 3 == 0){	//오른쪽
					if (distance <= 2) Send_Play_Motion(41);
					else Send_Play_Motion(30);
				}
				Send_Play_Motion(18);
			}
			else if ( i%3 != 1 || direction != 3) // 회전
			{
				if (i % 3 == 2){
					Send_Play_Motion(29);
				}
				else if (i % 3 == 0) {
					Send_Play_Motion(30);
				}
				else{
					if (direction == 1)			Send_Play_Motion(29); //조금도는걸로수정필요
					else if (direction == 2)	Send_Play_Motion(29);
					else if (direction == 4)	Send_Play_Motion(30);
					else if (direction == 5)	Send_Play_Motion(13);
				}
				i = 1;
			}
			else	//걷기
			{
				Send_Play_Motion(15);
			}

		}
		//정면 4000이상, 측면 1500이상 공격

		usleep(100000);
	}	
	ImgDisplayQuit();		// LCD Display 종료

	uart_close();
	close(devfb);

	eagle_camera_on();		// 커널에서 동작되는 Camera ON

	return 0;
}

/* 영상처리에 사용되는 메모리 초기화 */
void memoryAllocate(void)
{
	in_Img = (WORD *)malloc(INDEXHEIGHT * INDEXWIDTH * sizeof(WORD));
	buf_Img = (BYTE *)malloc(HEIGHT * WIDTH);
	h_Img = (WORD *)malloc(HEIGHT * WIDTH * sizeof(WORD));
	s_Img = (BYTE *)malloc(HEIGHT * WIDTH);
	i_Img = (BYTE *)malloc(HEIGHT * WIDTH);
	p_Img = (BYTE *)malloc(HEIGHT * WIDTH);
	out_Img[0] = (WORD *)malloc(INDEXHEIGHT * INDEXWIDTH * sizeof(WORD));
	out_Img[1] = (WORD *)malloc(INDEXHEIGHT * INDEXWIDTH * sizeof(WORD));
	out_Img[2] = (WORD *)malloc(INDEXHEIGHT * INDEXWIDTH * sizeof(WORD));
	out_Img[3] = (WORD *)malloc(INDEXHEIGHT * INDEXWIDTH * sizeof(WORD));
	out_Img[4] = (WORD *)malloc(INDEXHEIGHT * INDEXWIDTH * sizeof(WORD));

	return ;
}

int draw_rotate_value(int cdx, int cdy, int ctx, int cty, float zoom, DWORD angle)
{
	float MySin,MyCos;
	int InitDX, InitDY;
	int EndX, EndY;
	int InitSX, InitSY;
	int dxSx, dxSy;
	int dySx, dySy;
	int dx,dy;

	int x,y;
	float a;
	int cosa;
	int sina;
	int rhzoom;

	x = y = 0;
	dx = 320;
	dy = 480;

	long tx,ty;

	angle %= 360;
	a = angle*(3.1415926535f*2)/360.0f;

	MySin = (float)sin(a);
	MyCos = (float)cos(a);

	tx = (-cdx / zoom) * MyCos + (-cdy / zoom) * MySin;
	ty = (cdx / zoom) * MySin + (-cdy / zoom) * MyCos;

	if(zoom <= 0)
		rhzoom = 0;
	else
		rhzoom = (int)((float)(1 << 9) / zoom);
	cosa = (int)(rhzoom * cos( a ));
	sina = (int)(rhzoom * sin( a ));





	if(dx <= 0 || dy <= 0)
		return -1;

	InitDX  = x;
	InitDY  = y;
	EndX    = x+dx-1;
	EndY    = y+dy-1;

	InitSX  = (x+tx+ctx)*512;
	dxSx    = cosa;
	dxSy    = -sina;

	InitSY  = (y+ty+cty)*512;
	dySx    = sina;
	dySy    = cosa;

	draw_value.InitDX=InitDX;
	draw_value.InitDY=InitDY;
	draw_value.EndX=EndX;
	draw_value.EndY=EndY;
	draw_value.InitSX=InitSX;
	draw_value.InitSY=InitSY;
	draw_value.dxSx=dxSx;
	draw_value.dxSy=dxSy;
	draw_value.dySx=dySx;
	draw_value.dySy=dySy;

	printf("InitDX=%d\r\n", InitDX);
	printf("InitDY=%d\r\n", InitDY);
	printf("EndX=%d\r\n", EndX);
	printf("EndY=%d\r\n", EndY);
	printf("InitSX=%d\r\n", InitSX);
	printf("InitSY=%d\r\n", InitSY);
	printf("dxSx=%d\r\n", dxSx);
	printf("dxSy=%d\r\n", dxSy);
	printf("dySx=%d\r\n", dySx);
	printf("dySy=%d\r\n", dySy);

	return 0;
}

/* 뿌려주는 영상의 위치를 잡아 주는 함수 */
int draw_rotate_img(int InitSX, int InitSY, int dxSx, int dxSy, int dySx, int dySy)
{
	draw_value.InitDX = 0;
	draw_value.InitDY = 0;
	draw_value.EndX = 319;
	draw_value.EndY = 479;
	draw_value.InitSX = InitSX;
	draw_value.InitSY = InitSY;
	draw_value.dxSx = dxSx;
	draw_value.dxSy = dxSy;
	draw_value.dySx = dySx;
	draw_value.dySy = dySy;
	draw_value.imgbuf_en = 1;

	return 0;
}

/* 입력받은 번호에 맞게 출력해주는 함수 */
void draw_img_to_lcd(WORD *Buffer, int lcdNumber)
{
	draw_rotate_img(InitSX[lcdNumber], InitSY[lcdNumber], dxSx[lcdNumber], dxSy[lcdNumber], dySx[lcdNumber], dySy[lcdNumber]);

	draw_value.imgbuf = Buffer;

	if(ioctl(devfb, DISPLAY_IMG_DRAW, &draw_value))
	{
		printf("Draw Image to LCD -> fail!!!\r\n");
		exit(1);
	}

	return ;
}

/* 예시 함수 출력 */
int draw_img_from_buffer(WORD *Buffer, int CenterX, int CenterY, float zoom, DWORD rotate_angle)
{
	draw_rotate_value(CenterX, CenterY, 90, 60, zoom, rotate_angle);
	draw_value.imgbuf = Buffer;
	draw_value.imgbuf_en = 1;

	if(ioctl(devfb, DISPLAY_IMG_DRAW, &draw_value)) 
	{
		printf("Draw Image to LCD -> fail!!!\r\n");
		exit(1);
	}

	return 0;
}

/* 원하는 색상에 맞춰 배경을 정리 */
void ClearScreen(BYTE r, BYTE g, BYTE b)
{
	struct __shade_color clear_shade;

	clear_shade.r = r;
	clear_shade.g = g;
	clear_shade.b = b;

	if(ioctl(devfb, CLEAR_SCREEN, &clear_shade))
	{
		printf("Clear Screen -> fail!!!\r\n");
		exit(1);
	}

	return ;
}

/* 위치에 출력된 영상을 LCD에 뿌려주는 함수 */
static int gFlip(void)
{
	return ioctl(devfb, GRAPHIC_FLIP, 0);
}

int ImageProcess_Open(void)
{
	int fb;
	fb = open("/mnt/f0/dev_module/imgproc", O_RDWR);

	if(fb < 0) 
	{
		printf("/mnt/f0/dev_module/imgproc open failed!!\r\n");
		return -1;
	}

	devfb = fb;

	return 0;
}

void ReadImageFromFPGA(void)
{
	if(ioctl(devfb, RD_IMG_DATA, &in_Img)) 
	{
		printf("ioctl RD_IMG_DATA error\r\n");
		exit(1);
	}

	return ;
}

void ImgDisplayQuit(void)
{
	ioctl(devfb, DISABLE_IMG_IRQ, 0);

	return ;
}
