#pragma once

typedef struct tagChnMonthInfo
{
    int mmonth;           //ũ�������
    int mname;            //ũ��������
    int mdays;            //��������
    double shuoJD;        //����˷�յ�JD������ʱ��
    double nextJD;        //����˷�յ�JD������ʱ��
    int leap;             //0 �������£�1 ������
}CHN_MONTH_INFO;

class CChnMonthInfo
{
public:
    CChnMonthInfo(const CHN_MONTH_INFO *info);
    ~CChnMonthInfo(void);
    void SetChnmonthInfo(const CHN_MONTH_INFO *info);
    void GetChnmonthInfo(CHN_MONTH_INFO *info);
    void SetLeapMonth(bool leap) { m_Info.leap = leap ? 1 : 0; };
    void ReIndexMonthName() { m_Info.mname--; };
    double GetNewMoonDayJD() { return m_Info.shuoJD; };
    double GetNextNewMoonDayJD() { return m_Info.nextJD; };
    int GetMonthIndex() { return m_Info.mmonth; };
    bool IsLeapMonth() { return (m_Info.leap == 1); };
    int GetMonthDays() { return m_Info.mdays; };
    int GetMonthName() { return m_Info.mname; };
protected:
    CHN_MONTH_INFO m_Info;
};

