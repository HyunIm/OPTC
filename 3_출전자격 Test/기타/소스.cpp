
// FirstDlg.cpp : 구현 파일
//
/* read me first!!
수정위치는 함수 capCallbackOnFrame 입니다.(위치못찾겠으면 검색)
수정위치 윗부분의 함수는 MFC부분(윈도우화면출력) 이므로 신경 안써도 됩니다.
코드가 길다고 겁먹지 말고 수정위치 부분만 보도록 합시다^^
1~7은 쉬운거니까 반드시해야하구여(차례대로하면댐) 8은 할수있다면 해봅시다
*/

#include "stdafx.h"
#include "First.h"
#include "FirstDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFirstDlg * mainDlg;

BITMAPINFO BmInfo;
unsigned char * input_Image;
unsigned char * output_Image;
unsigned short *hImage;
unsigned char *movingImage;
unsigned char *sImage;
unsigned char *iImage;
unsigned char *binary_Image;
unsigned char *check_color;
int *histo;
int beforeX=0, beforeY=0;
int nextX = 0, nextY = 0;
int frameNum =0;

node* queue_front;
node* queue_rear;
frontAndRear_t* set;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

#ifdef _DEBUG
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


//  대화 상자
CFirstDlg::CFirstDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFirstDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CFirstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_str);
}

BEGIN_MESSAGE_MAP(CFirstDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

// CFirstDlg 메시지 처리기

void CFirstDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFirstDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFirstDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CFirstDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DestroyWindow();
}

void enqueue(frontAndRear_t* set, BYTE *data)
{
        int i;
        node_t* queue_front = set->front;
        node_t* queue_rear = set->rear;

        node_t* queue_node = (node*)malloc(sizeof(node));

        queue_node->prev = queue_rear->prev;
        queue_node->next = queue_rear;

        queue_rear->prev->next = queue_node;
        queue_rear->prev = queue_node;

        for(i=0 ; i< WIDTH*HEIGHT ; i++){
                queue_node->data[i] = data[i];
        }
        (queue_front->data[0])++;
}

void dequeue(frontAndRear_t* set)
{
        int i;
        node_t* queue_front = set->front;
        node_t* queue_rear = set->rear;

        node_t* deQueueNode;


        if ( queue_front->next != queue_rear ) 
        {
                deQueueNode = queue_front->next;

                queue_front->next = deQueueNode->next;
                deQueueNode->next->prev = queue_front;
/*
                for(i=0 ; i< WIDTH*HEIGHT ; i++){
                        data[i] = deQueueNode->data[i];
                }
*/
                free(deQueueNode);
                (queue_front->data[0])--;
        }
        else
        {
                printf("queue is empty\n");
        }

}

// ------------여기서부터 수정 영역 ----------------------------------------------------

CFirstDlg::~CFirstDlg(){ //종료시 메모리 해제 부분
	delete [] input_Image;
	delete [] output_Image;
	delete [] hImage;
	delete [] sImage;
	delete [] iImage;
	delete [] movingImage;
	delete [] binary_Image;
	delete [] check_color;
	delete [] histo;
}

void memallocate(void){ //동적 메모리 할당 부분
	//색, 채도, 명도 3가지 정보라서 3
	input_Image = new unsigned char[WIDTH * HEIGHT * 3];
	output_Image = new unsigned char[WIDTH * HEIGHT * 3];
	//hsi , 색, 채도, 명도
	hImage = new unsigned short[WIDTH * HEIGHT]; 
	sImage = new unsigned char[WIDTH * HEIGHT];
	iImage  = new unsigned char[WIDTH * HEIGHT];
	movingImage = new unsigned char[WIDTH * HEIGHT]; 
	//명도 값만 가지면 바이너리 이미지
	binary_Image = new unsigned char[WIDTH * HEIGHT];
	check_color = new unsigned char[WIDTH * HEIGHT];
	histo = new int[5];
	frameNum = 1;

	//동적배경 업데이트를위한 큐 만들기
    queue_front = (node*)malloc(sizeof(node));
    queue_rear = (node*)malloc(sizeof(node));
    queue_front->data[0] = 0;

    queue_front->next = queue_rear;
    queue_front->prev = NULL;
    queue_rear->next = NULL;
    queue_rear->prev = queue_front;

    set = (frontAndRear_t*)malloc(sizeof(frontAndRear_t));

    set->front = queue_front;
    set->rear = queue_rear;
    //큐 초기화 끝
}

