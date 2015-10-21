
// calendar2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "calendar2.h"
#include "calendar2Dlg.h"
#include "ChineseCalendar.h"
#include "support.h"
#include "calendar_func.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// Ccalendar2Dlg �Ի���




Ccalendar2Dlg::Ccalendar2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ccalendar2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_CurrentMonth = 0;
}

void Ccalendar2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STC_CALENDAR, m_CalendarCtrl);
}

BEGIN_MESSAGE_MAP(Ccalendar2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_SET_YEAR, &Ccalendar2Dlg::OnBnClickedBtnSetYear)
    ON_BN_CLICKED(IDC_BTN_LAST_MONTH, &Ccalendar2Dlg::OnBnClickedBtnLastMonth)
    ON_BN_CLICKED(IDC_BTN_NEXT_MONTH, &Ccalendar2Dlg::OnBnClickedBtnNextMonth)
    ON_BN_CLICKED(IDC_BTN_INFORMATION, &Ccalendar2Dlg::OnBnClickedBtnInformation)
    ON_BN_CLICKED(IDC_BTN_TEST, &Ccalendar2Dlg::OnBnClickedBtnTest)
    ON_BN_CLICKED(IDC_BTN_SOLARTERM, &Ccalendar2Dlg::OnBnClickedBtnSolarterm)
    ON_BN_CLICKED(IDC_BTN_NEWMOON, &Ccalendar2Dlg::OnBnClickedBtnNewmoon)
END_MESSAGE_MAP()


// Ccalendar2Dlg ��Ϣ�������

BOOL Ccalendar2Dlg::OnInitDialog()
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
    SetDlgItemText(IDC_EDIT_YEAR, _T("2015"));
    m_CalendarCtrl.SetGeriYear(2015);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Ccalendar2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ccalendar2Dlg::OnPaint()
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
HCURSOR Ccalendar2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ccalendar2Dlg::OnBnClickedBtnSetYear()
{
    int year = GetDlgItemInt(IDC_EDIT_YEAR);
    if(year < 1600)
    {
        AfxMessageBox(_T("��֧��1600����ǰ����ʷũ�����㣡"));
        return;
    }
    
    m_CalendarCtrl.SetGeriYear(year);
}

void Ccalendar2Dlg::OnBnClickedBtnLastMonth()
{
    m_CalendarCtrl.LastMonth();
}

void Ccalendar2Dlg::OnBnClickedBtnNextMonth()
{
    m_CalendarCtrl.NextMonth();
}

void Ccalendar2Dlg::OnBnClickedBtnInformation()
{
    // TODO: Add your control notification handler code here
}

void Ccalendar2Dlg::OnBnClickedBtnSolarterm()
{
    // TODO: Add your control notification handler code here
}

void Ccalendar2Dlg::OnBnClickedBtnNewmoon()
{
    // TODO: Add your control notification handler code here
}

/*��ĳһ���ĳ��������ʼ����������25�����������ظ������������գ�����ʱ��*/
void GetAllSolarTermsJD(int year, double *SolarTerms, int start)
{
    int i = 0;
    int st = start;
    while(i < 25)
    {
        double jd = CalculateSolarTerms(year, st * 15);
        SolarTerms[i++] = JDTDtoLocalTime(jd);
        if(st == WINTER_SOLSTICE)
        {
            year++;
        }
        st = (st + 1) % SOLAR_TERMS_COUNT;
    }
}

void PrintJDTimes(double *jds, int count)
{
    char timeStr[128];

    TRACE("\n");
    for(int i = 0; i < count; i++)
    {
        JDtoString(jds[i], timeStr, 128);
        TRACE("%f, %s\n", jds[i], timeStr);
    }
    TRACE("\n");
}

void Ccalendar2Dlg::OnBnClickedBtnTest()
{
    int this_year = 2011;

    double solarTerms[25];
    GetAllSolarTermsJD(this_year - 1, solarTerms, WINTER_SOLSTICE);
    PrintJDTimes(solarTerms, 25);

}
