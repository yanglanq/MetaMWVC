//
// Created by 杨岚清 on 2022/4/14.
//

#ifndef METAMWVC_1_HEURISTIC_H
#define METAMWVC_1_HEURISTIC_H
void init_sol_k_best();
void init_best();

// 删点方法
int select_high_dscore_breaking_ties_age(int* cand_set,int set_length,int allowtabu);
int select_high_dscore_breaking_ties_first(int* cand_set,int set_length,int allowtabu);
int select_high_dscore_breaking_ties_random(int* cand_set,int set_length,int allowtabu);
int select_vertex_valid_score_breaking_ties_age(int* cand_set,int set_length,int allowtabu);
int select_vertex_valid_score_breaking_ties_first(int* cand_set,int set_length,int allowtabu);
int select_vertex_valid_score_breaking_ties_random(int* cand_set,int set_length,int allowtabu);
int select_high_score_breaking_ties_age(int* cand_set,int set_length,int allowtabu);
int select_high_score_breaking_ties_first(int* cand_set,int set_length,int allowtabu);
int select_high_score_breaking_ties_random(int* cand_set,int set_length,int allowtabu);

int select_vertex_use_bms_1();
int select_vertex_not_use_bms_1();

void del_vertex_dynwvc2();
void del_vertex_numwvc();
void del_vertex_fastwvc();

int select_vertex_use_bms_2();
int select_vertex_not_use_bms_2();

int select_vertex_use_bms_3();
int select_vertex_not_use_bms_3();

// 加点方法
int select_add_high_score_breaking_ties_age();
int select_add_high_score_breaking_ties_first();
int select_add_high_score_breaking_ties_random();
int select_add_from_edge_high_score_breaking_ties_age();
int select_add_from_edge_high_score_breaking_ties_first();
int select_add_from_edge_high_score_breaking_ties_random();


#endif //METAMWVC_1_HEURISTIC_H