BOOL CFirstDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	mainDlg = this;
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}



	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	SetWindowText(_T("Tae Kwon"));
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	memallocate();

	Capture = capCreateCaptureWindow(TEXT("Capture Window"),WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, this->m_hWnd,3333);

	//매 frame이 캡처될 때마다 호출될 callback function 지정
	if(capSetCallbackOnFrame(Capture, capCallbackOnFrame) == FALSE) return FALSE;
	//+306
	//Camera Driver와 Capture Window 연결
	if(capDriverConnect(Capture, 0) == FALSE) return FALSE;

	////////////////////////////////////////////////////////////////////////////
	//현재 Video Format을 조사해서 24bits color가 아니면 24bits color로 바꿈
	capGetVideoFormat(Capture, &BmInfo, sizeof(BITMAPINFO));

	if(BmInfo.bmiHeader.biBitCount != 24)
	{
		BmInfo.bmiHeader.biBitCount = 24;
		BmInfo.bmiHeader.biCompression = 0;
		BmInfo.bmiHeader.biSizeImage = BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight * 3;
		capSetVideoFormat(Capture, &BmInfo, sizeof(BITMAPINFO));
	}
	////
	capPreviewRate(Capture, 40);
	capOverlay(Capture, true);
	capPreview(Capture, true);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

