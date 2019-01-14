#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/time.h>

#include "aop.h"
#include "hImg.h"
#include "sImg.h"
#include "iImg.h"
#include "camera.h"
#include "image.h"
#include "RGB.h"
#include "draw.h"
#include "find_pixel.h"

#undef	TIME_MEASURE

static struct image_ushort	inputRGB;
static struct image_ushort  inputHue;
static struct image_uchar   inputSaturation;
static struct image_uchar   inputIntensy;

void find_pixel(void)
{
    int r, c, index, index2;
	unsigned char iR, iG, iB;

	unsigned short rgb;
	
#ifdef	TIME_MEASURE
	struct timeval stime, etime;
	long usec, sec;
#endif

    initImage_ushort(&inputRGB, BUF_WIDTH, BUF_HEIGHT);
    initImage_ushort(&inputHue, WIDTH, HEIGHT);
    initImage_uchar(&inputSaturation, WIDTH, HEIGHT);
    initImage_uchar(&inputIntensy, WIDTH, HEIGHT);

    while(1)
    {
#ifdef	TIME_MEASURE
		//시간 측정
		gettimeofday(&stime, NULL);
#endif
        read_image_from_FPGA(inputRGB.img);
        
		//LCD에 원래 이미지 출력
        //draw_img_from_buffer_pure1(inputRGB.img);
	   
		//HSI 변환
		for(r = 0; r < HEIGHT; r++)
		{
			index = r * WIDTH;
			index2 = r * BUF_WIDTH;
			
			for(c = 0; c < WIDTH; c++)
			{
				rgb = inputRGB.img[index2 + c];
				
				inputHue.img[index + c] = h_lookup[ rgb ];
				inputSaturation.img[index + c] = s_lookup[ rgb ];
				inputIntensy.img[index + c] = i_lookup[ rgb ];
			}
		}

		iR = (((inputRGB.img[60*BUF_WIDTH + 90]&0xF800) >> 11) << 3);
		iG = (((inputRGB.img[60*BUF_WIDTH + 90]&0x07E0) >> 5) << 2);
		iB = ((inputRGB.img[60*BUF_WIDTH + 90]&0x001F) << 3);

				//중심 픽셀값 출력
		printf("H = %3d, S = %3d, I = %3d, R = %3d, G = %3d, B = %3d\n",
				inputHue.img[60*WIDTH + 90],
				inputSaturation.img[60*WIDTH + 90],
				inputIntensy.img[60*WIDTH + 90],
				iR, iG, iB);
	
		drawCross(inputRGB.img, 90, 60, 10, 3, BUF_WIDTH, BUF_HEIGHT);

		//LCD에 영상처리된 이미지 출력
		//draw_img_from_buffer_pure2(inputRGB.img);
		draw_img_from_buffer_full(inputRGB.img);
		graphic_flip();

		//시간 측정 완료
#ifdef	TIME_MEASURE
		gettimeofday(&etime, NULL);
		
		sec = etime.tv_sec - stime.tv_sec;
		usec = etime.tv_usec - stime.tv_sec;
		if(usec < 0)
		{
			sec--;
			usec += 1000000;
		}

		printf("1Frame time = %ldsec %ldusec\n", sec, usec);
#endif
	}//while
	
	freeImage_ushort(&inputRGB);
	freeImage_ushort(&inputHue);
	freeImage_uchar(&inputSaturation);
	freeImage_uchar(&inputIntensy);
}
