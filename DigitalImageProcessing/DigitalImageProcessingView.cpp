
// DigitalImageProcessingView.cpp: CDigitalImageProcessingView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DigitalImageProcessing.h"
#endif

#include "DigitalImageProcessingDoc.h"
#include "DigitalImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitalImageProcessingView

IMPLEMENT_DYNCREATE(CDigitalImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CDigitalImageProcessingView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_IMG_LOAD_BMP, &CDigitalImageProcessingView::OnImgLoadBmp)
	ON_COMMAND(ID_IMG_LOAD_JPG, &CDigitalImageProcessingView::OnImgLoadJpg)
	ON_COMMAND(ID_RGB_TO_HSI, &CDigitalImageProcessingView::OnRgbToHsi)
END_MESSAGE_MAP()

// CDigitalImageProcessingView 생성/소멸

CDigitalImageProcessingView::CDigitalImageProcessingView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	rgbBuffer = nullptr;

}

CDigitalImageProcessingView::~CDigitalImageProcessingView()
{
	if (rgbBuffer != nullptr) {
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}
}

BOOL CDigitalImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CDigitalImageProcessingView 그리기

void CDigitalImageProcessingView::OnDraw(CDC* pDC)
{
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (rgbBuffer != NULL) { //이미지가 들어있는 경우에 출력
		for (int i = 0; i < imgHeight; i++) { //이미지 사이즈 만큼 돌면서 한 픽셀씩 출력
			for (int j = 0; j < imgWidth; j++) {
				POINT p; //출력할 픽셀의 위치 지정
				p.x = j;
				p.y = i;
				pDC->SetPixel(p, RGB(rgbBuffer[i][j].rgbRed, rgbBuffer[i][j].rgbGreen, rgbBuffer[i][j].rgbBlue));
				//픽셀 위치 p에 RGB 값을 출력

				if (viewType == 2) {
					p.x = j + imgWidth + 10;
					p.y = i;
					pDC->SetPixel(p, RGB(hueBuffer[i][j], hueBuffer[i][j], hueBuffer[i][j]));
					
					p.x = j + imgWidth + 10;
					p.y = i + imgHeight + 10;
					pDC->SetPixel(p, RGB(satuBuffer[i][j], satuBuffer[i][j], satuBuffer[i][j]));

					p.x = j;
					p.y = i + imgHeight + 10;
					pDC->SetPixel(p, RGB(intenBuffer[i][j], intenBuffer[i][j], intenBuffer[i][j]));
				}
			}
		}
	}
}


// CDigitalImageProcessingView 인쇄

BOOL CDigitalImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CDigitalImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CDigitalImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CDigitalImageProcessingView 진단

#ifdef _DEBUG
void CDigitalImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CDigitalImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingDoc)));
	return (CDigitalImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigitalImageProcessingView 메시지 처리기


void CDigitalImageProcessingView::OnImgLoadBmp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnBmpLoad();
}


void CDigitalImageProcessingView::OnBmpLoad()
{
	// TODO: 여기에 구현 코드 추가.
	//1. 파일 다이얼로그로부터 BMP 파일 입력
	CFileDialog dlg(TRUE, ".bmp", NULL, NULL, "Bitmap File (*.bmp)|*.bmp||");
	if (IDOK != dlg.DoModal())
		return;
	CString filename = dlg.GetPathName();
	if (rgbBuffer != NULL) { //이미 할당된 경우, 메모리 해제
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}

	//2. 파일을 오픈하여 영상 정보 획득
	CFile file;
	file.Open(filename, CFile::modeRead);
	file.Read(&bmpHeader, sizeof(BITMAPFILEHEADER));
	file.Read(&bmpInfo, sizeof(BITMAPINFOHEADER));
	imgWidth = bmpInfo.biWidth;
	imgHeight = bmpInfo.biHeight;

	//3. 이미지를 저장할 버퍼 할당 (2차원 배열) [이미지 높이 * 이미지 너비 만큼 할당]
	rgbBuffer = new RGBQUAD*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		rgbBuffer[i] = new RGBQUAD[imgWidth];
	}

	//4. 이미지의 너비가 4의 배수인지 체크
	//BMP조건 가로는 4byte씩 이어야 한다.
	//한 픽셀이 3바이트(R, G, B)씩이니까, 가로(m_width) * 3이 4의 배수인가 아닌가를 알아야한다.
	//b4byte : 4byte배수인지 아닌지를 안다.
	//upbyte : 4byte배수에 모자라는 바이트다.
	bool b4byte = false;
	int upbyte = 0;
	if ((imgWidth * 3) % 4 == 0) {
		//4의 배수로 떨어지는 경우
		b4byte = true;
		upbyte = 0;
	}
	else {
		//4의 배수로 떨어지지않는 경우
		b4byte = false;
		upbyte = 4 - (imgWidth * 3) % 4;
	}

	//5. 픽셀 데이터를 파일로부터 읽어옴
	BYTE data[3];
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			file.Read(&data, 3);
			//이미지가 거꾸로 저장되어 있기 때문에 거꾸로 읽어옴
			rgbBuffer[imgHeight - i - 1][j].rgbBlue = data[0];
			rgbBuffer[imgHeight - i - 1][j].rgbGreen = data[1];
			rgbBuffer[imgHeight - i - 1][j].rgbRed = data[2];
		}
		if (b4byte == false) {
			//가로가 4byte 배수가 아니면 쓰레기 값을 읽는다
			file.Read(&data, upbyte);
		}
	}
	file.Close(); //파일 닫기
	Invalidate(TRUE); //화면 갱신
}


