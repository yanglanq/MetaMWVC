//
// Created by yanglq on 2022/4/14.
//
#include "basic.h"
#include "settings.h"

using namespace std;
inline int compare(int s1, int c1, int s2, int c2);

void init_sol_k_best(){//
    int e;
    int v1, v2;
    double v1dd, v2dd;

    uncov_stack_fill_pointer = 0;
    c_size = 0;
    best_weight = (int)(~0U >> 1);

    fill_n(dscore, v_num + 1, 0);

    for (e = 0; e < e_num; e++)
    {
        v1 = edge[e].v1;
        v2 = edge[e].v2;

        if (v_in_c[v1] == 0 && v_in_c[v2] == 0)
        {
            v1dd = (double)v_degree[v1] / (double)v_weight[v1];
            v2dd = (double)v_degree[v2] / (double)v_weight[v2];
            if (v1dd > v2dd)
            {
                v_in_c[v1] = 1;
                now_weight += v_weight[v1];
            }
            else
            {
                v_in_c[v2] = 1;
                now_weight += v_weight[v2];
            }
            c_size++;
        }
    }

    int *save_v_in_c = new int[v_num + 1];
    memcpy(save_v_in_c, v_in_c, sizeof(int) * (v_num + 1));
    int save_c_size = c_size;
    llong save_weight = now_weight;

    int times = 50;
    vector<int> blocks(e_num / 1024 + 1);
    for (int i = 0; i < e_num / 1024 + 1; i++)
    {
        blocks[i] = i;
    }

    while (times-- > 0)
    {
        fill_n(v_in_c, v_num + 1, 0);
        c_size = 0;
        now_weight = 0;

        for (int i = 1; i <= v_num; ++i) {
            v_in_c[i] = must_in_c[i];
            if(v_in_c[i]==1){
                c_size++;
                now_weight+=v_weight[i];
            }
        }

        shuffle(blocks.begin(), blocks.end(), default_random_engine(seed));

        for (auto &block : blocks)
        {
            auto begin = block * 1024;
            auto end = block == e_num / 1024 ? e_num : begin + 1024;
            int tmpsize = end - begin + 1;
            vector<int> idx(tmpsize);
            for (int i = begin; i < end; i++)
            {
                idx[i - begin] = i;
            }
            while (tmpsize > 0)
            {
                int i = rand() % tmpsize;
                Edge e = edge[idx[i]];
                v1 = e.v1;
                v2 = e.v2;
                swap(idx[i], idx[--tmpsize]);
                if (v_in_c[v1] == 0 && v_in_c[v2] == 0)
                {
                    v1dd = (double)v_degree[v1] / (double)v_weight[v1];
                    v2dd = (double)v_degree[v2] / (double)v_weight[v2];
                    if (v1dd > v2dd)
                    {
                        v_in_c[v1] = 1;
                        now_weight += v_weight[v1];
                    }
                    else
                    {
                        v_in_c[v2] = 1;
                        now_weight += v_weight[v2];
                    }
                    c_size++;
                }
            }
        }
        if (now_weight < save_weight)
        {
            save_weight = now_weight;
            save_c_size = c_size;
            memcpy(save_v_in_c, v_in_c, sizeof(int) * (v_num + 1));
        }
    }

    now_weight = save_weight;
    c_size = save_c_size;
    memcpy(v_in_c, save_v_in_c, sizeof(int) * (v_num + 1));
    delete[] save_v_in_c;
}

inline int compare(int s1, int c1, int s2, int c2){
    if(c1==c2) {
        if(s1>s2) return 1;
        else if(s1==s2) return 0;
        else return -1;
    }
    long long t1=s1, t2=s2;
    t1=t1*c2;
    t2=t2*c1;
    if(t1>t2) return 1;
    else if(t1==t2) return 0;
    else return -1;

}

