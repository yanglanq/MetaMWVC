//
// Created by yanglq on 2022/4/12.
//
#include "basic.h"
#include "settings.h"
chrono::steady_clock::time_point start;

string inst;

llong   max_steps;
llong   step;
int     try_step;
uint    seed;
int     cutoff_time;
int     mode;

int     v_num;
int     e_num;

Edge    *edge;
int     *edge_weight;

int     *dscore;
llong   *time_stamp;
int     *valid_score;

int     *v_weight;
int     **v_edges;
int     **v_adj;
int     *v_degree;

int     c_size;
int     *v_in_c;
int     *must_in_c;
int     must_in_c_size;
int     *remove_cand;
int     *index_in_remove_cand;
int     remove_cand_size;
llong   now_weight;

int     best_c_size;
int     *best_v_in_c;
double  best_comp_time;
llong   best_step;
llong   best_weight;

int     *uncov_stack;
int     uncov_stack_fill_pointer;
int     *index_in_uncov_stack;

int     ave_weight;
int     delta_total_weight;
int     threshold;
double  p_scale;
int     para_bms_k;
int     bms_cand_set_size;
int*    bms_cand_set;
int     noimprovement;
int     dyn_count;
int     *remove_list;
int     remove_length;
bool    forget_edge_weight;


int     NumRemove;
int     NoImpro;

int     CC_strategy;
int     use_tabu;
int     use_hash;
int     *conf_change;
set<int> tabu_list;

int     L;
int     current_hashk1;
int     current_hashk2;
int     current_hashk3;
int     *wk1;
int     *wk2;
int     *wk3;
set<int> hashk1;
set<int> hashk2;
set<int> hashk3;
set<int> tabu_list_hash;

int      *temp_list;
int      temp_list_len;

double TimeElapsed()
{
    chrono::steady_clock::time_point finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    return duration.count();
}

bool build_instance()
{
    string tmp;
    string sign;
    int v, e;
    int v1, v2;

    ifstream infile(inst);
    if (!infile)
    {
        return false;
    }
    cout << inst << " " << seed << endl;
    infile >> sign >> tmp >> v_num >> e_num;
    edge = new Edge[e_num];
    edge_weight = new int[e_num];
    uncov_stack = new int[e_num];
    index_in_uncov_stack = new int[e_num];
    dscore = new int[v_num + 1];
    valid_score = new int[v_num +1];
    time_stamp = new llong[v_num + 1];
    v_edges = new int*[v_num + 1];
    v_adj = new int*[v_num + 1];
    v_degree = new int[v_num + 1];
    v_weight = new int[v_num + 1];
    v_in_c = new int[v_num + 1];
    must_in_c = new int[v_num + 1];
    remove_cand = new int[v_num + 1];
    index_in_remove_cand = new int[v_num + 1];
    best_v_in_c = new int[v_num + 1];
    bms_cand_set = new int[v_num + 1];
    conf_change = new int[v_num + 1];
    wk1 = new int[v_num + 1];
    wk2 = new int[v_num + 1];
    wk3 = new int[v_num + 1];
    remove_list = new int[v_num+1];
    for (int i = 1; i <=  v_num; ++i) {
        wk1[i] = pow(i,1.3);
        wk2[i] = pow(i,1.5);
        wk3[i] = pow(i,1.8);
    }
    temp_list = new int[v_num + 1];


    fill_n(v_degree, v_num + 1, 0);
    fill_n(v_in_c, v_num + 1, 0);
    fill_n(must_in_c, v_num + 1, 0);
    fill_n(time_stamp, v_num + 1, 0);
    fill_n(edge_weight, e_num, 1);
    fill_n(valid_score, v_num+1, 1000000);
    fill_n(conf_change, v_num+1, 1);

    for (v = 1; v < v_num + 1; v++)
    {
        infile >> sign >> tmp >> v_weight[v];
    }

    for (e = 0; e < e_num; e++)
    {
        infile >> tmp >> v1 >> v2;
        v_degree[v1]++;
        v_degree[v2]++;

        edge[e].v1 = v1;
        edge[e].v2 = v2;
        Uncover(e);
    }
    infile.close();

    v_adj[0] = new int[1];
    v_edges[0] = new int[1];
    for (v = 1; v < v_num + 1; v++)
    {
        v_adj[v] = new int[v_degree[v]];
        v_edges[v] = new int[v_degree[v]];
    }

    int *v_degree_tmp = new int[v_num + 1];
    fill_n(v_degree_tmp, v_num + 1, 0);

    for (e = 0; e < e_num; e++)
    {
        v1 = edge[e].v1;
        v2 = edge[e].v2;

        v_edges[v1][v_degree_tmp[v1]] = e;
        v_edges[v2][v_degree_tmp[v2]] = e;

        v_adj[v1][v_degree_tmp[v1]] = v2;
        v_adj[v2][v_degree_tmp[v2]] = v1;

        v_degree_tmp[v1]++;
        v_degree_tmp[v2]++;
    }
    delete[] v_degree_tmp;

    return true;
}


