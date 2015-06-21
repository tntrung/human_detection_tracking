
// hogdetector_imageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hogdetector_image.h"
#include "hogdetector_imageDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
///////////////////////library
#include "hog.cpp"
#include <fstream>
using namespace cv;
using namespace std;
//////////////////////////////////
//////////////////////////parameter
//input
int rectmax_x=0;
int rectmax_y=350;
double thread=0;
int windownstride_w=8;
int windownstride_h=8;
int pading_w=0;
int pading_h=0;
double scale=1.05;
float groundthread=2;
int hogsize_w=48;
int hogsize_h=96;
CButton *btnPtr;
int frameinit=0;
int minconfident=100000,maxconfident=-100000;
//int vtframe=0;
vector<Rect> tempfound,found, found_filtered;//output hog descriptor
vector<Rect> rect_hog; 
vector<vector<Rect>> matranrect_hog;	
/////////////////////////////////////
////////////////////////////function
char* ConverterStringToChar(const CString &s)
{	
	 int nSize = s.GetLength();
	 char *pAnsiString = new char[nSize+1];
	 memset(pAnsiString,0,nSize+1);
	 wcstombs(pAnsiString, s, nSize+1);
	 return pAnsiString;
}
IplImage* resizeIpImage(IplImage *img,Rect vunghienthi)
{		
	//neu kich thuoc anh khong vuot qua kich thuoc toi da thi thoat
	if(img->width<=vunghienthi.width && img->height<=vunghienthi.height)
		return cvCloneImage(img);//khong can scale  
	int x=img->width;
	int y=img->height;
	float scale;
	if(x>vunghienthi.width)//scale theo x
	{
		scale = (float)x/vunghienthi.width;
		x=int((float)x/scale);
		y=int((float)y/scale);
	}
	if(y>vunghienthi.height)//scale theo y
	{
		scale=(float)y/vunghienthi.height;
		x=int((float)x/scale);
		y=int((float)y/scale);
	}
	//khoi tao khung nen de scale	
	IplImage *DispImage=cvCreateImage(cvSize(x,y), img->depth, img->nChannels);
	cvSetImageROI(DispImage,cvRect(vunghienthi.x,vunghienthi.y,x,y));	
    cvResize(img, DispImage);
	cvResetImageROI(DispImage);    //}
	return DispImage;  	
}
Mat resizemat(Mat &img,cv::Rect local)
{	
	IplImage *DispImage=resizeIpImage(&IplImage(img),local);    
	img = Mat(DispImage,true);
	cvReleaseImage(&DispImage);
	return img;
}
Rect resizerect(Rect a)
{
	int w=a.width;
	a.width=a.height*42/128;
	a.x=a.x+(w-a.width)/2;
	return a;
}
bool tuongdong(Rect a,Rect b,int nguong)
{
	//tinh khoang cach khong vuot nguong
	float x1=a.x+a.width/2;
	float y1=a.y+a.height/2;
	float x2=b.x+b.width/2;
	float y2=b.y+b.height/2;
	if((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)>nguong*nguong)
		return false;
	return true;
}
vector<Rect> groupRectangles_fix(vector<Rect>&rect,int nguong)
{
	vector<Rect> temprect;
	for(int i=0;i<rect.size();i++)
	{
		int dem=0;
		for(int j=0;j<rect.size();j++)
			if(tuongdong(rect[i],rect[j],nguong))//neu 2 rect gan nhau
				dem++;
		if(dem>1)// co it nhat 2 box tuong dong
		{
			bool flag=false;
			for(int k=0;k<temprect.size();k++)//kiem tra co ton tai hay chua
				if(!tuongdong(rect[i],temprect[k],nguong))
					flag=true;
			if(!flag)
				temprect.push_back(rect[i]);
		}
	}
	return temprect;
}
void convertocolor(Mat &img)
{
	
	int height,width,step,channels;
	uchar *data;
	int i,j,k;
	height    = img.size().height;
	width     = img.size().width;
	step      = img.step;
	channels  = img.channels();
	data      = (uchar *)img.data;
	for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
	{
		data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
	}
}
Mat  hogdetector(const HOGDescriptor &hog,const char* argv)
{	

	//Mat img;
	Mat img= imread(argv);
    if( !img.data ) return img; 
	//img= convertergri(img);
	found.clear();
	found_filtered.clear();	
	
	//hog.detectMultiScale_khunhieu(img, found,rectmax_x,rectmax_y, thread, Size(windownstride_w,windownstride_h), Size(pading_w,pading_h), scale,groundthread);				
	//hog.detectMultiScale_mex(img, found, thread, Size(windownstride_w,windownstride_h), Size(pading_w,pading_h), scale,groundthread);					
	hog.detectMultiScale_track(img,found,thread,Size(windownstride_w,windownstride_h),Size(pading_w,pading_h),scale,groundthread,rectmax_x,rectmax_y,btnPtr->GetCheck()==1);	
	if(found.empty())
		return img;
	size_t i, j;
	for( i = 0; i < found.size(); i++ )
	{
		Rect r = found[i];
		rectangle(img,r.tl(),r.br(),cv::Scalar(255,255,255),3);
		for( j = 0; j < found.size(); j++ )
			if( j != i && (r & found[j]) == r)
				break;
		if( j == found.size() )
			found_filtered.push_back(r);
	}
	for( i = 0; i < found_filtered.size(); i++ )
	{
		Rect r = found_filtered[i];
		// the HOG detector returns slightly larger rectangles than the real objects.
		// so we slightly shrink the rectangles to get a nicer output.
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
		rect_hog.push_back(r);
	}	
	return img;
}
void savefilekey(char* path,vector<vector<Rect>> &matranrect,int begin)//,const vector<vector<float>>&confident)
{	
	if(matranrect.size()<1)
		return;
	ofstream Ffile;
	if(begin<1)//ghi de file
	{
		begin=0;
		Ffile.open (path);			
		//vtframe=0;
	}
	else //ghi them vao
		Ffile.open (path,5);	
	if(! Ffile.is_open())return;
	for(int i=0;i<matranrect.size();i++)
	{
		for(int j=0;j<(int)matranrect[i].size();j++)
		{
			//in dong cot
			Ffile<<begin+i<<" "<<j<<" ";
			//in trung binh
			Ffile<<matranrect[i][j].x+matranrect[i][j].width/2<<" ";
			Ffile<<matranrect[i][j].y+matranrect[i][j].height/2<<" ";
			//in ti so
			Ffile<<j+1<<" 0 ";//"<<vtframe<<" ";//confident[i][j];			
			//in vung rect
			Ffile <<matranrect[i][j].x<<" "<<matranrect[i][j].y<<" "<<matranrect[i][j].width<<" "<<matranrect[i][j].height<<" \n";
			//in cac so 0 cho giong ket qua .key cua ism
			//Ffile<<"0 0 0 0 0 0 0 0 0 0 0 0 0\n";						
		}
	}	
	Ffile.close();	
}
///////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// Chogdetector_imageDlg dialog




