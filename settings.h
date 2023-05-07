//
// Created by yanglq on 2022/4/14.
//

#ifndef METAMWVC_1_SETTING_H
#define METAMWVC_1_SETTING_H
extern void set_default_setting();
extern void set_DynWVC2_setting();
extern void set_numwvc_setting();
extern void set_fastwvc_setting();
extern void (*init_sol_heuristic_ptr) ();
extern int (*del_1_strategy_heuristic_ptr) (int *,int ,int);
extern int (*del_1_bms_ptr)();
extern void (*del_2_mode_heuristic_ptr)();
extern int (*del_2_bms_ptr)();
extern int (*del_2_strategy_heuristic_ptr)(int *,int,int);
extern int (*del_3_bms_ptr)();
extern int (*del_3_strategy_heuristic_ptr)(int *,int,int);
extern int (*add_strategy_heuristic_ptr)();
extern void (*add_conf_change)(int);
extern void (*remove_conf_change)(int);
#endif //METAMWVC_1_SETTING_H
