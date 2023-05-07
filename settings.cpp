//
// Created by yanglq on 2022/4/14.
//
#include "basic.h"
#include "settings.h"
#include "heuristic.h"


void set_default_setting(){
    para_bms_k = 50;
}
void set_DynWVC2_setting(){
    del_1_bms_ptr = select_vertex_not_use_bms_1;
    del_1_strategy_heuristic_ptr = select_high_dscore_breaking_ties_first;
    del_2_mode_heuristic_ptr = del_vertex_dynwvc2;
    del_2_bms_ptr = select_vertex_not_use_bms_2;
    del_2_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_age;
    del_3_bms_ptr = select_vertex_use_bms_3;
    del_3_strategy_heuristic_ptr = select_high_dscore_breaking_ties_age;
    add_strategy_heuristic_ptr = select_add_high_score_breaking_ties_age ;
    add_conf_change = add_conf_change1;
    remove_conf_change = remove_conf_change1;
    use_tabu = false;
    CC_strategy = true;
    forget_edge_weight = true;
}

void set_numwvc_setting(){
    del_1_bms_ptr = select_vertex_not_use_bms_1;
    del_1_strategy_heuristic_ptr = select_high_score_breaking_ties_age;
    del_2_mode_heuristic_ptr = del_vertex_numwvc;
    del_2_bms_ptr = select_vertex_use_bms_2;
    del_2_strategy_heuristic_ptr = select_high_score_breaking_ties_age;
    del_3_bms_ptr = select_vertex_not_use_bms_3;
    del_3_strategy_heuristic_ptr = select_high_score_breaking_ties_age;
    add_strategy_heuristic_ptr = select_add_from_edge_high_score_breaking_ties_age;
    add_conf_change = add_conf_change2;
    remove_conf_change = remove_conf_change2;
    use_tabu = false;
    CC_strategy = true;
    forget_edge_weight = false;
}

void set_fastwvc_setting(){
    del_1_bms_ptr = select_vertex_not_use_bms_1;
    del_1_strategy_heuristic_ptr = select_high_dscore_breaking_ties_first;
    del_2_mode_heuristic_ptr = del_vertex_fastwvc;
    del_2_bms_ptr = select_vertex_use_bms_2;
    del_2_strategy_heuristic_ptr = select_high_dscore_breaking_ties_age;
    del_3_bms_ptr = select_vertex_use_bms_3;
    del_3_strategy_heuristic_ptr = select_high_dscore_breaking_ties_age;
    add_strategy_heuristic_ptr = select_add_high_score_breaking_ties_age ;
    add_conf_change = add_conf_change1;
    remove_conf_change = remove_conf_change1;
    use_tabu = false;
    CC_strategy = true;
    forget_edge_weight = true;
}

void (*init_sol_heuristic_ptr) ();
int (*del_1_strategy_heuristic_ptr) (int *,int,int);
int (*del_1_bms_ptr)();
void (*del_2_mode_heuristic_ptr)();
int (*del_2_bms_ptr)();
int (*del_2_strategy_heuristic_ptr)(int *,int,int);
int (*del_3_bms_ptr)();
int (*del_3_strategy_heuristic_ptr)(int *,int,int);
int (*add_strategy_heuristic_ptr)();
void (*add_conf_change)(int);
void (*remove_conf_change)(int);