Chogdetector_imageDlg::Chogdetector_imageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Chogdetector_imageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Chogdetector_imageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//get parameter		
	DDX_Text(pDX, IDC_RECTMAX_X,rectmax_x );	
	DDX_Text(pDX, IDC_RECTMAX_Y, rectmax_y);	
	DDX_Text(pDX, IDC_THREAD, thread);	
	DDX_Text(pDX, IDC_WINDOWNSTRIDE_W, windownstride_w );	
	DDX_Text(pDX, IDC_WINDOWNSTRIDE_H, windownstride_h);	
	DDX_Text(pDX, IDC_PADING_W, pading_w);	
	DDX_Text(pDX, IDC_PADING_H, pading_h);	
	DDX_Text(pDX, IDC_SCALE, scale);	
	DDX_Text(pDX, IDC_GROUNDTHREAD, groundthread);	
	DDX_Text(pDX, IDC_FRAMEINIT, frameinit);		
	DDX_Text(pDX, IDC_MINCONFIDENT, minconfident);		
	DDX_Text(pDX, IDC_MAXCONFIDENT, maxconfident);	
	DDX_Text(pDX, IDC_HOGSIZE_W, hogsize_w);	
	DDX_Text(pDX, IDC_HOGSIZE_H, hogsize_h);	
}

BEGIN_MESSAGE_MAP(Chogdetector_imageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Chogdetector_imageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EXIT, &Chogdetector_imageDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_DATASET, &Chogdetector_imageDlg::OnBnClickedDataset)
	ON_EN_CHANGE(IDC_RECTMAX_X, &Chogdetector_imageDlg::OnEnChangeRectmaxX)
END_MESSAGE_MAP()


// Chogdetector_imageDlg message handlers

