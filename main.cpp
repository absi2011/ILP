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
using namespace std;

void data_input(string file_name);
void process();
void output_result();

int main()
{
    freopen("output.txt","w",stdout);
    data_input("conv2-cp1=_4m4-plus_52.2s.lp");
    process();
    output_result();
    return 0;
} 
