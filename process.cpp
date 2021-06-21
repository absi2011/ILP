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
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<algorithm>

#include "bound.h"
using namespace std;

extern int sum_vars;
extern map<string, int> var_name;
extern vector<string> goal;
extern vector<bound> bounds;
double sigma;
struct vec
{
    vector<double> v;
    vec()
    {
        v.resize(sum_vars);
    }
    const double operator [] (int x) const
    {
        return v[x];
    }
    double & operator [] (int x)
    {
        return v[x];
    }
    double operator * (const vec &y) const
    {
        double sum = 0;
        int i;
        for (i = 0; i < sum_vars; i++)
        {
            sum += v[i] * y[i];
        }
        return sum;
    }
    
    double operator * (const bound x) const
    {
        double sum = 0;
        int i;
        for (i = 0; i < (int)x.vars_id.size(); i++)
        {
            if (x.positive[i])
            {
                sum += v[x.vars_id[i]]; 
            }
            else
            {
                sum -= v[x.vars_id[i]];
            }
        }
        return x.val - sum;
    }
    friend vec operator + (const vec &x, vec y)
    {
        int i;
        for (i = 0; i < sum_vars; i++)
        {
            y[i] += x[i];
        }
        return y;
    }
    friend vec operator * (const double y, vec x)
    {
        int i;
        for (i = 0; i < sum_vars; i++)
        {
            x[i] *= y;
        }
        return x;
    }
    void divide()
    {
        int i;
        for (i = 0; i < sum_vars; i++)
        {
            if (v[i] <= 0.5)
            {
                v[i] = 0;
            }
            else
            {
                v[i] = 1;
            }
        }
    }
    void add(double x, bound y)
    {
        int i;
        for (i = 0; i < (int)y.vars_id.size(); i++)
        {
            if (y.positive[i])
            {
                v[y.vars_id[i]] += x;
                /*
                if (v[y.vars_id[i]] < 0)
                {
                    v[y.vars_id[i]] = 0;
                }
                */
            }
            else
            {
                v[y.vars_id[i]] -= x;
                /*
                if (y.vars_id[i] > 1)
                {
                    v[y.vars_id[i]] = 1;
                }
                */
            }
        }
    }
    void get_bound()
    {
        int i;
        for (i = 0; i < sum_vars; i++)
        {
            if (v[i] < 0)
            {
                v[i] = 0;
            }
            if (v[i] > 1)
            {
                v[i] = 1;
            }
        }
    }
};

vec c;

vec P(vec x, vector<int> &order)
{
    int i;
    for (i = 0; i < (int)bounds.size(); i++)
    {
        int j = order[i];
        double v = x * bounds[j];
        v /= bounds[j].vars.size();
        if (bounds[j].relation != "=")
        {
            v = min(0.0, v);
        }
        x.add(v * sigma, bounds[j]);
    }
    x.get_bound();
    return x;
}
double r(vec x, bool test = false)
{
    double sum = 0, sum2 = 0;
    int i;
    map<string,double> ma; 
    for (i = 0; i < (int)bounds.size(); i++)
    {
        double v = x * bounds[i];
        if (bounds[i].relation != "=")
        {
            v = min(0.0, v);
            sum += v * v;
        }
        else
        {
            sum += v * v;
        }
        if (test)
        {
            string name = bounds[i].name.substr(0,7);
            if ((name != "sub_val") && (name != "op_supp"))
            {
                name=bounds[i].name.substr(0,15);
            }
            ma[name]+=v*v;
        }
    }
    if (test)
    {
        map<string,double>::iterator ii;
        for (ii=ma.begin();ii!=ma.end();ii++)
        {
            printf("%s %.12lf\n",(*ii).first.c_str(),(*ii).second);
        }
    }
    return sum + sum2;
}
vec S(vec x, double la)
{
    // Calculate ga
    static double last_ga = 0.5;
    double ga = last_ga * 2;
    double std = r(x);
    int i;
    vector<int> order;
    for (i = 0; i < (int)bounds.size(); i++)
    {
        order.push_back(i);
    }
    random_shuffle(order.begin(),order.end());
    //return P(x, order);
    for (i = 0 ; i <= 30; i++)
    {
        vec new_x = x + (-ga) * c;
        new_x.get_bound();
        if (c * new_x <= c * x)
        {
            vec new_x_2 = la * P(new_x, order) + (1 - la) * new_x;
            if (r(new_x_2) < std)
            {
                break;
            }
            else
            {
                ga = ga / 2;
            }
        }
        else
        {
            ga = ga / 2;
        }
    }
    last_ga = ga;
    if (i > 30)
    {
        printf("ga = %lf\n", ga);
        return P(x, order);
    }
    
    vec t_x = x + (-ga) * c;
    t_x.get_bound();
    return la * P(t_x, order) + (1 - la) * t_x;
}
const int M = 1000;
vec calc(double th, double ep, double la, double al, double be, vec x)
{
    //return S(x, la);
    //printf("al = %.12lf, be = %.12lf\n", al, be);
    // Step 1
    int i;
    for (i = 0 ; i < sum_vars; i++)
    {
        if (x[i] <= al)
        {
            x[i] = 0;
        }
        else if (x[i] >= be)
        {
            x[i] = 1;
        }
    }
    
    // Step 2
    
    vec y = S(x, la);
    
    // Step 3
    
    for (i = 0; i < sum_vars; i++)
    {
        if ((x[i] <= al) && (y[i] >= th))
        {
            x[i] = th - ep;
        }
        else if ((x[i] >= be) && (y[i] <= th))
        {
            x[i] = th + ep;
        }
        else
        {
            x[i] = y[i];
        }
    }
    
    return x;
}
vec main_algorithm(double la)
{
    vec x;
    int m;
    printf("r(0) = %.12lf, c * x = %.12lf\n",sqrt(r(x, true)), c * x);
    for (m = 1; m <= M; m++)
    {
        double th = 0.5;
        double ep = 0.05;
        double al = (m * 1.0 / M) * th;
        double be = 1 - (m * 1.0 / M) * (1 - th);
        sigma = 0.8;
        x = calc(th, ep, la, al, be, x);
        printf("r(%d) = %.12lf, c * x = %.12lf\n",m, sqrt(r(x, true)), c * x);
    }
    //x.divide();
    return x;
}

void init()
{
    c = vec();
    int i;
    for (i = 0; i < (int)goal.size(); i++)
    {
        c[i] = 1;
    }
    for (i = 0; i < (int)bounds.size(); i++)
    {
        int j;
        for (j = 0; j < (int)bounds[i].vars.size(); j++)
        {
            bounds[i].vars_id.push_back(var_name[bounds[i].vars[j]]);
        }
    }
    /*
    for (i = 0; i < sum_vars; i++)
    {
        bound x;
        x.vars.push_back("Virtual_var_name");
        x.vars_id.push_back(i);
        x.positive.push_back(false);
        x.relation = "<=";
        x.val = 0;
        bounds.push_back(x);
    }
    for (i = 0; i < sum_vars; i++)
    {
        bound x;
        x.vars.push_back("Virtual_var_name");
        x.vars_id.push_back(i);
        x.positive.push_back(true);
        x.relation = "<=";
        x.val = 1;
        bounds.push_back(x);
    }
    */
}

void process()
{
    init();
    vec ans = main_algorithm(1.8);
    int i;
    for (i = 0; i < sum_vars; i ++)
    {
        printf("%.8lf\n", ans[i]);
    }
}
