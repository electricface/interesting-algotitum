#pragma once

#include "DayInfo.h"

typedef struct tagMonthInfo
{
    int month;           //���������
    int days;            //����������
    int first_week;      //����1�ŵ�����
    double jieqiJD;        //���½�����JD������ʱ��
    SOLAR_TERMS jieqi;     //���½������
    double zhongqiJD;      //����������JD������ʱ��
    SOLAR_TERMS zhongqi;   //�������������
}MONTH_INFO;

class CChineseCalendar;

class CMonthInfo
{
    friend class CChineseCalendar;
protected:
    CMonthInfo(const MONTH_INFO *info);
public:
    ~CMonthInfo(void);
    void SetMonthInfo(const MONTH_INFO *info);
    void GetMonthInfo(MONTH_INFO *info);
    const CDayInfo& GetDayInfo(int day) const;
    void AddSingleDay(CDayInfo& info);
    int GetMonthIndex() const { return m_Info.month; };
    int GetDaysCount() const { return m_Info.days; };
    int GetFirstDayWeek() const { return m_Info.first_week; };
    bool CheckValidDayCount();
    void ClearInfo();
protected:
    MONTH_INFO m_Info;

    std::vector<CDayInfo> m_DayInfo;
};