LRESULT CALLBACK capCallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) { //인풋에 넣기
		int temp1 = i * WIDTH;

		for (j = 0; j < WIDTH; j++)
		{
			int temp2 = temp1 + j;
			input_Image[temp2 * 3 + R] = *(lpVHdr->lpData + temp2 * 3 + R);
			input_Image[temp2 * 3 + G] = *(lpVHdr->lpData + temp2 * 3 + G);
			input_Image[temp2 * 3 + B] = *(lpVHdr->lpData + temp2 * 3 + B);
		}
	}

	rgb2hsi(input_Image, hImage, sImage, iImage, HEIGHT, WIDTH);
	
	
	printf("try\n");
	compareBGwithQ(set, iImage, movingImage);

	for (i = 0; i < 5; i++) histo[i] = 0;
	int manyColor = 1;
	
	// h,s 값 기준으로 이진화
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			//r
			if ((hImage[i*WIDTH + j] < 5 || hImage[i*WIDTH + j]>350)
				&& sImage[i*WIDTH + j] > 70) {
				binary_Image[i*WIDTH + j] = 255;
				check_color[i*WIDTH + j] = 1;
				//histo[1]++;
			}
			//y
			else if ((hImage[i*WIDTH + j] < 63 && hImage[i*WIDTH + j]>35)
				&& sImage[i*WIDTH + j] > 70) {
				binary_Image[i*WIDTH + j] = 255;
				check_color[i*WIDTH + j] = 2;
				//histo[2]++;
			}
			//g
			else if ((hImage[i*WIDTH + j] < 143 && hImage[i*WIDTH + j]>63)
				&& sImage[i*WIDTH + j] > 20) {
				binary_Image[i*WIDTH + j] = 255;
				check_color[i*WIDTH + j] = 3;
				//histo[3]++;
			}

			//b
			else if ((hImage[i*WIDTH + j] < 210 && hImage[i*WIDTH + j]>190)
				&& sImage[i*WIDTH + j] > 117 && sImage[i*WIDTH + j] < 198) {
				binary_Image[i*WIDTH + j] = 255;
				check_color[i*WIDTH + j] = 4;
				//histo[4]++;
			}

			else {
				binary_Image[i*WIDTH + j] = 0;
				check_color[i*WIDTH + j] = 0;
			}
		}
	}


	//7.밑의 아웃풋을 수정하여 출력해보세요.

	/*
	for (i = 0; i < HEIGHT; i++){ //아웃풋에 넣기
		int temp1 = i * WIDTH;

		for (j = 0; j < WIDTH; j++)
		{
			int temp2 = temp1 + j;
			output_Image[temp2 * 3 + R] = input_Image[temp2 * 3 + R];
			output_Image[temp2 * 3 + G] = input_Image[temp2 * 3 + G];
			output_Image[temp2 * 3 + B] = input_Image[temp2 * 3 + B];
		}
	}
	*/
	// 라벨링(Grassfire) 후 가장 큰 영역 구분
	m_BlobColoring(binary_Image, HEIGHT, WIDTH);

	int gX = 0, gY = 0, cnt = 0;

	//라벨링 후 색 검사
	for (i = 0; i < HEIGHT; i++) { //아웃풋에 넣기
		int temp1 = i * WIDTH;

		for (j = 0; j < WIDTH; j++)
		{
			if (binary_Image[temp1 + j] == 255) {
				histo[check_color[temp1 + j]]++;
			}
		}
	}
	
	//printf("%d, %d, %d, %d", histo[1], histo[2], histo[3], histo[4]);
	//가장 많이 검출된 색 검출
	for (i = 1; i < 5; i++)
		if (histo[manyColor] < histo[i]) {
			manyColor = i;
		}
	if (manyColor == 1)
		printf("빨간색\n");
	else if(manyColor==2)
		printf("노란색\n");
	else if(manyColor==3)
		printf("초록색\n");
	else if(manyColor==4)
		printf("파란색\n");
	//printf("%d", manyColor);
	//검출된 부분의 중심 찾고 적은 색상 제거
	for (i = 0; i < HEIGHT; i++) { //아웃풋에 넣기
		int temp1 = i * WIDTH;

		for (j = 0; j < WIDTH; j++)
		{
			if (binary_Image[temp1 + j] == 255 && check_color[temp1+j] == manyColor) {
				gX += j;
				gY += i;
				cnt++;
			}
			else binary_Image[temp1 + j] = 0;
		}
	}

	if (cnt != 0) {
		gX /= cnt;
		gY /= cnt;
		for (i = -15; i < 15; i++) {
			binary_Image[(gX + i) + gY*WIDTH] = 0;
		}

		for (i = -15; i < 15; i++) {
			binary_Image[gX + (gY + i)*WIDTH] = 0;
		}
		// 예상경로 추적
		nextX=gX+(gX-beforeX);
		nextY=gY+(gY-beforeY);
		if (nextX > WIDTH-16) nextX = WIDTH - 15;
		else if(nextX < 16) nextX = 16;

		if (nextY > HEIGHT-16) nextY = HEIGHT - 15;
		else if (nextY < 16) nextY = 16;
		for (i = -15; i < 15; i++) {
			binary_Image[nextX+i+nextY*WIDTH] = 254;
		}

		for (i = -15; i < 15; i++) {
			binary_Image[nextX +(nextY+i)*WIDTH] = 254;
		}
		

		beforeX = gX;
		beforeY = gY;
	}
	
	for (i = 0; i < HEIGHT; i++) { //아웃풋에 넣기
		int temp1 = i * WIDTH;

		for (j = 0; j < WIDTH; j++)
		{
			int temp2 = temp1 + j;
			
			if (binary_Image[temp2] == 255 ) {
				output_Image[temp2 * 3 + R] = input_Image[temp2 * 3 + R];
				output_Image[temp2 * 3 + G] = input_Image[temp2 * 3 + G];
				output_Image[temp2 * 3 + B] = input_Image[temp2 * 3 + B];
				//output_Image[temp2 * 3 + R] = 255;
				//output_Image[temp2 * 3 + G] = 255;
				//output_Image[temp2 * 3 + B] = 255;

			}
			//예상경로 중심
			else if (binary_Image[temp2] == 254) {
				output_Image[temp2 * 3 + R] = 255;
				output_Image[temp2 * 3 + G] = 255;
				output_Image[temp2 * 3 + B] = 255;
			}
			else {
				output_Image[temp2 * 3 + R] = 0;
				output_Image[temp2 * 3 + G] = 0;
				output_Image[temp2 * 3 + B] = 0;
			}

		}
	}
	
	

	for (i = 0; i < HEIGHT; i++){ //아웃풋에 넣기
		int temp1 = i * WIDTH;
		
		for (j = 0; j < WIDTH; j++)
		{
			int temp2 = temp1 + j;
			*(lpVHdr->lpData + temp2 * 3 + R) = output_Image[temp2 * 3 + R];
			*(lpVHdr->lpData + temp2 * 3 + G) = output_Image[temp2 * 3 + G];
			*(lpVHdr->lpData + temp2 * 3 + B) = output_Image[temp2 * 3 + B];
		}
	}
	
	CString str; //글 출력하는 부분
	//5. 현재 프레임의 번호를 frameNum를 선언 해서 1부터 번호매겨서 출력해보세요 (밑의 TEXT부분)
	// 5-2 . 위의 4번에서 받은 HSI값 개를 번호 다음에 출력하게끔만드세여
	//str.Format(TEXT("frameNum : %d ,%d, %d, %d \r\n"),frameNum , iImage[(HEIGHT/2) * WIDTH + (WIDTH/2)], 
	//	hImage[(HEIGHT / 2) * WIDTH + (WIDTH / 2)], sImage[(HEIGHT / 2) * WIDTH + (WIDTH / 2)]);
	//if(cnt!=0)
	str.Format(TEXT("frameNum : %d ,%d, %d \r\n"), hImage[gY*WIDTH+gX], sImage[gY*WIDTH + gX], check_color[gY*WIDTH + gX]);
	
	

	//str.Format(TEXT("frameNum : %d ,%d, %d, %d \r\n"), hImage[WIDTH/2+HEIGHT/2*WIDTH], sImage[WIDTH / 2 + HEIGHT / 2 * WIDTH], iImage[WIDTH / 2 + HEIGHT / 2 * WIDTH], manyColor);
		
	frameNum ++;
	mainDlg->m_str.ReplaceSel(str);	
	return (LRESULT)TRUE;
}