void CDigitalImageProcessingView::OnImgLoadJpg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnJpgLoad();
}

void CDigitalImageProcessingView::OnJpgLoad() {
	CFileDialog dlg(TRUE, ".jpg", NULL, NULL, "Jpeg File (*.jpg)|*.jpg||");
	if (IDOK != dlg.DoModal()) return;
	CString filename = dlg.GetPathName();
	//BMP 로드때와 마찬가지로, rgbBuffer가 NULL이 아닌 경우 메모리 해제 코드 추가하기
	if (rgbBuffer != NULL) { //이미 할당된 경우, 메모리 해제
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}
	FILE *fp = fopen(filename, "rb");
	BITMAPINFOHEADER pbh;
	UINT w, h;
	BYTE *pbuf = LoadJpegFromOpenFile(fp, &pbh, &w, &h);
	imgWidth = (int)w;
	imgHeight = (int)h;
	//BMP로드때와 마찬가지로, rgbBuffer를 이미지 사이즈 만큼 메모리 할당 코드 추가하기
	rgbBuffer = new RGBQUAD*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		rgbBuffer[i] = new RGBQUAD[imgWidth];
	}
	int dw = WIDTHBYTES(imgWidth * 24);
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			rgbBuffer[imgHeight - i - 1][j].rgbBlue = pbuf[i*dw + j * 3 + 0];
			rgbBuffer[imgHeight - i - 1][j].rgbGreen = pbuf[i*dw + j * 3 + 1];
			rgbBuffer[imgHeight - i - 1][j].rgbRed = pbuf[i*dw + j * 3 + 2];
		}
	}
	delete[] pbuf;
	fclose(fp);
	Invalidate(TRUE);
}

BYTE* CDigitalImageProcessingView::LoadJpegFromOpenFile(FILE *fp, BITMAPINFOHEADER *pbh, UINT *pWidth, UINT *pHeight) {
	//파일로부터 JPG 정보 획득
	if (pWidth == NULL || pHeight == NULL) return NULL;
	BYTE* tmp = JpegFile::OpenFileToRGB(fp, pWidth, pHeight);
	if (!tmp) return NULL;
	JpegFile::BGRFromRGB(tmp, *pWidth, *pHeight);
	UINT dw;
	BYTE* pbuf = JpegFile::MakeDwordAlignedBuf(tmp, *pWidth, *pHeight, &dw);
	delete[] tmp;
	if (!pbuf) return NULL;
	JpegFile::VertFlipBuf(pbuf, dw, *pHeight);
	pbh->biSize = sizeof(BITMAPINFOHEADER);
	pbh->biWidth = *pWidth;
	pbh->biHeight = *pHeight;
	pbh->biPlanes = 1;
	pbh->biBitCount = 24;
	pbh->biCompression = BI_RGB;
	pbh->biSizeImage = 0;
	pbh->biXPelsPerMeter = 0;
	pbh->biYPelsPerMeter = 0;
	pbh->biClrUsed = 0;
	pbh->biClrImportant = 0;
	return pbuf;
}


void CDigitalImageProcessingView::OnRgbToHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 1. rgbBuffer에 이미지가 들어있는 지 여부 확인
	if (rgbBuffer == NULL)
		OnBmpLoad(); //rgbBuffer에 데이터가 없는 경우, 로드 함수를 호출하여 이미지 획득
	// 2. 변수 메모리 할당
	hueBuffer = new float*[imgHeight];
	satuBuffer = new float*[imgHeight];
	intenBuffer = new float*[imgHeight];

	for (int i = 0; i < imgHeight; i++) {
		hueBuffer[i] = new float[imgWidth];
		satuBuffer[i] = new float[imgWidth];
		intenBuffer[i] = new float[imgWidth];
	}
	// 3. RGB to HSI 값 변환
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			float r = rgbBuffer[i][j].rgbRed;
			float g = rgbBuffer[i][j].rgbGreen;
			float b = rgbBuffer[i][j].rgbBlue;
			intenBuffer[i][j] = (r + g + b) / (float)(3 * 255); //intensity
			float total = r + g + b;
			r = r / total; g = g / total; b = b / total;
			satuBuffer[i][j] = 1 - 3 * (r > g ? (g > b ? b : g) : (r > b ? b : r));
			if (r == g && g == b) {
				hueBuffer[i][j] = 0; satuBuffer[i][j] = 0;
			}
			else {
				total = (0.5*(r - g + r - b) / sqrt((r - g)*(r - g) + (r - b)*(g - b)));
				hueBuffer[i][j] = acos((double)total);
				if (b > g) {
					hueBuffer[i][j] = 6.28 - hueBuffer[i][j];
				}
			}
		}
	}
	// 4. 출력 값 범위 정규화 : 출력 시, 값의 범위를 [0, 255]로 맞춰줌
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			hueBuffer[i][j] = hueBuffer[i][j] * 255 / (3.14 * 2);
			satuBuffer[i][j] = satuBuffer[i][j] * 255;
			intenBuffer[i][j] = intenBuffer[i][j] * 255;
		}
	}
	// 5. 출력
	viewType = 2;
	Invalidate(FALSE);
}