void init_sol()
{
    int v, e;

    uncov_stack_fill_pointer = 0;
    c_size = 0;
    best_weight = (int)(~0U >> 1);

    init_sol_heuristic_ptr();

    int v1,v2;
    for (e = 0; e < e_num; e++)
    {
        v1 = edge[e].v1;
        v2 = edge[e].v2;

        if (v_in_c[v1] == 1 && v_in_c[v2] == 0)
        {
            dscore[v1] -= edge_weight[e];
        }
        else if (v_in_c[v2] == 1 && v_in_c[v1] == 0)
        {
            dscore[v2] -= edge_weight[e];
        }
    }


    ResetRemoveCand();
    for (v = 1; v < v_num + 1; v++)
    {
        if (v_in_c[v] == 1 && must_in_c[v]==0 && dscore[v] == 0)
        {
            Remove(v);
        }
    }

    UpdateBestSolution();
}

void ResetRemoveCand()
{
    int v,degree,i;
    int j = 0;

    for (v = 1; v < v_num + 1; v++)
    {
        if (v_in_c[v] == 1)
        {
            remove_cand[j] = v;
            index_in_remove_cand[v] = j;
            j++;

            valid_score[v] = -v_weight[v];
            degree = v_degree[v];
            for (i=0; i < degree; i++)
            {
                if (v_in_c[v_adj[v][i]]==0)
                {
                    valid_score[v] += v_weight[v_adj[v][i]];
                }
            }
        }
        else
        {
            index_in_remove_cand[v] = 0;
        }
    }

    remove_cand_size = j;
}

void record_hash(){
    if(hashk1.find(current_hashk1)==hashk1.end())hashk1.insert(current_hashk1);
    if(hashk2.find(current_hashk2)==hashk2.end())hashk2.insert(current_hashk2);
    if(hashk3.find(current_hashk3)==hashk3.end())hashk3.insert(current_hashk3);
}

int check_solution_in_hash(int v){
    if(hashk1.find(current_hashk1+wk1[v])!=hashk1.end()&&
    hashk2.find(current_hashk2+wk2[v])!=hashk2.end()&&
    hashk3.find(current_hashk3+wk3[v])!=hashk3.end())return 1;
    return 0;
}

void UpdateBestSolution()
{
    int v;
    if (now_weight < best_weight)
    {
        NoImpro=0;
        if(use_hash)record_hash();
        for (v = 1; v < v_num + 1; v++)
        {
            best_v_in_c[v] = v_in_c[v];
        }
        best_weight = now_weight;
        best_c_size = c_size;
        best_comp_time = TimeElapsed();
        best_step = step;
        CheckSolution();
    }else{
        NoImpro++;
    }
}

inline void Uncover(int e)
{
    index_in_uncov_stack[e] = uncov_stack_fill_pointer;
    push(e, uncov_stack);
}

inline void Cover(int e)
{
    int index, last_uncov_edge;
    last_uncov_edge = pop(uncov_stack);
    index = index_in_uncov_stack[e];
    uncov_stack[index] = last_uncov_edge;
    index_in_uncov_stack[last_uncov_edge] = index;
}

void add_conf_change0(int v){}
void remove_conf_change0(int v){}

void remove_conf_change1(int v){
    int i,n,e;
    conf_change[v] = 0;
    int edge_count = v_degree[v];
    for (i = 0; i < edge_count; i++)
    {
        n = v_adj[v][i];
        if (v_in_c[n] == 0)
        {
            conf_change[n] = 1;
        }
    }
}

void add_conf_change1(int v){
    int i,n,e;
    int edge_count = v_degree[v];
    for (i = 0; i < edge_count; i++)
    {
        n = v_adj[v][i];
        if (v_in_c[n] == 0)
        {
            conf_change[n] = 1;
        }
    }
}

void remove_conf_change2(int v){
    int i,n,e;
    conf_change[v] = 0;
    int edge_count = v_degree[v];
    for (i = 0; i < edge_count; i++)
    {
        n = v_adj[v][i];
        conf_change[n] = 1;
    }
}

void add_conf_change2(int v){
    int i,n;
    conf_change[v] = 0;
    int edge_count = v_degree[v];
    for (i = 0; i < edge_count; i++)
    {
        n = v_adj[v][i];
        conf_change[n] = 1;
    }
}

