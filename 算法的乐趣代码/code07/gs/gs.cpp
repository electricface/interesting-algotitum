// gs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int UNIT_COUNT = 4;

typedef struct tagPartner
{
    char *name;   //����
    int next;     //��һ���������
    int current;  //��ǰ��飬-1��ʾ��û�����
    int pCount; //ƫ���б���������
    int perfect[UNIT_COUNT]; //ƫ���б�
}PARTNER;

bool IsAllPartnerMatch(PARTNER *partners, int n)
{
    for(int i = 0; i < n; i++)
    {
        if(partners[i].current == -1)
        {
            return false;
        }
    }

    return true;
}

int FindFreePartner(PARTNER *partners, int n)
{
    for(int i = 0; i < n; i++)
    {
        if((partners[i].current == -1) && (partners[i].next < partners[i].pCount))
        {
            return i;
        }
    }

    return -1;
}
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

void InitFreeBoyStack(std::stack<int>& freeBoys, int count)
{
    for(int i = 0; i < count; i++)
    {
        freeBoys.push(i);
    }
}
#if 0
bool Gale_Shapley(BOY *boys, GIRL *girls, int count)
{
    bool success = true;
    std::stack<int> freeBoys;

    InitFreeBoyStack(freeBoys, count);
    while(!freeBoys.empty())
    {
        int bid = freeBoys.top();
        freeBoys.pop();
        int gid = boys[bid].perfect[boys[bid].next];
        if(girls[gid].current == -1)//���Ů����û����飿
        {
            boys[bid].current = gid;//�ύ�����
            girls[gid].current = bid;
        }
        else
        {
            //Ů���Ѿ�����飬ǰ�����bpid
            int bpid = girls[gid].current;
            if(GetGirlPerfectPosition(&girls[gid], bpid) > GetGirlPerfectPosition(&girls[gid], bid))
            {
                //Ů��ϲ��bidʤ����ǰ���bpid������˦��ǰ���
                boys[bpid].current = -1; //ǰ���ָ�������
                if(boys[bpid].next >= boys[bpid].cCount)
                {
                    success = false; //ûϣ����
                    break;
                }
                freeBoys.push(bpid);
                boys[bid].current = gid; //�ύ�����
                girls[gid].current = bid;
            }
            else
            {
                //Ů����ϲ��ǰ��飬���ԣ�bid������һ�ΰ�
                if(boys[bid].next >= boys[bid].cCount)
                {
                    success = false; //ûϣ����
                    break;
                }
                freeBoys.push(bid);
            }
        }
        boys[bid].next++; //�����Ƿ���Գɹ�����ͬһ��Ů��ֻ�������һ��
    }

    return success;
}
#endif
#if 0
bool Gale_Shapley(BOY *boys, GIRL *girls, int count)
{
    bool success = true;
    std::stack<int> freeBoys;

    InitFreeBoyStack(freeBoys, count);
    while(!freeBoys.empty())
    {
        int bid = freeBoys.top();
        freeBoys.pop();
        for(int i = boys[bid].next; i < boys[bid].pCount; i++)
        {
            int gid = boys[bid].perfect[i];
            if(girls[gid].current == -1)//���Ů����û����飿
            {
                boys[bid].current = gid;//�ύ�����
                girls[gid].current = bid;
                boys[bid].next = i + 1;
                break;
            }
            else
            {
                //Ů���Ѿ�����飬ǰ�����bpid
                int bpid = girls[gid].current;
                if(GetGirlPerfectPosition(&girls[gid], bpid) > GetGirlPerfectPosition(&girls[gid], bid))
                {
                    //Ů��ϲ��bidʤ����ǰ���bpid������˦��ǰ���
                    boys[bpid].current = -1; //ǰ���ָ�������
                    freeBoys.push(bpid);
                    boys[bid].current = gid; //�ύ�����
                    girls[gid].current = bid;
                    boys[bid].next = i + 1;
                    break;
                }
            }
        }
        if(boys[bid].current == -1)
        {
            success = false;
            break;
        }
    }

    return success;
}
#endif
#if 1
bool Gale_Shapley(PARTNER *boys, PARTNER *girls, int count)
{
    int bid = FindFreePartner(boys, count);
    while(bid >= 0)
    {
        int gid = boys[bid].perfect[boys[bid].next];
        if(girls[gid].current == -1)
        {
            boys[bid].current = gid;
            girls[gid].current = bid;
        }
        else
        {
            int bpid = girls[gid].current;
            //Ů��ϲ��bidʤ���䵱ǰ���bpid
            if(GetPerfectPosition(&girls[gid], bpid) > GetPerfectPosition(&girls[gid], bid))
            {
                boys[bpid].current = -1; //��ǰ���ָ�������
                boys[bid].current = gid; //�ύ�����
                girls[gid].current = bid;
            }
        }
        boys[bid].next++; //�����Ƿ���Գɹ�����ͬһ��Ů��ֻ����һ��
        bid = FindFreePartner(boys, count);
    }

    return IsAllPartnerMatch(boys, count);
}
/*
if((GetPerfectPosition(&girls[gid], bpid) == -1) 
    && (GetPerfectPosition(&girls[gid], bid) == -1))
{
    //��ǰ���bpid��bid������Ů����ϲ���б��У�̫�����
    ...
}
else if(GetPerfectPosition(&girls[gid], bpid) == -1)
{
    //��ǰ���bpid����Ů����ϲ���б��У�bid�л���
    ...
}
else if(GetPerfectPosition(&girls[gid], bid) == -1)
{
    //bid����Ů����ϲ���б��У���ǰ���bpidά��ԭ״
    ...
}
else if(GetPerfectPosition(&girls[gid], bpid) > GetPerfectPosition(&girls[gid], bid))
{
    //Ů��ϲ��bidʤ���䵱ǰ���bpid
    ...
}
else
{
    //Ů��ϲ����ǰ���bpidʤ��bid
    ...
}
*/
#endif

void InitPriorityTable()
{
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
    int priority[UNIT_COUNT][UNIT_COUNT];

for(int w = 0; w < UNIT_COUNT; w++)
{
    //��ʼ�������ֵ��ԭ��ͬ��
    for(int j = 0; j < UNIT_COUNT; j++)
    {
        priority[w][j] = 0x7FFFFFFF;
    }
    //��ƫ�����ָ��λ�ù�ϵ
    int pos = 0;
    for(int m = 0; m < girls[w].pCount; m++)
    {
        priority[w][girls[w].perfect[m]] = pos++;
    }
}
//if(priority[gid][bpid] > priority[gid][bid])
//{}
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

int main(int argc, char* argv[])
{
#if 0

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
#if 1
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

    if(Gale_Shapley(boys, girls, UNIT_COUNT))
    {
        PrintResult(boys, girls, UNIT_COUNT);
    }

    return 0;
}
