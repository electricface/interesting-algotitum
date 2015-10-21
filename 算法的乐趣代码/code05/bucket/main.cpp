// bucket.cpp 
//

#include "stdafx.h"
#include "bucket_state.h"


bool IsSameBucketState(BucketState state1, BucketState state2)
{
    return state1.IsSameState(state2);
}

bool IsProcessedState(deque<BucketState>& states, const BucketState& newState)
{
    deque<BucketState>::iterator it = states.end();
   
    it = find_if( states.begin(), states.end(), 
                  bind2nd(ptr_fun(IsSameBucketState), newState) );

    //it = find_if(states.begin(), states.end(), bind1st(mem_fun_ref(&BucketState::IsSameState), newState));

    return (it != states.end());
}

void PrintResult(deque<BucketState>& states)
{
    cout << "Find Result : " << endl;
    for_each(states.begin(), states.end(), 
             mem_fun_ref(&BucketState::PrintStates));
    cout << endl << endl;
}

bool IsCurrentActionValid(BucketState& current, int from, int to)
{
    /*����ͬһ��Ͱ����fromͰ����ˮ����toͰ�в���*/
    if( (from != to) 
        && !current.IsBucketEmpty(from) 
        && !current.IsBucketFull(to) )
    {
        return true;
    }

    return false;
}

void SearchState(deque<BucketState>& states);

void SearchStateOnAction(deque<BucketState>& states, BucketState& current, int from, int to)
{
    if(IsCurrentActionValid(current, from, to))
    {
        BucketState next;
         /*��from��to��ˮ������ɹ������ص�ˮ���״̬*/
        bool bDump = current.DumpWater(from, to, next);
        if(bDump && !IsProcessedState(states, next))
        {
            states.push_back(next);
            SearchState(states);
            states.pop_back();
        }
    }
}

void SearchState(deque<BucketState>& states)
{
    BucketState current = states.back(); /*ÿ�ζ��ӵ�ǰ״̬��ʼ*/
    if(current.IsFinalState())
    {
        PrintResult(states);
        return;
    }

    /*ʹ������ѭ���������6�ֵ�ˮ״̬*/
    for(int j = 0; j < buckets_count; ++j)
    {
        for(int i = 0; i < buckets_count; ++i)
        {
            SearchStateOnAction(states, current, i, j);
        }
    }
}


int main(int argc, char* argv[])
{
    deque<BucketState> states;
    BucketState init;

    states.push_back(init);
    SearchState(states);

    assert(states.size() == 1); /*��ٽ�����statesӦ�û���ֻ��һ����ʼ״̬*/

    return 0;
}
