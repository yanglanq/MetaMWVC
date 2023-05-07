//
// Created by yanglq on 2022/4/12.
//
#include "parse_arguement.h"
#include "basic.h"
#include "settings.h"
#include "heuristic.h"
bool parse_arguments(int argc, char **argv) {
    bool flag_inst = false;
    bool flag_seed = false;
    bool flag_time = false;

    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-inst") == 0) {
            i++;
            if (i >= argc) return false;
            inst = argv[i];
            flag_inst = true;
            continue;
        }
        else if (strcmp(argv[i], "-cutoff_time") == 0) {
            i++;
            if (i >= argc) return false;
            sscanf(argv[i], "%d", &cutoff_time);
            flag_time = true;
            continue;
        }
        else if (strcmp(argv[i], "-seed") == 0) {
            i++;
            if (i >= argc) return false;
            sscanf(argv[i], "%d", &seed);
            flag_seed = true;
            continue;
        }
        else if (strcmp(argv[i], "-init_sol") == 0)
        {
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                init_sol_heuristic_ptr = init_sol_k_best;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {
                init_sol_heuristic_ptr = init_best;
                continue;
            }
            else {
                cout << "init_sol" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-del_1_strategy") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                del_1_strategy_heuristic_ptr = select_high_dscore_breaking_ties_age;
                continue;
            }
            else if(strcmp(argv[i], "1") == 0){
                del_1_strategy_heuristic_ptr = select_high_dscore_breaking_ties_random;
            }
            else if (strcmp(argv[i], "2") == 0) {
                del_1_strategy_heuristic_ptr = select_high_dscore_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "3") == 0) {
                del_1_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_age;
                continue;
            }
            else if(strcmp(argv[i], "4") == 0){
                del_1_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_random;
                continue;
            }
            else if(strcmp(argv[i], "5") == 0) {
                del_1_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "6") == 0) {
                del_1_strategy_heuristic_ptr = select_high_score_breaking_ties_age;
                continue;
            }
            else if (strcmp(argv[i], "7") == 0) {
                del_1_strategy_heuristic_ptr = select_high_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "8") == 0) {
                del_1_strategy_heuristic_ptr = select_high_score_breaking_ties_random;
                continue;
            }
            else {
                cout << "del_1_strategy" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-perform_bms_1") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {//
                del_1_bms_ptr = select_vertex_not_use_bms_1;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {//
                del_1_bms_ptr = select_vertex_use_bms_1;
                continue;
            }
            else {
                cout << "perform_bms_1" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-del_2_mode") == 0){
            i++;
            if (i >= argc) return false;
             if (strcmp(argv[i], "1") == 0) {
                del_2_mode_heuristic_ptr = del_vertex_dynwvc2;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0) {
                del_2_mode_heuristic_ptr = del_vertex_numwvc;
                CC_strategy = true;
                continue;
            }
             else if (strcmp(argv[i], "3") == 0) {
                 del_2_mode_heuristic_ptr = del_vertex_fastwvc;
                 CC_strategy = true;
                 continue;
             }
            else {
                cout << "del_2_mode" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-perform_bms_2") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {//
                del_2_bms_ptr = select_vertex_not_use_bms_2;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {//
                del_2_bms_ptr = select_vertex_use_bms_2;
                continue;
            }
            else {
                cout << "perform_bms_1" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-del_2_strategy") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                del_2_strategy_heuristic_ptr = select_high_dscore_breaking_ties_age;
                continue;
            }
            else if(strcmp(argv[i], "1") == 0){
                del_2_strategy_heuristic_ptr = select_high_dscore_breaking_ties_random;
            }
            else if (strcmp(argv[i], "2") == 0) {
                del_2_strategy_heuristic_ptr = select_high_dscore_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "3") == 0) {
                del_2_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_age;
                continue;
            }
            else if(strcmp(argv[i], "4") == 0){
                del_2_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_random;
                continue;
            }
            else if(strcmp(argv[i], "5") == 0) {
                del_2_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "6") == 0) {
                del_2_strategy_heuristic_ptr = select_high_score_breaking_ties_age;
                continue;
            }
            else if (strcmp(argv[i], "7") == 0) {
                del_2_strategy_heuristic_ptr = select_high_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "8") == 0) {
                del_2_strategy_heuristic_ptr = select_high_score_breaking_ties_random;
                continue;
            }
            else {
                cout << "del_2_strategy" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-perform_bms_3") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {//
                del_3_bms_ptr = select_vertex_not_use_bms_3;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {//
                del_3_bms_ptr = select_vertex_use_bms_3;
                continue;
            }
            else {
                cout << "perform_bms_1" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-del_3_strategy") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                del_3_strategy_heuristic_ptr = select_high_dscore_breaking_ties_age;
                continue;
            }
            else if(strcmp(argv[i], "1") == 0){
                del_3_strategy_heuristic_ptr = select_high_dscore_breaking_ties_random;
            }
            else if (strcmp(argv[i], "2") == 0) {
                del_3_strategy_heuristic_ptr = select_high_dscore_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "3") == 0) {
                del_3_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_age;
                continue;
            }
            else if(strcmp(argv[i], "4") == 0){
                del_3_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_random;
                continue;
            }
            else if(strcmp(argv[i], "5") == 0) {
                del_3_strategy_heuristic_ptr = select_vertex_valid_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "6") == 0) {
                del_3_strategy_heuristic_ptr = select_high_score_breaking_ties_age;
                continue;
            }
            else if (strcmp(argv[i], "7") == 0) {
                del_3_strategy_heuristic_ptr = select_high_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "8") == 0) {
                del_3_strategy_heuristic_ptr = select_high_score_breaking_ties_random;
                continue;
            }
            else {
                cout << "del_3_strategy" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-add_strategy") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                add_strategy_heuristic_ptr = select_add_high_score_breaking_ties_age;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {
                add_strategy_heuristic_ptr = select_add_from_edge_high_score_breaking_ties_age;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0) {
                add_strategy_heuristic_ptr = select_add_high_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "3") == 0) {
                add_strategy_heuristic_ptr = select_add_from_edge_high_score_breaking_ties_first;
                continue;
            }
            else if (strcmp(argv[i], "4") == 0) {
                add_strategy_heuristic_ptr = select_add_high_score_breaking_ties_random;
                continue;
            }
            else if (strcmp(argv[i], "5") == 0) {
                add_strategy_heuristic_ptr = select_add_from_edge_high_score_breaking_ties_random;
                continue;
            }
            else {
                cout << "add_strategy" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-perform_forget_edge_weight") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                forget_edge_weight = false;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {
                forget_edge_weight = true;
                continue;
            }
            else {
                cout << "perform_forget_edge_weight" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-use_CC_strategy") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                CC_strategy = false;
                add_conf_change = add_conf_change0;
                remove_conf_change = remove_conf_change0;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {
                CC_strategy = true;
                add_conf_change = add_conf_change1;
                remove_conf_change = remove_conf_change1;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0) {
                CC_strategy = true;
                add_conf_change = add_conf_change2;
                remove_conf_change = remove_conf_change2;
                continue;
            }
            else {
                cout << "perform_forget_edge_weight" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "-use_tabu") == 0){
            i++;
            if (i >= argc) return false;
            if (strcmp(argv[i], "0") == 0) {
                use_tabu = false;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0) {
                use_tabu = true;
                continue;
            }
            else {
                cout << "perform_forget_edge_weight" << endl;
                return false;
            }
        }
    }
    if(flag_inst&&flag_seed&&flag_time)return true;
    return false;
}
