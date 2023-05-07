#include <iostream>
#include "parse_arguement.h"
#include "basic.h"
#include "settings.h"
#include "localSearch.h"
#include "reduction.h"
using namespace std;

int main_framework(){
    set_default_setting();
    if(!build_instance())
    {
        cout << "c" << endl;
        cout << "c Instance File Error!" << endl;
        cout << "c" << endl;
        return -1;
    }

    Reduction();
    init_sol();

    cover_LS();

    return 0;

}

int main(int argc,char** argv) {
    if(!parse_arguments(argc, argv))
    {
        cout << "c" << endl;
        cout << "c Arguments Error!" << endl;
        cout << "c" << endl;
        return -1;
    }
    srand(seed);
    main_framework();

    if (CheckSolution() == 1)
    {
        cout << best_weight << " " << best_comp_time <<endl;
    }
    else
    {
        cout << ", the solution is wrong." << endl;
    }
    free_memory();

    return 0;
}