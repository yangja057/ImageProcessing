
// DigitalImageProcessingView.h: CDigitalImageProcessingView 클래스의 인터페이스
//

#pragma once
#include "Jpegfile.h"


class CDigitalImageProcessingView : public CView
{
protected: // serialization에서만 만들어집니다.
	CDigitalImageProcessingView();
	DECLARE_DYNCREATE(CDigitalImageProcessingView)

// 특성입니다.
public:
	CDigitalImageProcessingDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CDigitalImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImgLoadBmp();
	RGBQUAD** rgbBuffer; // 이미지를 저장할 변수
	int imgHeight; // 이미지 높이 정보
	int imgWidth; // 이미지 너비 정보
	BITMAPINFOHEADER bmpInfo; // BMP 정보 헤더
	BITMAPFILEHEADER bmpHeader; // BMP 파일 헤더
	void OnBmpLoad();
	afx_msg void OnImgLoadJpg();
	BYTE* LoadJpegFromOpenFile(FILE *fp, BITMAPINFOHEADER *pbh, UINT *pWidth, UINT *pHeight);
	void OnJpgLoad();
	afx_msg void OnRgbToHsi();
	float** hueBuffer;
	float** satuBuffer;
	float** intenBuffer;
	int viewType;
};

#ifndef _DEBUG  // DigitalImageProcessingView.cpp의 디버그 버전
inline CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const
   { return reinterpret_cast<CDigitalImageProcessingDoc*>(m_pDocument); }
#endif