void init_best(){
    int cnt;
    int i,j,k,l,jj;
    int sr, ct;
    int temp_weight = 0;
    for (int i = 1; i <= must_in_c_size; ++i) {
        temp_weight += v_weight[must_in_c[i]];
    }
    memcpy(v_in_c, must_in_c, sizeof(int) * (v_num + 1));

    c_size = must_in_c_size;
    now_weight = temp_weight;
    for (int e = 0; e < e_num; ++e)
    {
        if (v_in_c[edge[e].v1] != 1 && v_in_c[edge[e].v2] != 1)
        {
            Uncover(e);
        }
    }
    while(uncov_stack_fill_pointer>0){
        cout << uncov_stack_fill_pointer << endl;
        temp_list_len = 0;
        sr=INT_MIN; ct=1;
        for(j=1;j<=v_num;j++){
            if(v_in_c[j]) continue;
            if(must_in_c[j]) continue;
            k=compare(sr,ct,dscore[j], v_weight[j]);
            if(sr==INT_MIN||k<0){
                sr=dscore[j];
                ct=v_weight[j];
                temp_list[0] = j;
                temp_list_len = 1;
            } else if(k==0){
                temp_list[temp_list_len] = j;
                temp_list_len++;
            }
        }
        if(temp_list_len>0){
            Add(temp_list[rand()%temp_list_len]);
        }
    }

}

int select_high_dscore_breaking_ties_first(int* cand_set,int set_length, int allow_tabu){
    int v;
    int best_remove_v = -1;
    double best_dscore = INT_MIN;
    double dscore_v;

    for (int i = 0; i < set_length; i++)
    {
        v = cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        dscore_v = (double)(v_weight[v]) / (double)abs(dscore[v]);
        if (dscore_v > best_dscore)
        {
            best_dscore = dscore_v;
            best_remove_v = v;
        }
    }
    if(best_remove_v==-1){
        for (int i = 0; i < remove_cand_size; i++)
        {
            v = remove_cand[i];
            if(must_in_c[v]==1)continue;
            if (dscore[v] == 0) break;

            dscore_v = (double)(v_weight[v]) / (double)abs(dscore[v]);
            if (dscore_v > best_dscore)
            {
                best_dscore = dscore_v;
                best_remove_v = v;
            }
        }
    }
    if(best_remove_v==-1) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return best_remove_v;
}

int select_high_dscore_breaking_ties_age(int* cand_set,int set_length, int allow_tabu){
    int i, v;
    double dscore_v, dscore_remove_v = INT_MIN;
    int remove_v = -1;
    for (i = 0; i < set_length; i++)
    {
        v = cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        dscore_v = (double)v_weight[v] / (double)abs(dscore[v]);
        if (dscore_v < dscore_remove_v)
        {
            continue;
        }
        if (dscore_v > dscore_remove_v)
        {
            remove_v = v;
            dscore_remove_v = dscore_v;
        }
        else if (time_stamp[v] < time_stamp[remove_v])
        {
            remove_v = v;
            dscore_remove_v = dscore_v;
        }
    }
    if(remove_v==-1){
        for (i = 0; i < remove_cand_size; i++)
        {
            v = remove_cand[i];
            if(must_in_c[v]==1)continue;
            if(dscore[v]==0)break;
            dscore_v = (double)v_weight[v] / (double)abs(dscore[v]);
            if (dscore_v < dscore_remove_v)
            {
                continue;
            }
            if (dscore_v > dscore_remove_v)
            {
                remove_v = v;
                dscore_remove_v = dscore_v;
            }
            else if (time_stamp[v] < time_stamp[remove_v])
            {
                remove_v = v;
                dscore_remove_v = dscore_v;
            }
        }
    }
    if(remove_v==-1) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return remove_v;
}

int select_high_dscore_breaking_ties_random(int* cand_set,int set_length, int allow_tabu){
    int i, v;
    double dscore_v, dscore_remove_v=INT_MIN;
    int remove_v = -1;
    temp_list_len = 0;
    for (i = 0; i < set_length; i++)
    {
        v = cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        dscore_v = (double)v_weight[v] / (double)abs(dscore[v]);
        if (dscore_v < dscore_remove_v)
        {
            continue;
        }
        if (dscore_v > dscore_remove_v)
        {
            temp_list[0] = v;
            temp_list_len = 1;
            dscore_remove_v = dscore_v;
        }
        else if (dscore_v == dscore_remove_v)
        {
            temp_list[temp_list_len] = v;
            temp_list_len++;
        }
    }
    if(temp_list_len==0){
        for (i = 0; i < remove_cand_size; i++)
        {
            v = remove_cand[i];
            if(must_in_c[v]==1)continue;
            if(dscore[v]==0)break;
            dscore_v = (double)v_weight[v] / (double)abs(dscore[v]);
            if (dscore_v < dscore_remove_v)
            {
                continue;
            }
            if (dscore_v > dscore_remove_v)
            {
                temp_list[0] = v;
                temp_list_len = 1;
                dscore_remove_v = dscore_v;
            }
            else if (dscore_v == dscore_remove_v)
            {
                temp_list[temp_list_len] = v;
                temp_list_len++;
            }
        }
    }
    if(temp_list_len==0) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    remove_v = temp_list[rand()%temp_list_len];
    return remove_v;
}

