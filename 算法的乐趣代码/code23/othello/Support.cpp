#include "StdAfx.h"
#include "Support.h"


int line_idx_tbl[LINE_DIRECTION][LINE_CELLS] = 
{
    {0, 1, 2}, //��һ��
    {3, 4, 5}, //�ڶ���
    {6, 7, 8}, //������
    {0, 3, 6}, //��һ��
    {1, 4, 7}, //�ڶ���
    {2, 5, 8}, //������
    {0, 4, 8}, //��������
    {2, 4, 6}, //��������
};