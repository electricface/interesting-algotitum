
// plot_spectrum.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cplot_spectrumApp:
// �йش����ʵ�֣������ plot_spectrum.cpp
//

class Cplot_spectrumApp : public CWinAppEx
{
public:
	Cplot_spectrumApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cplot_spectrumApp theApp;