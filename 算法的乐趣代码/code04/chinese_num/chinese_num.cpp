// chinese_num.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <cassert>

//using namespace std;

const int CHN_NUM_CHAR_COUNT = 10;
const int CHN_CHAR_LENGTH = 2;

const char *chnNumChar[CHN_NUM_CHAR_COUNT] = { "��", "һ", "��", "��", "��", "��", "��", "��", "��", "��" };
const char *chnUnitChar[] = { "", "ʮ", "��", "ǧ" };
const char *chnUnitSection[] = { "", "��", "��", "����" };

typedef struct 
{
    const char *name; //����Ȩλ����
    int value; //10�ı���ֵ
    bool secUnit; //�Ƿ��ǽ�Ȩλ
}CHN_NAME_VALUE;

CHN_NAME_VALUE chnValuePair[] = 
{
    { "ʮ", 10, false }, { "��", 100, false }, { "ǧ", 1000, false }, 
    { "��", 10000, true }, { "��", 100000000, true }
};



void SectionToChinese(unsigned int section, std::string& chnStr)
{
    chnStr.clear();
    std::string strIns;
    int unitPos = 0;
    bool zero = true;
    while(section > 0)
    {
        int v = section % 10;
        if(v == 0)
        {
            if( (section == 0) || !zero )
            {
                zero = true; /*��Ҫ��0��zero��������ȷ���������Ķ��0��ֻ��һ��������*/
                chnStr.insert(0, chnNumChar[v]);
            }
        }
        else
        {
            zero = false; //������һ�����ֲ���0
            strIns = chnNumChar[v]; //��λ��Ӧ����������
            strIns += chnUnitChar[unitPos]; //��λ��Ӧ������Ȩλ
            chnStr.insert(0, strIns); 
        }
        unitPos++; //��λ
        section = section / 10;
    }
}

//num == 0��Ҫ���⴦��ֱ�ӷ���"��"
void NumberToChinese(unsigned int num, std::string& chnStr)
{
    chnStr.clear();
    int unitPos = 0;
    std::string strIns;
    bool needZero = false;

    if(num == 0)
    {
        chnStr = chnNumChar[0];
        return;
    }

    while(num > 0)
    {
        unsigned int section = num % 10000;
        if(needZero)
        {
            chnStr.insert(0, chnNumChar[0]);
        }
        SectionToChinese(section, strIns);
        /*�Ƿ���Ҫ��Ȩλ��*/
        strIns += (section != 0) ? chnUnitSection[unitPos] : chnUnitSection[0]; 
        chnStr.insert(0, strIns);
        /*ǧλ��0����Ҫ����һ��section����*/
        needZero = (section < 1000) && (section > 0); 
        num = num / 10000;
        unitPos++;
    }
}

int ChineseToValue(const std::string& chnStr)
{
    for(int val = 0; val < sizeof(chnNumChar) / sizeof(chnNumChar[0]); val++)
    {
        if(chnStr.compare(chnNumChar[val]) == 0)
        {
            return val;
        }
    }

    return -1;
}

int ChineseToUnit(const std::string& chnStr, bool& secUnit)
{
    for(int unit = 0; unit < sizeof(chnValuePair) / sizeof(chnValuePair[0]); unit++)
    {
        if(chnStr.compare(chnValuePair[unit].name) == 0)
        {
            secUnit = chnValuePair[unit].secUnit;
            return chnValuePair[unit].value;
        }
    }

    return 1;
}

unsigned int ChineseToNumber(const std::string& chnString)
{
    unsigned int rtn = 0;
    unsigned int section = 0;
    int number = 0;
    bool secUnit = false;
    std::string::size_type pos = 0;
    
    while(pos < chnString.length())
    {
        int num = ChineseToValue(chnString.substr(pos, CHN_CHAR_LENGTH));
        if(num >= 0) /*���ֻ��ǵ�λ��*/
        {
            number = num;
            pos += CHN_CHAR_LENGTH;
            if(pos >= chnString.length())//��������һλ���֣�ֱ�ӽ���
            {
                section += number;
                rtn += section;
                break;
            }
        }
        else
        {
            int unit = ChineseToUnit(chnString.substr(pos, CHN_CHAR_LENGTH), secUnit);
            if(secUnit)//�ǽ�Ȩλ˵��һ�����Ѿ�����
            {
                section = (section + number) * unit;
                rtn += section;
                section = 0;
            }
            else
            {
                section += (number * unit);
            }
            number = 0;
            pos += CHN_CHAR_LENGTH;
            if(pos >= chnString.length())
            {
                rtn += section;
                break;
            }
        }
    }

    return rtn;
}

typedef struct 
{
    int num;
    const char *chnNumStr;
}TEST_DATA;

TEST_DATA testPair[] = 
{
    {0, "��"},
    {1, "һ"},
    {2, "��"},
    {3, "��"},
    {4, "��"},
    {5, "��"},
    {6, "��"},
    {7, "��"},
    {8, "��"},
    {9, "��"},
    {10, "һʮ"},
    {11, "һʮһ"},
    {110, "һ��һʮ"},
    {111, "һ��һʮһ"},
    {100, "һ��"},
    {102, "һ�����"},
    {1020, "һǧ���ʮ"},
    {1001, "һǧ��һ"},
    {1015, "һǧ��һʮ��"},
    {1000, "һǧ"},
    {10000, "һ��"},
    {20010, "������һʮ"},
    {20001, "������һ"},
    {100000, "һʮ��"},
    {1000000, "һ����"},
    {10000000, "һǧ��"},
    {100000000, "һ��"},
    {1000000000, "һʮ��"},
    {1000001000, "һʮ��һǧ"},
    {1000000100, "һʮ����һ��"},
    {200010, "��ʮ����һʮ"},
    {2000105, "��������һ������"},
    {20001007, "��ǧ��һǧ����"},
    {2000100190, "��ʮ����һʮ����һ�پ�ʮ"},
    {1040010000, "һʮ����ǧ��һ��"},
    {200012301, "������һ���ǧ������һ"},
    {2005010010, "��ʮ���������һ����һʮ"},
    {4009060200, "��ʮ����Ű������������"},
    {4294967295, "��ʮ���ھ�ǧ�İپ�ʮ������ǧ���پ�ʮ��"}
};

void testNumberToChinese()
{
    std::string chnNum;

    for(int i = 0; i < sizeof(testPair)/sizeof(testPair[0]); i++)
    {
        NumberToChinese(testPair[i].num, chnNum);
        assert(strcmp(chnNum.c_str(), testPair[i].chnNumStr) == 0);
    }
}

void testChineseToNumber()
{
    for(int i = 0; i < sizeof(testPair)/sizeof(testPair[0]); i++)
    {
        unsigned int num = ChineseToNumber(testPair[i].chnNumStr);
        assert(num == testPair[i].num);
    }
}

int main(int argc, char* argv[])
{
    testNumberToChinese();
    testChineseToNumber();
	return 0;
}