void Remove(int v)
{
    int i, e, n;
    int edge_count = v_degree[v];

    v_in_c[v] = 0;
    c_size--;
    dscore[v] = -dscore[v];
    valid_score[v]=1000000;


    int last_remove_cand_v = remove_cand[--remove_cand_size];
    int index = index_in_remove_cand[v];
    remove_cand[index] = last_remove_cand_v;
    index_in_remove_cand[last_remove_cand_v] = index;
    index_in_remove_cand[v] = 0;

    now_weight -= v_weight[v];

    for (i = 0; i < edge_count; i++)
    {
        e = v_edges[v][i];
        n = v_adj[v][i];

        if (v_in_c[n] == 0)
        {
            dscore[n] += edge_weight[e];
            Uncover(e);
        }
        else
        {
            dscore[n] -= edge_weight[e];
            valid_score[n] += v_weight[v];
        }
    }
    remove_conf_change(v);
}

void Add(int v)
{
    int i, e, n;
    int edge_count = v_degree[v];

    v_in_c[v] = 1;
    c_size++;
    dscore[v] = -dscore[v];
    now_weight += v_weight[v];
    valid_score[v] = -v_weight[v];

    remove_cand[remove_cand_size] = v;
    index_in_remove_cand[v] = remove_cand_size++;

    for (i = 0; i < edge_count; i++)
    {
        e = v_edges[v][i];
        n = v_adj[v][i];

        if (v_in_c[n] == 0)
        {
            dscore[n] -= edge_weight[e];
            Cover(e);
            valid_score[v] += v_weight[n];
        }
        else
        {
            dscore[n] += edge_weight[e];
            valid_score[n] -= v_weight[v];
            if(valid_score[n] == -v_weight[n])
            {
                Remove(n);
            }
        }
    }
    add_conf_change(v);
}

void ForgetEdgeWeights()
{
    int v, e;
    int new_total_weitght = 0;

    for (v = 1; v < v_num + 1; v++)
    {
        dscore[v] = 0;
    }

    for (e = 0; e < e_num; e++)
    {
        edge_weight[e] = edge_weight[e] * p_scale;
        new_total_weitght += edge_weight[e];

        if (v_in_c[edge[e].v1] + v_in_c[edge[e].v2] == 0)
        {
            dscore[edge[e].v1] += edge_weight[e];
            dscore[edge[e].v2] += edge_weight[e];
        }
        else if (v_in_c[edge[e].v1] + v_in_c[edge[e].v2] == 1)
        {
            if (v_in_c[edge[e].v1] == 1)
            {
                dscore[edge[e].v1] -= edge_weight[e];
            }
            else
            {
                dscore[edge[e].v2] -= edge_weight[e];
            }
        }
    }
    ave_weight = new_total_weitght / e_num;
}

void UpdateEdgeWeight()
{
    int i, e;

    for (i = 0; i < uncov_stack_fill_pointer; i++)
    {
        e = uncov_stack[i];
        edge_weight[e] += 1;
        dscore[edge[e].v1] += 1;
        dscore[edge[e].v2] += 1;
        conf_change[edge[e].v1] = 1;
        conf_change[edge[e].v2] = 1;
    }

    delta_total_weight += uncov_stack_fill_pointer;
    if (forget_edge_weight)
    {
        if (delta_total_weight >= e_num)
        {
            ave_weight += 1;
            delta_total_weight -= e_num;
        }

        if (ave_weight >= threshold)
        {
            ForgetEdgeWeights();
        }
    }
}

int CheckSolution()
{
    int e,v;

    for (e = 0; e < e_num; ++e)
    {
        if (best_v_in_c[edge[e].v1] != 1 && best_v_in_c[edge[e].v2] != 1)
        {
            cout << "uncovered edge " << e;
            exit(0);
        }
    }
    return 1;
}

int in_tabu(int i){
    return tabu_list.find(i)!=tabu_list.end();
}

int in_tabu_hash(int i){
    return tabu_list_hash.find(i)!=tabu_list_hash.end();
}

void free_memory(){
    delete []edge;
    delete []edge_weight;
    delete []uncov_stack;
    delete []index_in_uncov_stack;
    delete []dscore;
    delete []valid_score;
    delete []time_stamp;
    for (int i = 1; i <= v_num; ++i) {
        delete []v_edges[i];
    }
    delete []v_edges;
    for (int i = 1; i <= v_num; ++i) {
        delete []v_adj[i];
    }
    delete []v_adj;
    delete []v_degree;
    delete []v_weight;
    delete []v_in_c;
    delete []must_in_c;
    delete []remove_cand;
    delete []index_in_remove_cand;
    delete []best_v_in_c;
    delete []bms_cand_set;
    delete []conf_change;
    delete []wk1;
    delete []wk2;
    delete []wk3;
    delete []temp_list;
    delete []remove_list;
}


