#ifndef _BOUND_H_
#define _BOUND_H_
#include<vector>
#include<string>
struct bound
{
    std::vector<std::string> vars;
    std::vector<int> vars_id;
    std::vector<bool> positive;
    std::string relation;
    std::string name;
    int val;
};
#endif //_BOUND_H_
