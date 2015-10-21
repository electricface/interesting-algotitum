
// playerDlg.h : ͷ�ļ�
//

#pragma once

#include "Wave.h"
#include "WaveDevice.h"
#include "WaveOut.h"
#include "fft.h"
#include "equalizer.h"
#include "SpectrumWnd.h"
#include "DisplayWnd.h"


// CplayerDlg �Ի���
class CplayerDlg : public CDialog
{
// ����
public:
	CplayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
    void UpdateSpectrum(short *sampleData, int totalSamples, int channels);
    void PlayBufferReady(short *sampleData, int totalSamples, int channels);
    void ResetEqStatus(int sampleRate);
    void UpdateEqCurve();

// ʵ��
protected:
	HICON m_hIcon;
	CWaveOut waveOut;
    CWaveDevice monDevice;
    FFT_HANDLE m_hFFT;
    EQUALIZER_HANDLE m_hEQ;
    CSpectrumWnd m_SpectrumWnd;
    CDisplayWnd m_CurveWnd;
    BOOL m_bEnableEQ;

    void InitEqSliderCtrls();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedBtnOpebfile();
    afx_msg void OnBnClickedBtnPlay();
    afx_msg void OnBnClickedBtnStop();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnTest();
public:
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnBnClickedChkUsingEq();
};
