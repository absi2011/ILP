#include<set>
#include<map>
#include<list>
#include<queue>
#include<stack>
#include<string>
#include<math.h>
#include<time.h>
#include<vector>
#include<memory>
#include<bitset>
#include<stdio.h>
#include<fstream>
#include<sstream>
#include<utility>
#include<assert.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<algorithm>

#include "bound.h"
using namespace std;

vector<string> goal;
vector<bound> bounds;
map<string, int> var_name;
int sum_vars = 0;

void data_input(string file_name)
{
    static char buff[85];
    freopen(file_name.c_str(), "r", stdin);
    ios::sync_with_stdio(false);
    cin.getline(buff, 80);
    string goal_name;
    cin >> goal_name;
    assert(goal_name == "Minimize");
    string var, sym;
    for (;;)
    {
        cin >> var;
        cin >> sym;
        goal.push_back(var);
        if (sym != "+")
        {
            break;
        } 
    }
    assert(sym == "Subject");
    cin >> sym;
    assert(sym == "To");
    for (;;)
    {
        string func_name;
        cin >> func_name;
        if (func_name == "Bounds")
        {
            break;
        }
        bound x;
        x.name = func_name;
        x.positive.push_back(true);
        for (;;)
        {
            cin >> var;
            if (var == "-")
            {
                x.positive[0] = false;
                continue;
            }
            x.vars.push_back(var);
            cin >> sym;
            if ((sym == "=") || (sym == ">=") || (sym == "<="))
            {
                x.relation = sym;
                cin >> x.val;
                break;
            }
            if (sym == "-")
            {
                x.positive.push_back(false);
            }
            else if (sym == "+")
            {
                x.positive.push_back(true);
            }
            else
            {
                cout<<func_name<<" "<<sym<<endl;
                assert(false);
            }
        }
        if (x.relation == ">=")
        {
            x.relation = "<=";
            int i;
            for (i = 0; i < (int)x.positive.size();i++)
            {
                x.positive[i] = !x.positive[i];
            }
        }
        bounds.push_back(x);
    }
    cin >> var;
    assert(var == "Binaries");
    for (;;)
    {
        cin >> var;
        if (var == "End")
        {
            break;
        }
        var_name[var] = sum_vars++;
    }
}
