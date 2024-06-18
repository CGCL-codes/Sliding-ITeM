#include <sys/time.h>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <random>
#include <iomanip>
#include "sliding_ITeM.h"

using namespace std;

double sum_itime = 0;        // insert sum time
unsigned int sum_idata = 0;  // insert data num
double max_itime = 0;        // insert max time
unsigned int max_idata = 0;  // insert max sum
double sum_etime = 0;        // edge qurey sum time
unsigned int sum_edata = 0;  // edge query data sum
double max_etime = 0;        // edge qurey max time
unsigned int max_edata = 0;  // edge query max sum
double sum_sntime = 0;       // src node qurey sum time
unsigned int sum_sndata = 0; // src node query data sum
double max_sntime = 0;       // src node qurey max time
unsigned int max_sndata = 0; // src node query max sum
double sum_dntime = 0;       // dst node qurey sum time
unsigned int sum_dndata = 0; // dst node query data sum
double max_dntime = 0;       // dst node qurey max time
unsigned int max_dndata = 0; // dst node query max sum
double sum_ef = 0;
double sum_snf = 0;
double sum_dnf = 0;

template <typename T>
void slidingQuery(T *st, string sdata, string filename)
{
    timeval tot_start, tot_end;
    gettimeofday(&tot_start, NULL);
    int data_size = 179;
    for (int i = 0; i < data_size; i++)
    {
        string file = sdata + "insertData/out." + filename + "." + to_string(i) + ".txt";
        string efile = sdata + "edgeQueryData/in." + filename + "." + to_string(i) + ".txt";
        string snfile = sdata + "srcNodeQueryData/snode." + filename + "." + to_string(i) + ".txt";
        string dnfile = sdata + "dstNodeQueryData/dnode." + filename + "." + to_string(i) + ".txt";
        slidingInsertData(st, file, i);
        if (i % 5 == 4)
        {
            slidingEdgeQuery(st, efile);
            slidingSrcNodeQuery(st, snfile);
            slidingDstNodeQuery(st, dnfile);
        }
    }
    cout << "Insertion Time: " << fixed << setprecision(3) << sum_itime / double(sum_idata) << " μs" << endl;
    cout << "Insertion data: " << fixed << setprecision(0) << sum_idata << endl;
    cout << "Insertion MAXTime: " << fixed << setprecision(3) << max_itime << " μs" << endl;
    cout << "Insertion MAXdata: " << fixed << setprecision(0) << max_idata << endl
         << endl;

    cout << "EdgeQuery Time: " << fixed << setprecision(3) << sum_etime / double(sum_edata) << " μs" << endl;
    cout << "EdgeQuery data: " << fixed << setprecision(0) << sum_edata << endl;
    cout << "EdgeQuery MAXTime: " << fixed << setprecision(3) << max_etime << " μs" << endl;
    cout << "EdgeQuery MAXdata: " << fixed << setprecision(0) << max_edata << endl
         << endl;

    cout << "SrcNodeQuery Time: " << fixed << setprecision(3) << sum_sntime / double(sum_sndata) << " μs" << endl;
    cout << "SrcNodeQuery data: " << fixed << setprecision(0) << sum_sndata << endl;
    cout << "SrcNodeQuery MAXTime: " << fixed << setprecision(3) << max_sntime << " μs" << endl;
    cout << "SrcNodeQuery MAXdata: " << fixed << setprecision(0) << max_sndata << endl
         << endl;

    cout << "DstNodeQuery Time: " << fixed << setprecision(3) << sum_dntime / double(sum_dndata) << " μs" << endl;
    cout << "DstNodeQuery data: " << fixed << setprecision(0) << sum_dndata << endl;
    cout << "DstNodeQuery MAXTime: " << fixed << setprecision(3) << max_dntime << " μs" << endl;
    cout << "DstNodeQuery MAXdata: " << fixed << setprecision(0) << max_dndata << endl
         << endl;

    sum_ef /= double(sum_edata);
    cout << "ARE: " << fixed << setprecision(8) << sum_ef << endl;
    sum_snf /= double(sum_sndata);
    cout << "ARE: " << fixed << setprecision(8) << sum_snf << endl;
    sum_dnf /= double(sum_dndata);
    cout << "ARE: " << fixed << setprecision(8) << sum_dnf << endl;
    gettimeofday(&tot_end, NULL);
    double tot_time = (tot_end.tv_sec - tot_start.tv_sec) * 1000000.0 + (tot_end.tv_usec - tot_start.tv_usec);
    cout << "Total Time: " << fixed << setprecision(0) << tot_time << " μs" << endl;
}