BOOL Chogdetector_imageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	/////////////////////////////////////////////////set tabstop
	// From the parent window of the the controls...
	UINT nFlags=SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE; // Ignore window size and position parameters in SetWindowPos calls.
	CWnd* pAfter; // Ptr to window we want pWnd after.
	pAfter =GetDlgItem(IDC_RECTMAX_X);
	pAfter->SetWindowPos(&wndTopMost,0,0,0,0,nFlags);
	CWnd* pWnd; // Ptr to window we're changing.
	pWnd =GetDlgItem(IDC_RECTMAX_Y);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);	
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_THREAD);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);	
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_WINDOWNSTRIDE_W);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_WINDOWNSTRIDE_H);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_PADING_W);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_PADING_H);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_SCALE);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_GROUNDTHREAD);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_HOGSIZE_W);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_HOGSIZE_H);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_MEANSIFT);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_FRAMEINIT);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_DATASET);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDOK);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);
	pAfter =pWnd;
	pWnd =GetDlgItem(IDC_EXIT);
	pWnd->SetWindowPos(pAfter,0,0,0,0,nFlags);		
	/////////////////////////////////////////khoi tao picturebox
	cvNamedWindow("anhgoc", 1);
	HWND hWndg = (HWND) cvGetWindowHandle("anhgoc");
	HWND hParentg = ::GetParent(hWndg);
	::SetParent(hWndg, GetDlgItem(IDC_PICTURE)->m_hWnd);
	::ShowWindow(hParentg, SW_HIDE);
	///////////////////////////////////
	//checkbox meansift
	btnPtr = (CButton *) GetDlgItem(IDC_MEANSIFT);
	btnPtr->SetCheck(1);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Chogdetector_imageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Chogdetector_imageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Chogdetector_imageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Chogdetector_imageDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//HOGDescriptor hog;			
	HOGDescriptor hog(cv::Size(hogsize_w,hogsize_h),cv::Size(16,16),cv::Size(8,8),cv::Size(8,8),9,1,-1,HOGDescriptor::L2Hys,0.2,true,64);
	if(hogsize_w==64 && hogsize_h==128)
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	else if(hogsize_w==48 && hogsize_h==96)
		hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
	else
	{			
		MessageBox(L"Hog size no support",NULL,NULL);
		return;
	}
	cv::mincf=10000;
	cv::maxcf=-10000;
	CFileDialog dlg(TRUE, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY, _T("image files (*.bmp; *.jpg; *.png) |*.bmp;*.jpg;*.png|All Files (*.*)|*.*||"),NULL);  
	dlg.m_ofn.lpstrTitle= _T("Open Image");  
	if (dlg.DoModal() == IDOK) { 
		
		Mat img=hogdetector(hog,ConverterStringToChar(dlg.GetPathName()));				
		if(img.data)
		{
			Rect vunghienthi= Rect(0,0,550,415);
			//Rect r=Rect(0,0,rectmax_x,rectmax_y);
			imshow("anhgoc", resizemat(img,vunghienthi));	
			cvWaitKey(1);
		}
	}
	minconfident=(int)(cv::mincf*1000);
	maxconfident=(int)(cv::maxcf*1000);
	cv::minnguong=100000;
	cv::maxnguong=-100000;
	UpdateData(FALSE);	
}


void Chogdetector_imageDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	//CDialogEx::OnCancel();	
}


void Chogdetector_imageDlg::OnBnClickedDataset()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	HOGDescriptor hog(cv::Size(hogsize_w,hogsize_h),cv::Size(16,16),cv::Size(8,8),cv::Size(8,8),9,1,-1,HOGDescriptor::L2Hys,0.2,true,64);
	if(hogsize_w==64 && hogsize_h==128)
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	else if(hogsize_w==48 && hogsize_h==96)
		hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
	else
	{
		MessageBox(L"Hog size no support",NULL,NULL);
		return;
	}
	CFolderPickerDialog dlpk;
	if(dlpk.DoModal()==IDOK)
	{
		char*linkdata=ConverterStringToChar(dlpk.GetFolderPath());	
		char*savekeyfile;
		char linkimage[100];
		char savelinkimage[100];
		bool saveimage=true;
		CFolderPickerDialog sdlpk;		
		rect_hog.clear();
		if(sdlpk.DoModal()==IDOK)
		{
			savekeyfile=ConverterStringToChar(sdlpk.GetFolderPath());	
			matranrect_hog.clear();	
		}
		else
			saveimage=false;		
		int fframe=frameinit;
		int lensave=10;
		char string[33];			
		while(1){
			//mincf va maxcf tu dinh nghia trong hog.cpp
			cv::mincf=10000;
			cv::maxcf=-10000;
			strcpy(linkimage,linkdata);
			strcat(linkimage,"\\image (");
			itoa(++fframe,string,10);
			strcat(linkimage,string);
			strcat(linkimage,").jpg");			
			Mat img=hogdetector(hog,linkimage);
			if(!img.data) break;			
			Rect vunghienthi= Rect(0,0,550,415);
			imshow("anhgoc", resizemat(img,vunghienthi));	
			cvWaitKey(100);
			if(saveimage)
				matranrect_hog.push_back(rect_hog);	
			rect_hog.clear();
			if(saveimage)
			{
				strcpy(savelinkimage,savekeyfile);		
				strcat(savelinkimage,"\\image (");
				strcat(savelinkimage,string);
				strcat(savelinkimage,")_save.jpg");
				imwrite(savelinkimage,img);
				if(matranrect_hog.size()>lensave-1 )
				{			
					strcpy(savelinkimage,savekeyfile);		
					strcat(savelinkimage,"\\ground_hog_key");					
					savefilekey(savelinkimage,matranrect_hog,fframe-lensave);
					matranrect_hog.clear();
				}
			}
			frameinit++;
			minconfident=(int)(cv::mincf*1000);
			maxconfident=(int)(cv::maxcf*1000);
			UpdateData(FALSE);
		};
		if(saveimage)
		{
			strcpy(savelinkimage,savekeyfile);		
			strcat(savelinkimage,"\\ground_hog_key");
			if(fframe%lensave==0 && fframe)//phai giam di lensave lan
				fframe-=lensave;
			savefilekey(savelinkimage,matranrect_hog,fframe-fframe%lensave);
		}
		matranrect_hog.clear();
		delete linkdata,savekeyfile;
	}	
}


void Chogdetector_imageDlg::OnEnChangeRectmaxX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