int select_vertex_valid_score_breaking_ties_age(int* cand_set,int set_length, int allow_tabu)
{
    int i,v;
    int remove_v = -1;
    int improvement_remove = INT_MAX,improvement_v;
    for (i = 0; i < set_length; i++)
    {
        v=cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        improvement_v = valid_score[v];
        if (improvement_v > improvement_remove)
        {
            continue;
        }
        if(improvement_v < improvement_remove)
        {
            remove_v = v;
            improvement_remove = improvement_v;
        }
        else if (time_stamp[v] < time_stamp[remove_v])
        {
            remove_v = v;
            improvement_remove = improvement_v;
        }
    }
    if(remove_v==-1){
        for (i = 0; i < remove_cand_size; i++)
        {
            v = remove_cand[i];
            if(must_in_c[v]==1)continue;
            improvement_v = valid_score[v];
            if (improvement_v > improvement_remove)
            {
                continue;
            }
            if(improvement_v < improvement_remove)
            {
                remove_v = v;
                improvement_remove = improvement_v;
            }
            else if (time_stamp[v] < time_stamp[remove_v])
            {
                remove_v = v;
                improvement_remove = improvement_v;
            }
        }
    }
    if(remove_v==-1) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return remove_v;
}

int select_vertex_valid_score_breaking_ties_first(int* cand_set,int set_length, int allow_tabu){
    int i,v;
    int remove_v = -1;
    int improvement_remove = INT_MAX,improvement_v;
    for (i = 0; i < set_length; i++)
    {
        v=cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        improvement_v = valid_score[v];
        if (improvement_v > improvement_remove)
        {
            continue;
        }
        if(improvement_v < improvement_remove)
        {
            remove_v = v;
            improvement_remove = improvement_v;
        }
    }
    if(remove_v==-1){
        for (i = 0; i < remove_cand_size; i++)
        {
            v = remove_cand[i];
            if(must_in_c[v]==1)continue;
            improvement_v = valid_score[v];
            if (improvement_v > improvement_remove)
            {
                continue;
            }
            if(improvement_v < improvement_remove)
            {
                remove_v = v;
                improvement_remove = improvement_v;
            }
        }
    }
    if(remove_v==-1) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return remove_v;
}

int select_vertex_valid_score_breaking_ties_random(int* cand_set,int set_length, int allow_tabu){
    int i,v;
    int remove_v = -1;
    int improvement_remove = INT_MAX,improvement_v;
    temp_list_len = 0;
    for (i = 0; i < set_length; i++)
    {
        v=cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        improvement_v = valid_score[v];
        if (improvement_v > improvement_remove)
        {
            continue;
        }
        if(improvement_v < improvement_remove)
        {
            temp_list[0] = v;
            temp_list_len = 1;
            improvement_remove = improvement_v;
        }
        else if (improvement_v == improvement_remove)
        {
            temp_list[temp_list_len] = v;
            temp_list_len++;
        }
    }
    if(temp_list_len==0){
        for (i = 0; i < remove_cand_size; i++)
        {
            v = remove_cand[i];
            if(must_in_c[v]==1)continue;
            improvement_v = valid_score[v];
            if (improvement_v > improvement_remove)
            {
                continue;
            }
            if(improvement_v < improvement_remove)
            {
                temp_list[0] = v;
                temp_list_len = 1;
                improvement_remove = improvement_v;
            }
            else if (improvement_v == improvement_remove)
            {
                temp_list[temp_list_len] = v;
                temp_list_len++;
            }
        }
    }
    if(temp_list_len==0) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    remove_v = temp_list[rand()%temp_list_len];
    return remove_v;
}

