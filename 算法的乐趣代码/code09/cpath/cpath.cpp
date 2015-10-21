// graph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int MAX_VERTEXNODE = 20;

typedef struct tagEdgeNode
{
    int vertexIndex;    //����յ㶥������
    std::string name;   //��ߵ�����
    int duty;           //��ߵ�ʱ�䣨Ȩ�أ�
}EDGE_NODE;

typedef struct tagVertexNode
{
    int sTime;    //�¼����翪ʼʱ��
    int eTime;    //�¼�����ʼʱ��
    int inCount;  //���ǰ���ڵ����
    std::vector<EDGE_NODE> edges; //���ڱ߱�
}VERTEX_NODE;

typedef struct tagGraph
{
    int count; //ͼ�Ķ���ĸ���
    VERTEX_NODE vertexs[MAX_VERTEXNODE]; //ͼ�Ķ����б�
}GRAPH;


void InitGraph(GRAPH *g, int vertex)
{
    g->count = vertex;
    for(int i = 0; i < vertex; i++)
    {
        g->vertexs[i].sTime = 0;
        g->vertexs[i].eTime = 0x7FFFFFFF;
        g->vertexs[i].inCount = 0;
    }
}

bool AddGraphEdge(GRAPH *g, const char*name, int u, int v, int weight)
{
    if((u >= g->count) || (v >= g->count))
    {
        return false;
    }
    EDGE_NODE edge = {v, name, weight};

    g->vertexs[u].edges.push_back(edge);
    g->vertexs[v].inCount++;

    return true;
}

bool TopologicalSorting(GRAPH *g, std::vector<int>& sortedNode)
{
    std::priority_queue<int> nodeQueue;

    for(int i = 0; i < g->count; i++)
    {
        if(g->vertexs[i].inCount == 0)
        {
            nodeQueue.push(i);
        }
    }

    while(nodeQueue.size() != 0)
    {
        int u = nodeQueue.top();
        nodeQueue.pop();
        sortedNode.push_back(u);
        for(int j = 0; j < (int)g->vertexs[u].edges.size(); j++)
        {
            int v = g->vertexs[u].edges[j].vertexIndex;
            g->vertexs[v].inCount--;
            if(g->vertexs[v].inCount == 0)
            {
                nodeQueue.push(v);
            }
        }
    }

    return (sortedNode.size() == g->count);
}

void CalcESTime(GRAPH *g, const std::vector<int>& sortedNode)
{
    g->vertexs[0].sTime = 0; //est[0] = 0

    std::vector<int>::const_iterator nit = sortedNode.begin();
    for(; nit != sortedNode.end(); ++nit)
    {
        int u = *nit;
        //����u���������������
        std::vector<EDGE_NODE>::iterator eit = g->vertexs[u].edges.begin();
        for(; eit != g->vertexs[u].edges.end(); ++eit)
        {
            int v = eit->vertexIndex;
            int uvst = g->vertexs[u].sTime + eit->duty;
            if(uvst > g->vertexs[v].sTime)
            {
                g->vertexs[v].sTime = uvst;
            }
        }
    }
}

void CalcLSTime(GRAPH *g, const std::vector<int>& sortedNode)
{
    //���һ���ڵ������ʼʱ��������翪ʼʱ��
    g->vertexs[g->count - 1].eTime = g->vertexs[g->count - 1].sTime;

    std::vector<int>::const_reverse_iterator cit = sortedNode.rbegin();
    for(; cit != sortedNode.rend(); ++cit)
    {
        int u = *cit;
        //����u���������������
        std::vector<EDGE_NODE>::iterator eit = g->vertexs[u].edges.begin();
        for(; eit != g->vertexs[u].edges.end(); ++eit)
        {
            int v = eit->vertexIndex;
            int uvet = g->vertexs[v].eTime - eit->duty;
            if(uvet < g->vertexs[u].eTime)
            {
                g->vertexs[u].eTime = uvet;
            }
        }
    }
}

void PrintSorting(GRAPH *g, const std::vector<int>& sortedNode)
{
    std::vector<int>::const_iterator cit = sortedNode.begin();
    for(;cit != sortedNode.end(); ++cit)
    {
        std::cout << "e" << *cit << std::endl;
    }
}

bool CriticalPath(GRAPH *g)
{
    std::vector<int> sortedNode;
    if(!TopologicalSorting(g, sortedNode)) //����1
    {
        return false;
    }
    CalcESTime(g, sortedNode); //����2
    CalcLSTime(g, sortedNode); //����4

    //����4������ؼ�·���ϵĻ����
    //for(int u = 0; u < g->count; u++)
    //{

    std::vector<int>::iterator nit = sortedNode.begin();
    for(; nit != sortedNode.end(); ++nit)
    {
        int u = *nit;
        std::vector<EDGE_NODE>::iterator eit = g->vertexs[u].edges.begin();
        for(; eit != g->vertexs[u].edges.end(); ++eit)
        {
            int v = eit->vertexIndex;
            //�Ƿ��ǹؼ����
            if(g->vertexs[u].sTime == g->vertexs[v].eTime - eit->duty)
            {
                if(!eit->name.empty())//���������¼������������
                {
                    std::cout << eit->name << std::endl;
                }
            }
        }
    }

    return true;
}

int main(int argc, char* argv[])
{

    GRAPH graph;
    InitGraph(&graph, 10);
    AddGraphEdge(&graph, "P1", 0, 1, 8);
    AddGraphEdge(&graph, "P2", 0, 2, 5);
    AddGraphEdge(&graph, "", 1, 3, 0);
    AddGraphEdge(&graph, "", 2, 3, 0);
    AddGraphEdge(&graph, "P7", 1, 6, 4);
    AddGraphEdge(&graph, "P5", 2, 5, 7);
    AddGraphEdge(&graph, "P3", 3, 4, 6);
    AddGraphEdge(&graph, "P4", 4, 8, 4);
    AddGraphEdge(&graph, "P8", 6, 7, 3);
    AddGraphEdge(&graph, "", 8, 7, 0);
    AddGraphEdge(&graph, "", 8, 5, 0);
    AddGraphEdge(&graph, "P9", 7, 9, 4);
    AddGraphEdge(&graph, "P6", 5, 9, 7);

    CriticalPath(&graph);
	return 0;
}