void rgb2hsi(unsigned char *originalImage, unsigned short *hImage, unsigned char *sImage, unsigned char *iImage, int height, int width)
{
	int i, j;
        double minc, angle;

        int red, green, blue;
        float fR, fG, fB;

        for (i = 0; i < HEIGHT; i++) {
                for (j = 0; j < WIDTH; j++) {
                        red = originalImage[(i * WIDTH + j) * 3 + 2];
                        green = originalImage[(i * WIDTH + j) * 3 + 1];
                        blue = originalImage[(i * WIDTH + j) * 3 + 0];


                        //정규화된 RGB로 변환
                        if (red == 0 && green == 0 && blue == 0)
                        {
                                fR = 0.0f;
                                fG = 0.0f;
                                fB = 0.0f;
                        }
                        else
                        {
                                fR = (float)red / (red + green + blue);
                                fG = (float)green / (red + green + blue);
                                fB = (float)blue / (red + green + blue);
                        }

                        minc = MIN(fR, fG);
                        minc = MIN(minc, fB);

                        iImage[i * WIDTH + j] = (unsigned char)((red + green + blue) / 3);

                        if ((fR == fG) && (fG == fB)){          // Gray Scale
                                hImage[i * WIDTH + j] = 0;
                                sImage[i * WIDTH + j] = 0;
                                continue;
                        }
                        else {
                                //sImg[i * width + j] = (BYTE)(255.0 - 3.0 * minc *255 / (red + green + blue));
                                sImage[i * WIDTH + j] = (BYTE)(1.0f - 3.0f*minc *  255.0f);//fR+fG+fB = 1.0이니까

                                //angle = (((red - green) + (red - blue)) >> 1) / (double)(sqrt((double)((red - green) * (red - green) + (red - blue) * (green - blue))));
                                angle = (fR - 0.5f*fG - 0.5f*fB) / (float)sqrt((fR - fG)*(fR - fG) + (fR - fB)*(fG - fB));
                                hImage[i * WIDTH + j] = (WORD)(acos(angle) * 57.29577951);
                        }

                        if (fB > fG) hImage[i * WIDTH + j] = (WORD)(360 - hImage[i * WIDTH + j]);

                        //sImg[i * width + j] *= 255.0f;
                }
        }
        return;

}

void compareBGwithQ(frontAndRear_t* set, unsigned char* input,  unsigned char* output)
{
		printf("try enqueue\n");
        //int sum[WIDTH* HEIGHT]; //여기가 스택오버플로뜸 

        int length = (int)(set->front->data[0]);
		

        node_t* thisNode = set->front->next;
		
		printf("try enqueue2\n");
/*
        for(int i=0; i<length; i++)
        {
                for(int j=0 ; j<WIDTH*HEIGHT ; j++){ //현재큐의 데이터받아옴
                     sum[j] += thisNode->data[j];
					//printf("%d\n",  thisNode->data[j]);
                }
                thisNode = thisNode->next; //큐 순번 옮김
        }
		
        if(length > 0){
                for(int j=0 ; j<WIDTH*HEIGHT ; j++){
                        sum[j] = sum[j]/length; //평균 구함
                        if( (input[j] -sum[j]) > 50 || (sum[j] -input[j])> 50)
                        {
                                output[j] = 0; //차이값크면 0
                        }
                        else
                        {
                                output[j] = 255;
                        }
                }
        }
		*/
        //enqueue(set, input); // 새로들어온거 넣기
        //if(length > 10){
        //        dequeue(set);
        //}

}



/*
void CFirstDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
*/

int push(short *stackx, short *stacky, int arr_size, short vx, short vy, int *top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short *stackx, short *stacky, short *vx, short *vy, int *top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}


// GlassFire 알고리즘을 이용한 라벨링 함수
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// 스택으로 사용할 메모리 할당
	short* stackx = new short[height*width];
	short* stacky = new short[height*width];
	short* coloring = new short[height*width];

	int arr_size = height * width;

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당

	for (k = 0; k<height*width; k++) coloring[k] = 0;  // 메모리 초기화

	for (i = 0; i<height; i++)
	{
		index = i*width;
		for (j = 0; j<width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m*width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m<0 || m >= height || n<0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // 현재 라벨로 마크
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor<1000) BlobArea[curColor] = area;
		}
	}

	//	float grayGap = 250.0f / (float)curColor;

	// 가장 면적이 넓은 영역을 찾아내기 위함 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage 배열 클리어~
	for (k = 0; k < width*height; k++) CutImage[k] = 0;

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width*height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 255;  // 가장 큰 것만 저장
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
