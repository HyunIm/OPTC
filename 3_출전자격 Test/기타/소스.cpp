
// FirstDlg.cpp : ���� ����
//
/* read me first!!
������ġ�� �Լ� capCallbackOnFrame �Դϴ�.(��ġ��ã������ �˻�)
������ġ ���κ��� �Լ��� MFC�κ�(������ȭ�����) �̹Ƿ� �Ű� �Ƚᵵ �˴ϴ�.
�ڵ尡 ��ٰ� �̸��� ���� ������ġ �κи� ������ �սô�^^
1~7�� ����Ŵϱ� �ݵ���ؾ��ϱ���(���ʴ���ϸ��) 8�� �Ҽ��ִٸ� �غ��ô�
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
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

#ifdef _DEBUG
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


//  ��ȭ ����
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

// CFirstDlg �޽��� ó����

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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CFirstDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CFirstDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CFirstDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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

// ------------���⼭���� ���� ���� ----------------------------------------------------

CFirstDlg::~CFirstDlg(){ //����� �޸� ���� �κ�
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

void memallocate(void){ //���� �޸� �Ҵ� �κ�
	//��, ä��, �� 3���� ������ 3
	input_Image = new unsigned char[WIDTH * HEIGHT * 3];
	output_Image = new unsigned char[WIDTH * HEIGHT * 3];
	//hsi , ��, ä��, ��
	hImage = new unsigned short[WIDTH * HEIGHT]; 
	sImage = new unsigned char[WIDTH * HEIGHT];
	iImage  = new unsigned char[WIDTH * HEIGHT];
	movingImage = new unsigned char[WIDTH * HEIGHT]; 
	//�� ���� ������ ���̳ʸ� �̹���
	binary_Image = new unsigned char[WIDTH * HEIGHT];
	check_color = new unsigned char[WIDTH * HEIGHT];
	histo = new int[5];
	frameNum = 1;

	//������� ������Ʈ������ ť �����
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
    //ť �ʱ�ȭ ��
}

BOOL CFirstDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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



	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	SetWindowText(_T("Tae Kwon"));
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	memallocate();

	Capture = capCreateCaptureWindow(TEXT("Capture Window"),WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, this->m_hWnd,3333);

	//�� frame�� ĸó�� ������ ȣ��� callback function ����
	if(capSetCallbackOnFrame(Capture, capCallbackOnFrame) == FALSE) return FALSE;
	//+306
	//Camera Driver�� Capture Window ����
	if(capDriverConnect(Capture, 0) == FALSE) return FALSE;

	////////////////////////////////////////////////////////////////////////////
	//���� Video Format�� �����ؼ� 24bits color�� �ƴϸ� 24bits color�� �ٲ�
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

LRESULT CALLBACK capCallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) { //��ǲ�� �ֱ�
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
	
	// h,s �� �������� ����ȭ
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


	//7.���� �ƿ�ǲ�� �����Ͽ� ����غ�����.

	/*
	for (i = 0; i < HEIGHT; i++){ //�ƿ�ǲ�� �ֱ�
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
	// �󺧸�(Grassfire) �� ���� ū ���� ����
	m_BlobColoring(binary_Image, HEIGHT, WIDTH);

	int gX = 0, gY = 0, cnt = 0;

	//�󺧸� �� �� �˻�
	for (i = 0; i < HEIGHT; i++) { //�ƿ�ǲ�� �ֱ�
		int temp1 = i * WIDTH;

		for (j = 0; j < WIDTH; j++)
		{
			if (binary_Image[temp1 + j] == 255) {
				histo[check_color[temp1 + j]]++;
			}
		}
	}
	
	//printf("%d, %d, %d, %d", histo[1], histo[2], histo[3], histo[4]);
	//���� ���� ����� �� ����
	for (i = 1; i < 5; i++)
		if (histo[manyColor] < histo[i]) {
			manyColor = i;
		}
	if (manyColor == 1)
		printf("������\n");
	else if(manyColor==2)
		printf("�����\n");
	else if(manyColor==3)
		printf("�ʷϻ�\n");
	else if(manyColor==4)
		printf("�Ķ���\n");
	//printf("%d", manyColor);
	//����� �κ��� �߽� ã�� ���� ���� ����
	for (i = 0; i < HEIGHT; i++) { //�ƿ�ǲ�� �ֱ�
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
		// ������ ����
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
	
	for (i = 0; i < HEIGHT; i++) { //�ƿ�ǲ�� �ֱ�
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
			//������ �߽�
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
	
	

	for (i = 0; i < HEIGHT; i++){ //�ƿ�ǲ�� �ֱ�
		int temp1 = i * WIDTH;
		
		for (j = 0; j < WIDTH; j++)
		{
			int temp2 = temp1 + j;
			*(lpVHdr->lpData + temp2 * 3 + R) = output_Image[temp2 * 3 + R];
			*(lpVHdr->lpData + temp2 * 3 + G) = output_Image[temp2 * 3 + G];
			*(lpVHdr->lpData + temp2 * 3 + B) = output_Image[temp2 * 3 + B];
		}
	}
	
	CString str; //�� ����ϴ� �κ�
	//5. ���� �������� ��ȣ�� frameNum�� ���� �ؼ� 1���� ��ȣ�Űܼ� ����غ����� (���� TEXT�κ�)
	// 5-2 . ���� 4������ ���� HSI�� ���� ��ȣ ������ ����ϰԲ����弼��
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


                        //����ȭ�� RGB�� ��ȯ
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
                                sImage[i * WIDTH + j] = (BYTE)(1.0f - 3.0f*minc *  255.0f);//fR+fG+fB = 1.0�̴ϱ�

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
        //int sum[WIDTH* HEIGHT]; //���Ⱑ ���ÿ����÷ζ� 

        int length = (int)(set->front->data[0]);
		

        node_t* thisNode = set->front->next;
		
		printf("try enqueue2\n");
/*
        for(int i=0; i<length; i++)
        {
                for(int j=0 ; j<WIDTH*HEIGHT ; j++){ //����ť�� �����͹޾ƿ�
                     sum[j] += thisNode->data[j];
					//printf("%d\n",  thisNode->data[j]);
                }
                thisNode = thisNode->next; //ť ���� �ű�
        }
		
        if(length > 0){
                for(int j=0 ; j<WIDTH*HEIGHT ; j++){
                        sum[j] = sum[j]/length; //��� ����
                        if( (input[j] -sum[j]) > 50 || (sum[j] -input[j])> 50)
                        {
                                output[j] = 0; //���̰�ũ�� 0
                        }
                        else
                        {
                                output[j] = 255;
                        }
                }
        }
		*/
        //enqueue(set, input); // ���ε��°� �ֱ�
        //if(length > 10){
        //        dequeue(set);
        //}

}



/*
void CFirstDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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


// GlassFire �˰����� �̿��� �󺧸� �Լ�
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// �������� ����� �޸� �Ҵ�
	short* stackx = new short[height*width];
	short* stacky = new short[height*width];
	short* coloring = new short[height*width];

	int arr_size = height * width;

	// �󺧸��� �ȼ��� �����ϱ� ���� �޸� �Ҵ�

	for (k = 0; k<height*width; k++) coloring[k] = 0;  // �޸� �ʱ�ȭ

	for (i = 0; i<height; i++)
	{
		index = i*width;
		for (j = 0; j<width; j++)
		{
			// �̹� �湮�� ���̰ų� �ȼ����� 255�� �ƴ϶�� ó�� ����
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
						//���� �ȼ��� �����踦 ����� ó�� ����
						if (m<0 || m >= height || n<0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // ���� �󺧷� ��ũ
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

	// ���� ������ ���� ������ ã�Ƴ��� ���� 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage �迭 Ŭ����~
	for (k = 0; k < width*height; k++) CutImage[k] = 0;

	// coloring�� ����� �󺧸� ����� (Out_Area�� �����) ������ ���� ū �͸� CutImage�� ����
	for (k = 0; k < width*height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 255;  // ���� ū �͸� ����
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