int select_high_score_breaking_ties_age(int* cand_set,int set_length, int allow_tabu){
    int i, v, k;
    int dscore_v = INT_MIN;
    int v_cost = 1;
    int remove_v = -1;
    for (i = 0; i < set_length; i++)
    {
        v = cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        k=compare(dscore_v,v_cost, dscore[v], v_weight[v]);
        if(dscore_v==INT_MIN||k<0){
            dscore_v=dscore[v];
            v_cost=v_weight[v];
            remove_v=v;
        } else if(k==0){
            if(time_stamp[remove_v]>time_stamp[v]){
                remove_v=v;
            }
        }
    }
    if(remove_v==-1){
        for (i = 0; i < set_length; i++)
        {
            v = cand_set[i];
            if(must_in_c[v]==1)continue;
            k=compare(dscore_v,v_cost, dscore[v], v_weight[v]);
            if(dscore_v==INT_MIN||k<0){
                dscore_v=dscore[v];
                v_cost=v_weight[v];
                remove_v=v;
            } else if(k==0){
                if(time_stamp[remove_v]>time_stamp[v]){
                    remove_v=v;
                }
            }
        }
    }
    if(remove_v==-1) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return remove_v;
}
int select_high_score_breaking_ties_first(int* cand_set,int set_length, int allow_tabu){
    int i, v, k;
    int dscore_v = INT_MIN;
    int v_cost = 1;
    int remove_v = -1;
    for (i = 0; i < set_length; i++)
    {
        v = cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        k=compare(dscore_v,v_cost, dscore[v], v_weight[v]);
        if(dscore_v==INT_MIN||k<0){
            dscore_v=dscore[v];
            v_cost=v_weight[v];
            remove_v=v;
        }
    }
    if(remove_v==-1){
        for (i = 0; i < set_length; i++)
        {
            v = cand_set[i];
            if(must_in_c[v]==1)continue;
            k=compare(dscore_v,v_cost, dscore[v], v_weight[v]);
            if(dscore_v==INT_MIN||k<0){
                dscore_v=dscore[v];
                v_cost=v_weight[v];
                remove_v=v;
            }
        }
    }
    if(remove_v==-1) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return remove_v;
}

