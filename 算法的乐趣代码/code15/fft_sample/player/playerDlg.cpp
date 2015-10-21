
// playerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "player.h"
#include "playerDlg.h"
#include "spectrum.h"
#include "Spline3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define FFT_SIZE 2048
#define SAMPLE_RATE 44100

#define EQ_BAND_COUNT   18

double CurvePointX[EQ_BAND_COUNT + 2] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0  };
double CurvePointY[EQ_BAND_COUNT + 2] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0  };
double InterpolationX[EQ_BAND_COUNT + 2] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0  };
double InterpolationY[EQ_BAND_COUNT + 2] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
int bandFreq[EQ_BAND_COUNT] = { 55, 77, 110, 156, 220, 311, 440, 622, 880, 1200, 1800, 2500, 3500, 5000, 7000, 10000, 14000, 20000 };

static CplayerDlg *globalWin = NULL;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void SetGlobalWindow(CplayerDlg *win)
{
    globalWin = win;
}

CplayerDlg *GetGlobalWindow()
{
    return globalWin;
}

// CplayerDlg �Ի���

void WavePlayCallback(PVOID pData, int dataLen, int bytePerSample, int channel)
{
    int samples = dataLen / bytePerSample;

    CplayerDlg *pDlg = GetGlobalWindow();
    if(pDlg)
    {
        pDlg->UpdateSpectrum((short *)pData, samples, channel);
    }
}

void WaveBufferReady(PVOID pData, int dataLen, int sampleRate, int bytePerSample, int channel)
{
    int samples = dataLen / bytePerSample;
    CplayerDlg *pDlg = GetGlobalWindow();
    if(pDlg)
    {
        pDlg->PlayBufferReady((short *)pData, samples, channel);
    }
}

CplayerDlg::CplayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CplayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bEnableEQ = FALSE;
}

void CplayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CplayerDlg::UpdateSpectrum(short *sampleData, int totalSamples, int channels)
{
    float power[FFT_SIZE];
    if(PowerSpectrumS(&m_hFFT, sampleData, totalSamples, channels, power))
    {
        int fpFen = FFT_SIZE / 2 / BAND_COUNT;

        int level[BAND_COUNT];
        for(int i = 0; i < BAND_COUNT; i++)
        {
            int cPos = i * fpFen + fpFen / 2;
            double bandTotal = power[cPos - 2] * 0.1 + power[cPos - 1] * 0.15 
                               + power[cPos] * 0.5 + power[cPos + 1] * 0.15 + power[cPos + 2] * 0.1;
            
            level[i] = (int)(bandTotal + 0.5);
        }
        m_SpectrumWnd.SetBandLevel(level, BAND_COUNT);
    }
}

void CplayerDlg::PlayBufferReady(short *sampleData, int totalSamples, int channels)
{
    if(m_bEnableEQ)
    {
        Equalizer(&m_hEQ, sampleData, totalSamples, channels);
    }
}

void CplayerDlg::InitEqSliderCtrls()
{
    for(UINT sliderId = IDC_SLIDE_1; sliderId <= IDC_SLIDE_18; sliderId++)
    {
        CSliderCtrl *pCtrl = (CSliderCtrl *)GetDlgItem(sliderId);
        pCtrl->SetRange(-12000, 12000, TRUE);
        pCtrl->SetPos(0);
    }
}

void CplayerDlg::ResetEqStatus(int sampleRate)
{
    double freqS = double(sampleRate) / double(FFT_SIZE);

    InterpolationX[0] = 0.0;
    InterpolationY[0] = 0.0;
    for(int i = 1; i <= EQ_BAND_COUNT; i++)
    {
        int pos = int(double(bandFreq[i]) / freqS + 0.5);
        InterpolationX[i] = pos;
        CSliderCtrl *pSliderCtr = (CSliderCtrl *)GetDlgItem(IDC_SLIDE_1 + i - 1);
        InterpolationY[i] = -pSliderCtr->GetPos() / 1000.0;
        CurvePointY[i] = InterpolationY[i];
    }
    InterpolationX[EQ_BAND_COUNT + 1] = FFT_SIZE / 2;
    InterpolationY[EQ_BAND_COUNT + 1] = 0.0;

    UpdateEqCurve();
}

void CplayerDlg::UpdateEqCurve()
{
    float gain[FFT_SIZE/2];
    SplineFitting curve, eq;

    curve.CalcSpline(CurvePointX, CurvePointY, EQ_BAND_COUNT + 2, 1, 0.0, 0.0);
    eq.CalcSpline(InterpolationX, InterpolationY, EQ_BAND_COUNT + 2, 1, 0.0, 0.0);

    double curX[FFT_SIZE/2], curY[FFT_SIZE/2];
    for(int x = 0; x < FFT_SIZE/2; x++)
    {
        curX[x] = x;
        curY[x] = curve.GetValue(x);

        gain[x] = (float)(eq.GetValue(x) * 0.75);
        //TRACE(_T("gain[%d] = %10.4f\n"), x, gain[x]);
    }

    UpdateFilter(&m_hEQ, gain, FFT_SIZE/2);
    m_CurveWnd.SetSourceSplineCurve(curX, curY, FFT_SIZE/2);
    //m_CurveWnd.SetInterpolationPoint(CurvePointX, CurvePointY, EQ_BAND_COUNT + 2);
    m_CurveWnd.InvalidateRect(NULL, TRUE);
    m_CurveWnd.UpdateWindow();
}

