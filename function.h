#pragma once
#include <sys/time.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "ITeM.h"
using namespace std;

template <typename T>
void insertData(T *st, string dataset)
{
    ifstream ifs;
    ifs.open(dataset);
    if (!ifs.is_open())
    {
        cout << "Error in open file, Path = " << dataset << endl;
        return;
    }
    int64_t s, d;
    int w;
    unsigned int t;
    int n = 0;
    cout << "Insertion starts" << endl
         << endl;

    timeval tot_start, tot_end;
    gettimeofday(&tot_start, NULL);

    while (!ifs.eof())
    {
        n++;
        ifs >> s >> d >> w >> t;
        if (w == 0)
            w = 1;
        st->insert(to_string(s), to_string(d), w);
    }
    gettimeofday(&tot_end, NULL);
    double ins_time = (tot_end.tv_sec - tot_start.tv_sec) * 1000000.0 + (tot_end.tv_usec - tot_start.tv_usec);
    cout << "Total Time: " << fixed << setprecision(0) << ins_time << " μs" << endl;
    cout << "Average Execution Time: " << fixed << setprecision(3) << ins_time / n << " μs" << endl;
    cout << endl
         << "Insertion Finished" << endl
         << endl;
}

template <typename T>
void edgeQuery(T *st, string input)
{
    ifstream ifs;
    ifs.open(input);
    if (!ifs.is_open())
    {
        cout << "Error in open file, Path = " << input << endl;
        return;
    }
    int64_t s, d;
    int w;
    unsigned int t;
    int n = 0;
    double sum = 0;
    cout << "EdgeQuery starts" << endl
         << endl;
    timeval tot_start, tot_end;
    gettimeofday(&tot_start, NULL);
    while (!ifs.eof())
    {
        n++;
        ifs >> s >> d >> w;
        int res = st->edgeWeightQuery(to_string(s), to_string(d));
        sum += abs(res - w) / double(w);
    }
    gettimeofday(&tot_end, NULL);
    double ins_time = (tot_end.tv_sec - tot_start.tv_sec) * 1000000.0 + (tot_end.tv_usec - tot_start.tv_usec);
    sum = sum / (double)n;
    cout << "Total Time: " << fixed << setprecision(0) << ins_time << " μs" << endl;
    cout << "Average Execution Time: " << fixed << setprecision(3) << ins_time / n << " μs" << endl;
    cout << "ARE: " << fixed << setprecision(8) << sum << endl
         << endl;
    cout << "EdgeQuery Finished" << endl
         << endl;
}

template <typename T>
void nodeQuery(T *st, string input, int type)
{
    ifstream ifs;
    ifs.open(input);
    if (!ifs.is_open())
    {
        cout << "Error in open file, Path = " << input << endl;
        return;
    }
    int64_t s, d;
    int w;
    unsigned int t;
    int n = 0;
    double sum = 0;
    cout << "NodeQuery starts" << endl
         << endl;
    timeval tot_start, tot_end;
    gettimeofday(&tot_start, NULL);
    while (!ifs.eof() && n < 10000)
    {
        n++;
        ifs >> s >> w;
        int res = st->nodeWeightQuery(to_string(s), type);
        sum += abs(res - w) / double(w);
    }
    gettimeofday(&tot_end, NULL);
    double ins_time = (tot_end.tv_sec - tot_start.tv_sec) * 1000000.0 + (tot_end.tv_usec - tot_start.tv_usec);
    sum = sum / (double)n;
    cout << "Total Time: " << fixed << setprecision(0) << ins_time << " μs" << endl;
    cout << "Average Execution Time: " << fixed << setprecision(3) << ins_time / n << " μs" << endl;
    cout << "ARE: " << fixed << setprecision(8) << sum << endl
         << endl;
    cout << "NodeQuery Finished" << endl
         << endl;
}
