//
// Created by yanglq on 2022/4/21.
//
#include "basic.h"

bool ExeRule1(){

    int i;
    bool  isAdd=false;
    int v,numNotInC,n1,n2;
    int num=0;
    int CycleNum=0;
    while(CycleNum<1){

        if(uncov_stack_fill_pointer==0) break;
        bool isFFind=false;
        for(v=1;v<=v_num;v++){
            if(uncov_stack_fill_pointer==0) break;
            if(v_in_c[v]==1) continue;

            int *neighborNotInC=new int[v_degree[v]];
            numNotInC=0;

            //printf("v===%d\n",v);
            if(dscore[v]==1){

                for(i=0;i<v_degree[v];i++){
                    int temp=v_adj[v][i];
                    if(v_in_c[temp]==0){
                        neighborNotInC[numNotInC++]= temp;
                    }
                }
            }

            if(numNotInC==1){
                n1=neighborNotInC[0];

                if(v_weight[v]>=v_weight[n1]){
                    isAdd=true;
                    isFFind=true;
                    Add(n1);
                    must_in_c[n1]=1;
                    num++;
                }
            }
            delete [] neighborNotInC;
        }
        CycleNum++;
        if(!isFFind) break;
    }
    if(isAdd) return true;
    else return false;
}


bool ExeRule2(){
    int v,numNotInC,n1,n2;
    int i;
    bool isAdd=false;
    int CycleNum=0;
    while(CycleNum<1){
        if(uncov_stack_fill_pointer==0) break;
        bool isFind=false;
        for(v=1;v<=v_num;v++){
            if(uncov_stack_fill_pointer==0) break;
            if(v_in_c[v]==1) continue;
            int *neighborNotInC=new int[v_degree[v]];
            numNotInC=0;
            if(dscore[v]==2){

                for(i=0;i<v_degree[v];i++){
                    int temp=v_adj[v][i];
                    if(v_in_c[temp]==0){
                        neighborNotInC[numNotInC++]= temp;
                    }
                }
            }

            if(numNotInC==2){
                n1=neighborNotInC[0];
                n2=neighborNotInC[1];
                bool is=false;
                for(i=0;i<v_degree[n1];i++){
                    if(v_adj[n1][i]==n2){
                        is=true;
                        break;
                    }
                }

                if(is){
                    if(v_weight[v]>v_weight[n1]+v_weight[n2]){
                        isFind=true;
                        isAdd=true;
                        must_in_c[n1]=1;
                        must_in_c[n2]=1;
                        Add(n1);
                        Add(n2);
                    }

                }
            }
            delete [] neighborNotInC;

        }
        if(!isFind) break;
        CycleNum++;

    }

    if(isAdd) return true;
    else return false;

}

bool ExeRule3(){
    int v,numNotInC,n1,n2;
    int i;
    bool isAdd=false;
    int CycleNum=0;
    while(CycleNum<1){
        if(uncov_stack_fill_pointer==0) break;
        bool isFind=false;
        for(v=1;v<=v_num;v++){
            if(uncov_stack_fill_pointer==0) break;
            if(v_in_c[v]==1) continue;

            int *neighborNotInC=new int[v_degree[v]];
            numNotInC=0;
            if(dscore[v]==2){

                for(i=0;i<dscore[v];i++){
                    int temp=v_adj[v][i];
                    if(v_in_c[temp]==0){
                        neighborNotInC[numNotInC++]= temp;
                    }
                }
            }

            if(numNotInC==2){
                n1=neighborNotInC[0];
                n2=neighborNotInC[1];
                bool is=false;
                for(i=0;i<v_degree[n1];i++){
                    if(v_adj[n1][i]==n2){
                        is=true;
                        break;
                    }
                }

                if(is){

                    if(dscore[n1]==2){
                        isFind=true;
                        isAdd=true;
                        if(v_weight[v]<v_weight[n1]){
                            must_in_c[v]=1;
                            Add(v);
                        }else{
                            must_in_c[n1]=1;
                            Add(n1);
                        }
                    }else if(dscore[n2]==2){
                        isFind=true;
                        isAdd=true;
                        if(v_weight[v]<v_weight[n2]){
                            must_in_c[v]=1;
                            Add(v);
                        }else{
                            must_in_c[n2]=1;
                            Add(n2);
                        }
                    }

                }
            }
            delete [] neighborNotInC;

        }
        if(!isFind) break;
        CycleNum++;

    }

    if(isAdd) return true;
    else return false;

}

bool ExeRule4(){
    int v,numNotInC,n1,n2;
    int i;
    bool isAdd=false;
    int CycleNum=0;
    while(CycleNum<1){
        if(uncov_stack_fill_pointer==0) break;
        bool isFFind=false;
        for(v=1;v<=v_num;v++){

            if(uncov_stack_fill_pointer==0) break;
            if(v_in_c[v]==1) continue;

            int *neighborNotInC=new int[v_degree[v]];
            numNotInC=0;
            if(dscore[v]==2){

                for(i=0;i<v_degree[v];i++){
                    int temp=v_adj[v][i];
                    if(v_in_c[temp]==0){
                        neighborNotInC[numNotInC++]= temp;
                    }
                }
            }

            if(numNotInC==2){
                n1=neighborNotInC[0];
                n2=neighborNotInC[1];
                bool is=false;
                for(i=0;i<v_degree[n1];i++){
                    if(v_adj[n1][i]==n2){
                        is=true;
                        break;
                    }
                }

                if(!is){
                    bool isfind=false;
                    int u;
                    for(i=0;i<v_degree[n1];i++){
                        u=v_adj[n1][i];
                        if(u==v) continue;
                        if(v_in_c[u]==0&&dscore[u]==2){
                            for(int ii=0;ii<v_degree[n2];ii++){
                                if(v_adj[n2][ii]==u){
                                    isfind=true;
                                }
                            }
                            if(isfind) break;
                        }
                    }
                    if(isfind){
                        if(v_weight[v]+v_weight[u]>v_weight[n1]+v_weight[n2]){

                            isAdd=true;
                            isFFind=true;
                            Add(n1);
                            Add(n2);
                            must_in_c[n1]=true;
                            must_in_c[n2]=true;
                        }
                    }
                }
            }
            delete [] neighborNotInC;

        }
        if(!isFFind) break;
        CycleNum++;

    }

    if(isAdd) return true;
    else return false;

}

void Reduction(){

    for (int i = 1; i <= v_num; ++i) {
        dscore[i] = v_degree[i];
    }

    while(true){
        if(uncov_stack_fill_pointer==0) break;

        bool rule3=ExeRule3();

        bool rule4=ExeRule4();

        bool rule2=ExeRule2();

        bool rule1=ExeRule1();

        if(rule1||rule2||rule3||rule4) continue;
        else break;
    }

    must_in_c_size = 0;
    for(int v=1;v<=v_num;v++){

        if(must_in_c[v]==1) {
            must_in_c_size++;
        }
    }
}

