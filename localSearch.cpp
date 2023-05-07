//
// Created by yanglq on 2022/4/20.
//
#include "basic.h"
#include "settings.h"
void cover_LS(){

    long temp_weight;
    int remove_v,add_v;
    ave_weight = 1;
    delta_total_weight = 0;
    p_scale = 0.3;
    threshold = (int)(0.5 * v_num);
    step = 1;

    NumRemove = 3;
    NoImpro = 0;


    start = chrono::steady_clock::now();
    noimprovement=0;
    dyn_count=0;

    while(true)
    {

        remove_length = 0;
        temp_weight=now_weight;
        while(uncov_stack_fill_pointer==0){
            UpdateBestSolution();
            remove_v = del_1_bms_ptr();
            Remove(remove_v);
        }
        time_stamp[remove_v] = step;
        remove_list[remove_length] = remove_v;
        remove_length++;
        del_2_mode_heuristic_ptr();


        if (TimeElapsed() >= cutoff_time)
        {
            return;
        }
        tabu_list.clear();
        while (uncov_stack_fill_pointer > 0)
        {
            add_v = add_strategy_heuristic_ptr();
            Add(add_v);
            if(use_tabu) tabu_list.insert(add_v);
            UpdateEdgeWeight();
            time_stamp[add_v] = step;
        }

        tabu_list_hash.clear();
        if (now_weight >= temp_weight)
        {
            noimprovement++;
        }
        step++;
        if(TimeElapsed()>=cutoff_time)break;



    }
}

