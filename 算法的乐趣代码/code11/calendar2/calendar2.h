
// calendar2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ccalendar2App:
// �йش����ʵ�֣������ calendar2.cpp
//

class Ccalendar2App : public CWinAppEx
{
public:
	Ccalendar2App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ccalendar2App theApp;