//
// Created by yanglq on 2022/4/12.
//

#ifndef METAMWVC_1_BASIC_H
#define METAMWVC_1_BASIC_H
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>
#include <cstring>
#include <set>
#include <climits>

using namespace std;

#define pop(stack) stack[--stack ## _fill_pointer]
#define push(item, stack) stack[stack ## _fill_pointer++] = item

typedef long long llong;
typedef unsigned int uint;
struct Edge
{
    int v1;
    int v2;
};

extern string inst;

extern chrono::steady_clock::time_point start;
extern llong   max_steps;
extern llong   step;
extern int     try_step;
extern uint    seed;
extern int     cutoff_time;
extern int     mode;

extern int     v_num;
extern int     e_num;

extern Edge    *edge;
extern int     *edge_weight;

extern int     *dscore;
extern llong   *time_stamp;
extern int     *valid_score;

extern int     *v_weight;
extern int     **v_edges;
extern int     **v_adj;
extern int     *v_degree;

extern int     c_size;
extern int     *v_in_c;
extern int     *must_in_c;
extern int     must_in_c_size;
extern int     *remove_cand;
extern int     *index_in_remove_cand;
extern int     remove_cand_size;
extern llong   now_weight;

extern int     best_c_size;
extern int     *best_v_in_c;
extern double  best_comp_time;
extern llong   best_step;
extern llong   best_weight;

extern int     *uncov_stack;
extern int     uncov_stack_fill_pointer;
extern int     *index_in_uncov_stack;

extern int     ave_weight;
extern int     delta_total_weight;
extern int     threshold;
extern double  p_scale;
extern int     para_bms_k;
extern int     bms_cand_set_size;
extern int     *bms_cand_set;
extern int     noimprovement;
extern int     dyn_count;
extern int     *remove_list;
extern int     remove_length;
extern bool    forget_edge_weight;


extern int     NumRemove;
extern int     NoImpro;

extern int     CC_strategy;
extern int     use_tabu;
extern int     use_hash;
extern int     *conf_change;
extern set<int> tabu_list;

extern int     L;
extern int     current_hashk1;
extern int     current_hashk2;
extern int     current_hashk3;
extern int     *wk1;
extern int     *wk2;
extern int     *wk3;
extern set<int> hashk1;
extern set<int> hashk2;
extern set<int> hashk3;
extern set<int> tabu_list_hash;

extern int      *temp_list;
extern int      temp_list_len;

double TimeElapsed();
bool build_instance();
void init_sol();
void ResetRemoveCand();
void UpdateBestSolution();
void Remove(int v);
void Add(int v);
void UpdateEdgeWeight();
int CheckSolution();
inline void Uncover(int e);
inline void cover(int e);
int in_tabu(int i);
void record_hash();
int check_solution_in_hash(int v);
int in_tabu_hash(int i);
void free_memory();
void add_conf_change0(int v);
void remove_conf_change0(int v);
void add_conf_change1(int v);
void remove_conf_change1(int v);
void add_conf_change2(int v);
void remove_conf_change2(int v);


#endif //METAMWVC_1_BASIC_H
