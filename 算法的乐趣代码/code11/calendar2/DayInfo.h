#pragma once

#include "calendar_const.h"

typedef struct tagDayInfo
{
    int dayNo;         //�����ڵ������
    int week;          //���յ�����
    int mdayNo;        //���ն�Ӧ��ũ�����������
    int mmonth;        //�������ڵ�ũ�������
    SOLAR_TERMS st;    //���ն�Ӧ�Ľ�����-1 ��ʾ���ǽ���
}DAY_INFO;

class CDayInfo
{
public:
    CDayInfo(const DAY_INFO *info);
    ~CDayInfo(void);

    void SetDayInfo(const DAY_INFO *info);
    void GetDayInfo(DAY_INFO *info);
    int GetDayDate() const { return m_Info.dayNo; };
    int GetDayChnDate() const { return m_Info.mdayNo; };
    int GetDayChnMonth() const { return m_Info.mmonth; };
    SOLAR_TERMS GetDaySolarTerm() const { return m_Info.st; };
    int GetDayWeek() const { return m_Info.week; };
protected:
    DAY_INFO m_Info;
};
