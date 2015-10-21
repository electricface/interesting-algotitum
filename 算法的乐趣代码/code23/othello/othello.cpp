// othello.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameControl.h"
#include "GameState.h"
#include "NegamaxAlphaBetaSearcher.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include "WzEvaluator.h"
#include "OdEvaluator.h"

const int SEARCH_DEPTH = 6;

int testState[GAME_CELLS] = 
{
    0,0,2,1,1,1,0,0,
    2,0,0,2,1,1,0,0,
    2,2,2,1,2,1,1,1,
    2,2,2,1,2,2,1,1,
    2,2,2,1,2,2,2,0,
    2,2,2,1,2,0,0,0,
    2,2,2,1,2,2,0,0,
    2,0,1,1,1,0,0,0
};

int main(int argc, char* argv[])
{
    NegamaxAlphaBetaSearcher nabs;

    HumanPlayer human("����");
    //ComputerPlayer computer1("KA47");
    //computer1.SetSearcher(&nabs, 3);
    ComputerPlayer computer("ThinkPad X200");
    computer.SetSearcher(&nabs, SEARCH_DEPTH); 

    WzEvaluator wzEv;
    OdEvaluator odEv;
    GameState init_state;
    init_state.InitGameState(PLAYER_A);
    //init_state.SetGameState(testState, PLAYER_A);
    init_state.SetEvaluator(&wzEv);

    GameControl gc;
    gc.SetPlayer(&computer, PLAYER_A);
    gc.SetPlayer(&human, PLAYER_B);
    //gc.SetPlayer(&computer1, PLAYER_B);
    gc.InitGameState(init_state);
    gc.Run();

    return 0;
}
/*
int MiniMax(node, depth, isMaxPlayer)
{
    if(depth == 0)
    {
        return Evaluate(node);
    }

    int score = isMaxPlayer ? -INFINITY : INFINITY;
    for_each(node���ӽڵ�child_node)
    {
        int value = MiniMax(child_node, depth - 1, !isMaxPlayer);
        if(isMaxPlayer)
            score = max(score, value);
        else
            score = min(score, value);
    }
}
*/
/*
int MiniMax_AlphaBeta(node, depth, ��, ��, isMaxPlayer)
{
    if(depth == 0)
    {
        return Evaluate(node);
    }

    if(isMaxPlayer)
    {
        for_each(node���ӽڵ�child_node)
        {
            int value = MiniMax_AlphaBeta(child_node, depth - 1, ��, ��, FALSE);
            �� = max(��, value);
            if(�� <= ��) //�� ��֦
                break;
        }

        return ��;
    }
    else
    {
        for_each(node���ӽڵ�child_node)
        {
            int value = MiniMax_AlphaBeta(child_node, depth - 1, ��, ��, TRUE);
            �� = min(��, value);
            if(�� <= ��) //�� ��֦
                break;
        }

        return ��;
    }
}
*/
/*
int NegaMax(node, depth, color)
{
    if(depth == 0)
    {
        return color * Evaluate(node);
    }

    int score = -INFINITY;
    for_each(node���ӽڵ�child_node)
    {
        int value = -NegaMax(child_node, depth - 1, -color);
        score = max(score, value);
    }
}
*/
/*
int NegaMax_AlphaBeta(node, depth, ��, ��, color)
{
    if(depth == 0)
    {
        return color * Evaluate(node);
    }

    int score = -INFINITY;
    for_each(node���ӽڵ�child_node)
    {
        int value = -NegaMax_AlphaBeta(child_node, depth - 1, -��, -��, -color);
        score = max(score, value);
        �� = max(��, value);
        if(�� >= ��)
            break;
    }

    return score;
}
*/