int select_high_score_breaking_ties_random(int* cand_set,int set_length, int allow_tabu){
    int i, v, k;
    int dscore_v = INT_MIN;
    int v_cost = 1;
    int remove_v = -1;
    temp_list_len = 0;
    for (i = 0; i < set_length; i++)
    {
        v = cand_set[i];
        if(allow_tabu&&in_tabu(v))continue;
        if(must_in_c[v]==1)continue;
        k=compare(dscore_v,v_cost, dscore[v], v_weight[v]);
        if(dscore_v==INT_MIN||k<0){
            dscore_v=dscore[v];
            v_cost=v_weight[v];
            temp_list[0] = v;
            temp_list_len=1;
        }
        else if(k==0){
            temp_list[temp_list_len] = v;
            temp_list_len++;
        }
    }
    if(temp_list_len==0){
        for (i = 0; i < set_length; i++)
        {
            v = cand_set[i];
            if(must_in_c[v]==1)continue;
            k=compare(dscore_v,v_cost, dscore[v], v_weight[v]);
            if(dscore_v==INT_MIN||k<0){
                dscore_v=dscore[v];
                v_cost=v_weight[v];
                temp_list[0] = v;
                temp_list_len=1;
            }
            else if(k==0){
                temp_list[temp_list_len] = v;
                temp_list_len++;
            }
        }
    }
    if(temp_list_len==0) {
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    remove_v = temp_list[rand()%temp_list_len];
    return remove_v;
}
int select_vertex_use_bms_1(){
    int i, v, best_removing_v;
    if(remove_cand_size > para_bms_k)
    {
        bms_cand_set_size = 0;
        for(i=0; i<para_bms_k; i++)
        {
            v = remove_cand[rand()%remove_cand_size];
            bms_cand_set[bms_cand_set_size++] = v;
        }
        best_removing_v = del_1_strategy_heuristic_ptr(bms_cand_set, bms_cand_set_size,0);
    }
    else
    {
        best_removing_v = del_1_strategy_heuristic_ptr(remove_cand, remove_cand_size,0);
    }

    return best_removing_v;
}

int select_vertex_not_use_bms_1(){
    return del_1_strategy_heuristic_ptr(remove_cand, remove_cand_size,0);
}

void del_vertex_dynwvc2(){
    int remove_v;
    int remove_degree = 0;
    if (noimprovement<5)
    {
        remove_v = del_2_bms_ptr();
        Remove(remove_v);
        time_stamp[remove_v] = step;
        remove_list[remove_length] = remove_v;
        remove_length++;
    }
    else
    {
        if(noimprovement==5)
        {
            dyn_count=2;
        }
        if(dyn_count==1)
        {
            noimprovement=0;
        }
        remove_v = del_3_bms_ptr();
        Remove(remove_v);
        time_stamp[remove_v] = step;
        dyn_count--;
        remove_list[remove_length] = remove_v;
        remove_length++;
    }
    remove_degree = v_degree[remove_list[0]] + v_degree[remove_list[1]];
    if(remove_degree < 2*e_num/v_num)
    {
        int remove_v2 = del_3_bms_ptr();
        Remove(remove_v2);
        time_stamp[remove_v2] = step;
        remove_list[remove_length] = remove_v2;
        remove_length++;
    }
}

void del_vertex_numwvc(){
    if(NumRemove>1 && NoImpro>=100){
        NumRemove--;
        NoImpro=0;
    }
    int remove_v;

    for(int hh=0; hh<NumRemove;hh++)
    {
        remove_v=del_2_bms_ptr();
        Remove(remove_v);
        time_stamp[remove_v] = step;
        remove_list[remove_length] = remove_v;
        remove_length++;
    }
}

void del_vertex_fastwvc(){
    int remove_v=del_2_bms_ptr();
    Remove(remove_v);
    time_stamp[remove_v] = step;
    remove_list[remove_length] = remove_v;
    remove_length++;
}

int select_vertex_use_bms_2(){
    int i, v, best_removing_v;
    if(remove_cand_size > para_bms_k)
    {
        bms_cand_set_size = 0;
        for(i=0; i<para_bms_k; i++)
        {
            v = remove_cand[rand()%remove_cand_size];
            bms_cand_set[bms_cand_set_size++] = v;
        }
        best_removing_v = del_2_strategy_heuristic_ptr(bms_cand_set, bms_cand_set_size,use_tabu);
    }
    else
    {
        best_removing_v = del_2_strategy_heuristic_ptr(remove_cand, remove_cand_size,use_tabu);
    }

    return best_removing_v;
}

int select_vertex_not_use_bms_2(){
    return del_2_strategy_heuristic_ptr(remove_cand, remove_cand_size,use_tabu);
}

int select_vertex_use_bms_3(){
    int i, v, best_removing_v;
    if(remove_cand_size > para_bms_k)
    {
        bms_cand_set_size = 0;
        for(i=0; i<para_bms_k; i++)
        {
            v = remove_cand[rand()%remove_cand_size];
            bms_cand_set[bms_cand_set_size++] = v;
        }
        best_removing_v = del_3_strategy_heuristic_ptr(bms_cand_set, bms_cand_set_size,use_tabu);
    }
    else
    {
        best_removing_v = del_3_strategy_heuristic_ptr(remove_cand, remove_cand_size,use_tabu);
    }

    return best_removing_v;
}

int select_vertex_not_use_bms_3(){
    return del_3_strategy_heuristic_ptr(remove_cand, remove_cand_size,use_tabu);
}

int select_add_high_score_breaking_ties_age()
{
    int i, v;
    int add_v = -1;
    double improvemnt = INT_MIN;
    double dscore_v;

    for (int j = 0; j < remove_length; ++j) {
        int tmp_degree = v_degree[remove_list[j]];
        int *adjp = v_adj[remove_list[j]];
        for (i = 0; i < tmp_degree; i++)
        {
            v = adjp[i];
            if(CC_strategy&&conf_change[v]==0)continue;
            if (v_in_c[v] == 1)
            {
                continue;
            }
            dscore_v = (double)dscore[v] / (double)(v_weight[v]);
            if (dscore_v > improvemnt)
            {
                improvemnt = dscore_v;
                add_v = v;
            }
            else if (dscore_v == improvemnt)
            {
                if (time_stamp[v] < time_stamp[add_v])
                {
                    add_v = v;
                }
            }
        }
    }
    if(add_v==-1){
        improvemnt = INT_MIN;
        for (int j = 0; j < remove_length; ++j) {
            int tmp_degree = v_degree[remove_list[j]];
            int *adjp = v_adj[remove_list[j]];
            for (i = 0; i < tmp_degree; i++)
            {
                v = adjp[i];
                if (v_in_c[v] == 1)
                {
                    continue;
                }
                dscore_v = (double)dscore[v] / (double)(v_weight[v]);
                if (dscore_v > improvemnt)
                {
                    improvemnt = dscore_v;
                    add_v = v;
                }
                else if (dscore_v == improvemnt)
                {
                    if (time_stamp[v] < time_stamp[add_v])
                    {
                        add_v = v;
                    }
                }
            }
        }
    }
    if(add_v==-1){
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return add_v;
}

int select_add_high_score_breaking_ties_first(){
    int i, v;
    int add_v = -1;
    double improvemnt = INT_MIN;
    double dscore_v;

    for (int j = 0; j < remove_length; ++j) {
        int tmp_degree = v_degree[remove_list[j]];
        int *adjp = v_adj[remove_list[j]];
        for (i = 0; i < tmp_degree; i++)
        {
            v = adjp[i];
            if(CC_strategy&&conf_change[v]==0)continue;
            if (v_in_c[v] == 1)
            {
                continue;
            }
            dscore_v = (double)dscore[v] / (double)(v_weight[v]);
            if (dscore_v > improvemnt)
            {
                improvemnt = dscore_v;
                add_v = v;
            }
        }
    }
    if(add_v==-1){
        improvemnt = INT_MIN;
        for (int j = 0; j < remove_length; ++j) {
            int tmp_degree = v_degree[remove_list[j]];
            int *adjp = v_adj[remove_list[j]];
            for (i = 0; i < tmp_degree; i++)
            {
                v = adjp[i];
                if (v_in_c[v] == 1)
                {
                    continue;
                }
                dscore_v = (double)dscore[v] / (double)(v_weight[v]);
                if (dscore_v > improvemnt)
                {
                    improvemnt = dscore_v;
                    add_v = v;
                }
            }
        }
    }
    if(add_v==-1){
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    return add_v;
}

int select_add_high_score_breaking_ties_random(){
    int i, v;
    int add_v = -1;
    double improvemnt = INT_MIN;
    double dscore_v;
    temp_list_len = 0;
    for (int j = 0; j < remove_length; ++j) {
        int tmp_degree = v_degree[remove_list[j]];
        int *adjp = v_adj[remove_list[j]];
        for (i = 0; i < tmp_degree; i++)
        {
            v = adjp[i];
            if(CC_strategy&&conf_change[v]==0)continue;
            if (v_in_c[v] == 1)
            {
                continue;
            }
            dscore_v = (double)dscore[v] / (double)(v_weight[v]);
            if (dscore_v > improvemnt)
            {
                temp_list[0] = v;
                temp_list_len = 1;
                improvemnt = dscore_v;
            }else if(dscore_v == improvemnt){
                temp_list[temp_list_len] = v;
                temp_list_len ++;
            }
        }
    }
    if(temp_list_len==0){
        improvemnt = INT_MIN;
        for (int j = 0; j < remove_length; ++j) {
            int tmp_degree = v_degree[remove_list[j]];
            int *adjp = v_adj[remove_list[j]];
            for (i = 0; i < tmp_degree; i++)
            {
                v = adjp[i];
                if (v_in_c[v] == 1)
                {
                    continue;
                }
                dscore_v = (double)dscore[v] / (double)(v_weight[v]);
                if (dscore_v > improvemnt)
                {
                    temp_list[0] = v;
                    temp_list_len = 1;
                    improvemnt = dscore_v;
                }else if(dscore_v == improvemnt){
                    temp_list[temp_list_len] = v;
                    temp_list_len ++;
                }
            }
        }
    }
    if(temp_list_len==0){
        if(best_comp_time>1000)best_comp_time=0.001;
        cout << best_weight << " " << best_comp_time <<endl;
        exit(0);
    }
    add_v = temp_list[rand()%temp_list_len];
    return add_v;
}

int select_add_from_edge_high_score_breaking_ties_age(){
    int add_v=-1;

    int k = rand()%uncov_stack_fill_pointer;
    int e = uncov_stack[k];
    int v1,v2;
    v1 = edge[e].v1;
    v2 = edge[e].v2;
    if(conf_change[v1]==0&&now_weight+v_weight[v1]<best_weight){
        int j_uncover_edges=0;
        for(int ii=0;ii<v_degree[v1];ii++){
            int ee=v_edges[v1][ii];
            if(uncov_stack[ee]!=-1) j_uncover_edges++;
        }
        if(j_uncover_edges==uncov_stack_fill_pointer){add_v=v1;}
    }
    else{
        add_v = v1;
    }

    if(conf_change[v2]==0&&now_weight+v_weight[v2]<best_weight)
    {
        int j_uncover_edges=0;
        for(int ii=0;ii<v_degree[v2];ii++){
            int ee=v_edges[v2][ii];
            if(uncov_stack[ee]!=-1) j_uncover_edges++;
        }
        if(j_uncover_edges==uncov_stack_fill_pointer){add_v=v2;}
    }else{
        if(add_v==-1)add_v = v2;
        else{
            int c = compare(dscore[v1],v_weight[v1],dscore[v2],v_weight[v2]);
            if(c<0){
                add_v = v2;
            }else if(c==0){
                if(time_stamp[v1]>time_stamp[v2]){
                    add_v = v2;
                }
            }
        }
    }
    return add_v;
}

int select_add_from_edge_high_score_breaking_ties_first(){
    int add_v=-1;

    int k = rand()%uncov_stack_fill_pointer;
    int e = uncov_stack[k];
    int v1,v2;
    v1 = edge[e].v1;
    v2 = edge[e].v2;
    if(conf_change[v1]==0&&now_weight+v_weight[v1]<best_weight){
        int j_uncover_edges=0;
        for(int ii=0;ii<v_degree[v1];ii++){
            int ee=v_edges[v1][ii];
            if(uncov_stack[ee]!=-1) j_uncover_edges++;
        }
        if(j_uncover_edges==uncov_stack_fill_pointer){add_v=v1;}
    }
    else{
        add_v = v1;
    }

    if(conf_change[v2]==0&&now_weight+v_weight[v2]<best_weight)
    {
        int j_uncover_edges=0;
        for(int ii=0;ii<v_degree[v2];ii++){
            int ee=v_edges[v2][ii];
            if(uncov_stack[ee]!=-1) j_uncover_edges++;
        }
        if(j_uncover_edges==uncov_stack_fill_pointer){add_v=v2;}
    }else{
        if(add_v==-1)add_v = v2;
        else{
            int c = compare(dscore[v1],v_weight[v1],dscore[v2],v_weight[v2]);
            if(c<0){
                add_v = v2;
            }
        }
    }
    return add_v;
}

int select_add_from_edge_high_score_breaking_ties_random(){
    int add_v=-1;

    int k = rand()%uncov_stack_fill_pointer;
    int e = uncov_stack[k];
    int v1,v2;
    v1 = edge[e].v1;
    v2 = edge[e].v2;
    if(conf_change[v1]==0&&now_weight+v_weight[v1]<best_weight){
        int j_uncover_edges=0;
        for(int ii=0;ii<v_degree[v1];ii++){
            int ee=v_edges[v1][ii];
            if(uncov_stack[ee]!=-1) j_uncover_edges++;
        }
        if(j_uncover_edges==uncov_stack_fill_pointer){add_v=v1;}
    }
    else{
        add_v = v1;
    }

    if(conf_change[v2]==0&&now_weight+v_weight[v2]<best_weight)
    {
        int j_uncover_edges=0;
        for(int ii=0;ii<v_degree[v2];ii++){
            int ee=v_edges[v2][ii];
            if(uncov_stack[ee]!=-1) j_uncover_edges++;
        }
        if(j_uncover_edges==uncov_stack_fill_pointer){add_v=v2;}
    }else{
        if(add_v==-1)add_v = v2;
        else{
            int c = compare(dscore[v1],v_weight[v1],dscore[v2],v_weight[v2]);
            if(c<0){
                add_v = v2;
            }
            else if(c==0){
                if(rand()%100<50)add_v = v1;
                else add_v = v2;
            }
        }
    }
    return add_v;
}

