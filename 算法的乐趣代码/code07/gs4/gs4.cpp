// gs4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <cstdio>
//#include <cstring>
#include <algorithm>

//using namespace std;

const int UNIT_COUNT = 5;
const int INFINITE = 100000000; // ��������

typedef struct tagPartner
{
    char *name;   //����
    int weight[UNIT_COUNT]; 
}PARTNER;

typedef struct tagKmMatch
{
    int edge[UNIT_COUNT][UNIT_COUNT]; //Xi��Yj��Ӧ�ıߵ�Ȩ��
    bool sub_map[UNIT_COUNT][UNIT_COUNT];// ����ͼ�������ͼ, sub_map[i][j] = 1 ����Xi��Yj�б�
    bool x_on_path[UNIT_COUNT]; // �����һ��Ѱ������·���Ĺ����У�Xi�Ƿ�������·����
    bool y_on_path[UNIT_COUNT]; // �����һ��Ѱ������·���Ĺ����У�Yi�Ƿ�������·����
    int path[UNIT_COUNT]; // ƥ����Ϣ������iΪY�еĶ����ţ�path[i]ΪX�ж�����
}KM_MATCH;

// �������ͼ��Ѱ��XiΪ��������·��
bool FindAugmentPath(KM_MATCH *km, int xi)
{
   for(int yj = 0; yj < UNIT_COUNT; yj++) 
   {
       if(!km->y_on_path[yj] && km->sub_map[xi][yj]) 
      {
         km->y_on_path[yj] = true;
         int xt = km->path[yj];
         km->path[yj] = xi;
         if(xt == -1 || FindAugmentPath(km, xt)) 
         {
            return true;
         }
         km->path[yj] = xt;
         if(xt != -1) 
             km->x_on_path[xt] = true;
      }
   }

   return false;
}

void ResetMatchPath(KM_MATCH *km)
{
    for(int i = 0; i < UNIT_COUNT; i++)
    {
        km->path[i] = -1;
    }
}

void ClearOnPathSign(KM_MATCH *km)
{
    for(int i = 0; i < UNIT_COUNT; i++)
    {
        km->x_on_path[i] = false;
        km->y_on_path[i] = false;
    }
}

bool Kuhn_Munkres_Match(KM_MATCH *km) 
{
    int i, j;
    int A[UNIT_COUNT], B[UNIT_COUNT];
    // ��ʼ��Xi��Yi�Ķ���
    for(i = 0; i < UNIT_COUNT; i++) 
    {
        B[i] = 0;
        A[i] = -INFINITE;
        for(j = 0; j < UNIT_COUNT; j++) 
        {
            A[i] = std::max(A[i], km->edge[i][j]);
        }
    }
    while(true) 
    {
        // ��ʼ����Ȩ����ͼ�������ͼ
        for(i = 0; i < UNIT_COUNT; i++)
        {
            for(j = 0; j < UNIT_COUNT; j++) 
            {
                km->sub_map[i][j] = ((A[i]+B[j]) == km->edge[i][j]);
            }
        }
        //ʹ���������㷨Ѱ�������ͼ���걸ƥ��
        int match = 0;
        ResetMatchPath(km);
        for(int xi = 0; xi < UNIT_COUNT; xi++) 
        {
            ClearOnPathSign(km);
            if(FindAugmentPath(km, xi)) 
                match++;
            else 
            {
                km->x_on_path[xi] = true;
                break;
            }
        }
        //����ҵ��걸ƥ��ͷ��ؽ��
        if(match == UNIT_COUNT)
        {
            return true;
        }
        //�������꣬�����㷨
        int dx = INFINITE;
        for(i = 0; i < UNIT_COUNT; i++) 
        {
            if(km->x_on_path[i])
            {
                for(j = 0; j < UNIT_COUNT; j++) 
                {
                    if(!km->y_on_path[j]) 
                        dx = std::min(dx, A[i] + B[j] - km->edge[i][j]);
                }
            }
        }
        for(i = 0; i < UNIT_COUNT; i++) 
        {
            if(km->x_on_path[i]) 
                A[i] -= dx;
            if(km->y_on_path[i]) 
                B[i] += dx;
        }
    }

    return false;
}
 

/*
    Y1   Y2   Y3   Y4   Y5
X1  3    5    5    4    1
X2  2    2    0    2    2
X3  2    4    4    1    0
X4  0    1    1    0    0
X5  1    2    1    3    3
*/

PARTNER X[] = 
{
    {"X1", {3,5,5,4,1} },
    {"X2", {2,2,0,2,2} },
    {"X3", {2,4,4,1,0} },
    {"X4", {0,1,1,0,0} },
    {"X5", {1,2,1,3,3} }
};

void InitGraph(KM_MATCH *km, PARTNER *X)
{
    memset(km->edge, 0, UNIT_COUNT * UNIT_COUNT * sizeof(int));
    for(int i = 0; i < UNIT_COUNT; i++)
    {
        km->path[i] = -1;
        for(int j = 0; j < UNIT_COUNT; j++)
            km->edge[i][j] = X[i].weight[j];
    }
}

void PrintResult(KM_MATCH *km)
{
    int cost = 0;
    for(int i = 0; i < UNIT_COUNT; i++) 
    {
        cost += km->edge[km->path[i]][i];
        std::cout << "x" << km->path[i] << "<--->" << "y" << i << std::endl;
    }

    std::cout << "total cost : " << cost << std::endl;
}

int main(int argc, char* argv[])
{
    KM_MATCH km_match;
    InitGraph(&km_match, X);
    if(Kuhn_Munkres_Match(&km_match))
    {
        PrintResult(&km_match);
    }
 
    return 0;
}

