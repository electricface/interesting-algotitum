// bucket.cpp 
//

#include "stdafx.h"
#include "bucket_state.h"


bool IsSameBucketState(BucketState state1, BucketState state2)
{
    return state1.IsSameState(state2);
}

bool IsProcessedState(std::deque<BucketState>& states, const BucketState& newState)
{
    std::deque<BucketState>::iterator it = states.end();
   
    it = find_if( states.begin(), states.end(), 
                  std::bind2nd(std::ptr_fun(IsSameBucketState), newState) );

    //it = find_if(states.begin(), states.end(), std::bind1st(std::mem_fun_ref(&BucketState::IsSameState), newState));

    return (it != states.end());
}

void PrintResult(std::deque<BucketState>& states)
{
    std::cout << "Find Result : " << std::endl;
    for_each(states.begin(), states.end(), 
             std::mem_fun_ref(&BucketState::PrintStates));
    std::cout << std::endl << std::endl;
}

void SearchState(std::deque<BucketState>& states);

void SearchStateOnAction(std::deque<BucketState>& states, BucketState& current, int from, int to)
{
    if(current.CanTakeDumpAction(from, to))
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

void SearchState(std::deque<BucketState>& states)
{
    BucketState current = states.back(); /*ÿ�ζ��ӵ�ǰ״̬��ʼ*/
    if(current.IsFinalState())
    {
        PrintResult(states);
        return;
    }

    /*ʹ������ѭ���������6�ֵ�ˮ״̬*/
    for(int j = 0; j < BUCKETS_COUNT; ++j)
    {
        for(int i = 0; i < BUCKETS_COUNT; ++i)
        {
            SearchStateOnAction(states, current, i, j);
        }
    }
}


int main(int argc, char* argv[])
{
    std::deque<BucketState> states;
    BucketState init;

    states.push_back(init);
    SearchState(states);

    assert(states.size() == 1); /*��ٽ�����statesӦ�û���ֻ��һ����ʼ״̬*/

    return 0;
}