template <typename T>
void slidingInsertData(T *st, string dataset, int i)
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
    timeval cur_start, cur_end;
    int n = 0;
    gettimeofday(&cur_start, NULL);
    while (!ifs.eof())
    {
        n++;
        ifs >> s >> d >> w;
        if (w == 0)
            w = 1;
        st->insert(to_string(s), to_string(d), w, i);
    }
    gettimeofday(&cur_end, NULL);
    double cur_time = (cur_end.tv_sec - cur_start.tv_sec) * 1000000.0 + (cur_end.tv_usec - cur_start.tv_usec);
    double ave_time = cur_time / (double)(n);
    sum_idata += n;
    sum_itime += cur_time;
    if (max_itime < ave_time && n > 10000)
    {
        max_itime = ave_time;
        max_idata = n;
    }
}

template <typename T>
void slidingEdgeQuery(T *st, string input)
{
    double sum = 0;
    int n = 0;
    ifstream ifs;
    ifs.open(input);
    if (!ifs.is_open())
    {
        cout << "Error in open file, Path = " << input << endl;
        return;
    }
    int64_t s, d;
    int w;
    timeval cur_start, cur_end;
    gettimeofday(&cur_start, NULL);
    while (!ifs.eof())
    {
        n++;
        ifs >> s >> d >> w;
        int res = st->edgeWeightQuery(to_string(s), to_string(d));
        sum += abs(res - w) / double(w);
    }
    gettimeofday(&cur_end, NULL);
    double cur_time = (cur_end.tv_sec - cur_start.tv_sec) * 1000000.0 + (cur_end.tv_usec - cur_start.tv_usec);
    double ave_time = cur_time / (double)(n);
    if (ave_time > max_etime)
    {
        max_etime = ave_time;
        max_edata = n;
    }
    sum_ef += sum;
    sum_etime += cur_time;
    sum_edata += n;
}

template <typename T>
void slidingSrcNodeQuery(T *st, string input)
{
    double sum = 0;
    int n = 0;
    ifstream ifs;
    ifs.open(input);
    if (!ifs.is_open())
    {
        cout << "Error in open file, Path = " << input << endl;
        return;
    }
    int64_t s;
    int w;
    timeval cur_start, cur_end;
    gettimeofday(&cur_start, NULL);
    while (!ifs.eof())
    {
        n++;
        ifs >> s >> w;
        int res = st->nodeWeightQuery(to_string(s), 0);
        sum += abs(res - w) / double(w);
    }
    gettimeofday(&cur_end, NULL);
    double cur_time = (cur_end.tv_sec - cur_start.tv_sec) * 1000000.0 + (cur_end.tv_usec - cur_start.tv_usec);
    double ave_time = cur_time / (double)(n);
    if (ave_time > max_sntime)
    {
        max_sntime = ave_time;
        max_sndata = n;
    }
    sum_snf += sum;
    sum_sntime += cur_time;
    sum_sndata += n;
}

template <typename T>
void slidingDstNodeQuery(T *st, string input)
{
    double sum = 0;
    int n = 0;
    ifstream ifs;
    ifs.open(input);
    if (!ifs.is_open())
    {
        cout << "Error in open file, Path = " << input << endl;
        return;
    }
    int64_t s;
    int w;
    timeval cur_start, cur_end;
    gettimeofday(&cur_start, NULL);
    while (!ifs.eof())
    {
        n++;
        ifs >> s >> w;
        int res = st->nodeWeightQuery(to_string(s), 1);
        sum += abs(res - w) / double(w);
    }
    gettimeofday(&cur_end, NULL);
    double cur_time = (cur_end.tv_sec - cur_start.tv_sec) * 1000000.0 + (cur_end.tv_usec - cur_start.tv_usec);
    double ave_time = cur_time / (double)(n);
    if (ave_time > max_dntime)
    {
        max_dntime = ave_time;
        max_dndata = n;
    }
    sum_dnf += sum;
    sum_dntime += cur_time;
    sum_dndata += n;
}