BEGIN_MESSAGE_MAP(CplayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_OPEBFILE, &CplayerDlg::OnBnClickedBtnOpebfile)
    ON_BN_CLICKED(IDC_BTN_PLAY, &CplayerDlg::OnBnClickedBtnPlay)
    ON_BN_CLICKED(IDC_BTN_STOP, &CplayerDlg::OnBnClickedBtnStop)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_TEST, &CplayerDlg::OnBnClickedBtnTest)
    ON_WM_VSCROLL()
    ON_BN_CLICKED(IDC_CHK_USING_EQ, &CplayerDlg::OnBnClickedChkUsingEq)
END_MESSAGE_MAP()


// CplayerDlg ��Ϣ�������

BOOL CplayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    InitEqSliderCtrls();
    SetGlobalWindow(this);
    if(!InitFft(&m_hFFT, FFT_SIZE, 1))
    {
        AfxMessageBox(_T("���ٸ���Ҷ�任��ʼ������!"));
        OnCancel();
    }

    if(!InitEqualizer(&m_hEQ, FFT_SIZE))
    {
        AfxMessageBox(_T("��������ʼ������!"));
        OnCancel();
    }

    CWnd *pFrame = GetDlgItem(IDC_STC_SPFRAME);
    CRect rcFrame;
    pFrame->GetWindowRect(rcFrame);
    ScreenToClient(rcFrame);
    rcFrame.DeflateRect(1, 1);
    rcFrame.top += 7;
    m_SpectrumWnd.SetLevelLimit(20, 130);
    m_SpectrumWnd.CreateSpectrumWnd(this, rcFrame);

    pFrame = GetDlgItem(IDC_STC_CURVE);
    pFrame->GetWindowRect(rcFrame);
    ScreenToClient(rcFrame);
    rcFrame.DeflateRect(1, 1);
    rcFrame.top += 7;
    m_CurveWnd.SetPointLogicRange(0.0, FFT_SIZE/2.0, -16.0, 16.0);
    m_CurveWnd.CreateCurveWindow(this, rcFrame);

    double curWidth = (FFT_SIZE/2.0) / (EQ_BAND_COUNT + 1);
    CurvePointX[0] = 0.0;
    CurvePointY[0] = 0.0;
    for(int i = 1; i <= EQ_BAND_COUNT; i++)
    {
        CurvePointX[i] = i * curWidth;
        CurvePointY[i] = 0.0;
    }
    CurvePointX[EQ_BAND_COUNT + 1] = 1024.0;
    CurvePointY[EQ_BAND_COUNT + 1] = 0.0;

    srand((unsigned)time(NULL));
    ResetEqStatus(44100);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CplayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CplayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CplayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CplayerDlg::OnBnClickedBtnOpebfile()
{
    if(waveOut.IsPlaying())
    {
        AfxMessageBox(_T("�ļ����ڲ��ţ���ֹͣ��!"));
        return;
    }

	CFileDialog fileDlg(TRUE, _T("wav"), 0, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("Wave Files (*.wav)|*.wav||"));
	if ( fileDlg.DoModal() == IDOK )
	{
		CString fileName = fileDlg.GetPathName();
        SetDlgItemText(IDC_EDIT_WAVFILE, fileName);
        CWave& wave = waveOut.GetWaveObject();
	    wave.Load(fileName);
        waveOut.SetDevice(monDevice);
        if(!monDevice.IsOutputFormat(wave))
        {
		    AfxMessageBox("Wave�ļ���ʽ��֧��!");
		    return;
        }

        WAVEFORMATEX fmt = wave.GetFormat();
        int bufSampleCount = int(((fmt.nSamplesPerSec / 10.0) / double(FFT_SIZE) + 0.5)) * FFT_SIZE;
        waveOut.ModifyWaveOutBufferLength(bufSampleCount * fmt.nBlockAlign);
        waveOut.SetPlayCallback(WavePlayCallback);
        waveOut.SetBufferReadyFunc(WaveBufferReady);
        GetDlgItem(IDC_BTN_PLAY)->EnableWindow(TRUE);

        ResetEqStatus(fmt.nSamplesPerSec);
	}
}

void CplayerDlg::OnBnClickedBtnPlay()
{
	if(!waveOut.Open()) 
    {
		AfxMessageBox( waveOut.GetError() );
		return;
	}
	if(!waveOut.Play()) 
    {
		AfxMessageBox( waveOut.GetError() );
		return;
	}
}

void CplayerDlg::OnBnClickedBtnStop()
{
	if(!waveOut.Close()) 
    {
		AfxMessageBox(waveOut.GetError());
	}
    //GetDlgItem(IDC_BTN_PLAY)->EnableWindow(FALSE);
}

void CplayerDlg::OnDestroy()
{
    CDialog::OnDestroy();

    waveOut.Close();
    ReleaseFft(&m_hFFT);
    ReleaseEqualizer(&m_hEQ);
}

void CplayerDlg::OnBnClickedBtnTest()
{
    int i = 0;
    int a[] = { 10, 20, 30 };
    int r = 1 * a[i++] + 2 * a[i++] + 3 * a[i++];

    if(m_SpectrumWnd.GetSafeHwnd() != NULL)
    {
        int level[BAND_COUNT];
        for(int i = 0; i < BAND_COUNT; i++)
        {
            level[i] = rand() * 128 / RAND_MAX;
        }
        m_SpectrumWnd.SetBandLevel(level, BAND_COUNT);
    }
}


void CplayerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CSliderCtrl *pSliderCtr = (CSliderCtrl *)pScrollBar;

    int index = pSliderCtr->GetDlgCtrlID() - IDC_SLIDE_1;
    InterpolationY[index + 1] = -pSliderCtr->GetPos() / 1000.0;
    CurvePointY[index + 1] = InterpolationY[index + 1];

    UpdateEqCurve();

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CplayerDlg::OnBnClickedChkUsingEq()
{
    m_bEnableEQ = !m_bEnableEQ;
}
