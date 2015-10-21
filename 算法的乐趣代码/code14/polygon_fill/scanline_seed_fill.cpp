#include "stdafx.h"
#include <stack>
#include "dev_adp.h"
#include "common.h"
#include "point.h"
#include "scanline_seed_fill.h"

int FillLineRight(int x, int y, int new_color, int boundary_color)
{
    int xt = x;
    while(GetPixelColor(xt, y) != boundary_color)
    {
        SetPixelColor(xt, y, new_color);
        xt++;
    }

    return xt - x;
}

int FillLineLeft(int x, int y, int new_color, int boundary_color)
{
    int xt = x;
    while(GetPixelColor(xt, y) != boundary_color)
    {
        SetPixelColor(xt, y, new_color);
        xt--;
    }

    return x - xt;
}

/*�㲻�Ǳ߽磬����û������*/
bool IsPixelValid(int x, int y, int new_color, int boundary_color)
{
    return ( (GetPixelColor(x, y) != boundary_color) 
              && (GetPixelColor(x, y) != new_color) );
}

int SkipInvalidInLine(int x, int y, int xRight, int new_color, int boundary_color)
{
    int xt = x;
    while(!IsPixelValid(xt, y, new_color, boundary_color) && (xt < xRight))
    {
        xt++;
    }

    return xt - x;
}

void SearchLineNewSeed(std::stack<Point>& stk, int xLeft, int xRight, 
                       int y, int new_color, int boundary_color)
{
    int xt = xLeft;
    bool findNewSeed = false;

    while(xt <= xRight)
    {
        findNewSeed = false;
        while(IsPixelValid(xt, y, new_color, boundary_color) && (xt < xRight))
        {
            findNewSeed = true;
            xt++;
        }
        if(findNewSeed)
        {
            if(IsPixelValid(xt, y, new_color, boundary_color) && (xt == xRight))
                stk.push(Point(xt, y));
            else
                stk.push(Point(xt - 1, y));
        }

        /*���������ڲ�����Ч�㣨���������Ҷ����ϰ���������*/
        int xspan = SkipInvalidInLine(xt, y, xRight, new_color, boundary_color);
        xt += (xspan == 0) ? 1 : xspan;
        /*�����������,���˳�while(x<=xright)ѭ��*/
    }
}

void ScanLineSeedFill(int x, int y, int new_color, int boundary_color)
{
    std::stack<Point> stk;

    stk.push(Point(x, y)); //��1�������ӵ���վ
    while(!stk.empty()) 
    {
        Point seed = stk.top(); //��2����ȡ��ǰ���ӵ�
        stk.pop();

        //��3�������������
        int count = FillLineRight(seed.x, seed.y, new_color, boundary_color);//�������
        int xRight = seed.x + count - 1;
        count = FillLineLeft(seed.x - 1, seed.y, new_color, boundary_color);//�������
        int xLeft = seed.x - count;

        //��4����������������ɨ����
        SearchLineNewSeed(stk, xLeft, xRight, seed.y - 1, new_color, boundary_color); 
        SearchLineNewSeed(stk, xLeft, xRight, seed.y + 1, new_color, boundary_color); 
    }
}
