// gs2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


const int UNIT_COUNT = 3;

typedef struct tagPartner
{
    char *name;   //����
    int next;     //��һ���������
    int current;  //��ǰ��飬-1��ʾ��û�����
    int pCount; //ƫ���б���������
    int perfect[UNIT_COUNT]; //ƫ���б�
}PARTNER;


//����Ҳ���㷨һ���Դ����һ������
int GetPerfectPosition(PARTNER *partner, int id)
{
    for(int i = 0; i < partner->pCount; i++)
    {
        if(partner->perfect[i] == id)
        {
            return i;
        }
    }

    //����һ���ǳ����ֵ����ζ�Ÿ����Ų��϶�
    return 0x7FFFFFFF;
}

void PrintResult(PARTNER *boys, PARTNER *girls, int count)
{
    for(int i = 0; i < count; i++)
    {
        std::cout << boys[i].name << "[" 
                  << GetPerfectPosition(&boys[i], boys[i].current)
                  << "] <---> " << girls[boys[i].current].name << "["
                  << GetPerfectPosition(&girls[boys[i].current], i)
                  << "]" << std::endl;
    }
    std::cout << std::endl;
}

bool IsPartnerAssigned(PARTNER *partner)
{
    return (partner->current != -1);
}

bool IsFavoritePartner(PARTNER *partner, int bid)
{
    for(int i = 0; i < partner->pCount; i++)
    {
        if(partner->perfect[i] == bid) 
        {
            return true;
        }
    }

    return false;
}

bool IsStableMatch(PARTNER *boys, PARTNER *girls)
{
    for(int i = 0; i < UNIT_COUNT; i++)
    {
        //�ҵ��к���ǰ������Լ���ƫ���б��е�λ��
        int gpos = GetPerfectPosition(&boys[i], boys[i].current);
        //��positionλ��֮ǰ����飬�к�ϲ������ʤ��current
        for(int k = 0; k < gpos; k++)
        {
            int gid = boys[i].perfect[k];
            //�ҵ��к������Ů����ƫ���б��е�λ��
            int bpos = GetPerfectPosition(&girls[gid], i);
            //�ҵ�Ů���ĵ�ǰ��������Ů����ƫ���б��е�λ��
            int cpos = GetPerfectPosition(&girls[gid], girls[gid].current);
            if(bpos < cpos)
            {
                //Ů��Ҳ��ϲ������к�ʤ��ϲ���Լ���ǰ����飬���ǲ��ȶ�����
                return false;
            }
        }
    }

    return true;
}

int totalMatch = 0;
int stableMatch = 0;
void SearchStableMatch(int index, PARTNER *boys, PARTNER *girls)
{
    if(index == UNIT_COUNT)
    {
        totalMatch++;
        if(IsStableMatch(boys, girls))
        {
            stableMatch++;
            PrintResult(boys, girls, UNIT_COUNT);
        }
        return;
    }

    for(int i = 0; i < boys[index].pCount; i++)
    {
        int gid = boys[index].perfect[i];
        
        if(!IsPartnerAssigned(&girls[gid]) && IsFavoritePartner(&girls[gid], index))
        {
            boys[index].current = gid;
            girls[gid].current = index;
            SearchStableMatch(index + 1, boys, girls);
            boys[index].current = -1;
            girls[gid].current = -1;
        }
    }
}

int main(int argc, char* argv[])
{
#if 1
    PARTNER girls[] = 
    {
        {"Laura", 0, -1, 3, {2,0,1} }, //Laura
        {"Marcy", 0, -1, 3, {0,2,1} },  //Marcy
        {"Nancy", 0, -1, 3, {1,0,2} } //Nancy
    };

    PARTNER boys[] = 
    {
        {"Albert", 0, -1, 3, {0,2,1} }, //Albert
        {"Brad",   0, -1, 3, {1,2,0} }, //Brad
        {"Chuck",  0, -1, 3, {0,1,2} } //Chuck
    };

/*
Albert Laura Nancy Marcy
Brad Marcy Nancy Laura
Chuck Laura Marcy Nancy

Laura Chuck Albert Brad
Marcy Albert Chuck Brad
Nancy Brad Albert Chuck


Albert Nancy
Brad Marcy
Chuck Laura
*/
#endif
#if 0
PARTNER girls[] = 
{
    {"A", 0, -1, 4, {2,3,1,0} },
    {"B", 0, -1, 4, {2,1,3,0} },
    {"C", 0, -1, 4, {0,2,3,1} },
    {"D", 0, -1, 4, {1,3,2,0} }
};
PARTNER boys[] = 
{
    {"1", 0, -1, 4, {0,3,2,1} },
    {"2", 0, -1, 4, {0,1,2,3} },
    {"3", 0, -1, 4, {0,2,3,1} },
    {"4", 0, -1, 4, {1,0,3,2} }
};

/*
A: 3,4,2,1
B: 3,2,4,1
C: 1,3,4,2
D: 2,4,3,1

1: A,D,C,B
2: A,B,C,D
3: A,C,D,B
4: B,A,D,C
*/
#endif

#if 0
PARTNER girls[] = 
{
    {"girl0", 0, -1, 5, {2,1,4,0,3} },
    {"girl1", 0, -1, 5, {1,3,0,4,2} },
    {"girl2", 0, -1, 5, {0,1,2,4,3} },
    {"girl3", 0, -1, 5, {2,0,3,1,4} },
    {"girl4", 0, -1, 5, {4,1,2,3,0} }
};

PARTNER boys[] = 
{
    {"boy0", 0, -1, 5, {4,1,0,3,2} },
    {"boy1", 0, -1, 5, {3,1,4,0,2} },
    {"boy2", 0, -1, 5, {2,3,4,0,1} },
    {"boy3", 0, -1, 5, {4,0,3,1,2} },
    {"boy4", 0, -1, 5, {3,4,2,0,1} },
};
#endif

    SearchStableMatch(0, boys, girls);
    std::cout << "Total Matchs : " << totalMatch << std::endl;
    std::cout << "Stable Matchs : " << stableMatch << std::endl;

    return 0;
}
