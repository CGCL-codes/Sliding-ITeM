#include "ITeM.h"
#include "thread_pool.h"
using namespace std;

class SITeM
{
private:
    int w;
    int f;
    int cur_time;
    int window_size;
    vector<ITeM *> idata;
    vector<int> itime;
    vector<vector<bool>> imap;
    ThreadPool *pool;

public:
    SITeM(int wsize, int width, int f_len);
    ~SITeM()
    {
        for (auto it = idata.begin(); it != idata.end(); it++)
        {
            if ((*it) != nullptr)
            {
                delete (*it);
            }
        }
    }
    void insert(string s1, string s2, int weight, unsigned int time);
    int edgeWeightQuery(string s1, string s2);
    int nodeWeightQuery(string s1, int type);
    void getNodeInfo(string s, unsigned int &h, unsigned short &g);
    static void deleteITeM(ITeM *it)
    {
        delete it;
    }
};

SITeM::SITeM(int wsize, int width, int f_len)
{
    w = width;
    f = f_len;
    cur_time = -1;
    window_size = wsize;
    pool = new ThreadPool(1);
    pool->init();
}

void SITeM::getNodeInfo(string s, unsigned int &h, unsigned short &g)
{
    unsigned int hash = (*hfunc[0])((unsigned char *)(s.c_str()), s.length());
    unsigned int tmp = (1 << f) - 1;
    g = hash & tmp;
    if (g == 0)
        g += 1;
    h = (hash >> f) % w;
    return;
}

void SITeM::insert(string s1, string s2, int weight, unsigned int time)
{
    if (time != cur_time)
    {
        cur_time = time;
        if (idata.size() == window_size)
        {
            auto exi = idata.front();
            auto tmp = new ITeM(*exi);
            idata.erase(idata.begin());
            idata.push_back(tmp);
            exi->matrix = nullptr;
            pool->submit(deleteITeM, exi);
            itime.erase(itime.begin());
            itime.push_back(cur_time);
            imap.erase(imap.begin());
            vector<bool> tmpmap(w * w, false);
            imap.push_back(tmpmap);
        }
        else
        {
            auto tmp = new ITeM(w, f);
            idata.push_back(tmp);
            itime.push_back(cur_time);
            vector<bool> tmpmap(w * w, false);
            imap.push_back(tmpmap);
        }
    }
    unsigned int h1, h2;
    unsigned short g1, g2;
    getNodeInfo(s1, h1, g1);
    getNodeInfo(s2, h2, g2);
    int pos = h1 * w + h2;
    idata.back()->insert(pos, g1, g2, weight, imap.back()[pos]);
    imap.back()[pos] = true;
}

int SITeM::edgeWeightQuery(string s1, string s2)
{
    int ret = 0;
    unsigned int h1, h2;
    unsigned short g1, g2;
    getNodeInfo(s1, h1, g1);
    getNodeInfo(s2, h2, g2);
    int pos = h1 * w + h2;
    for (int i = 0; i < idata.size(); i++)
    {
        auto it = idata[i];
        if (it != nullptr && imap[i][pos])
        {

            ret += it->edgeWeightQuery(pos, g1, g2);
        }
    }
    return ret;
}

int SITeM::nodeWeightQuery(string s1, int type)
{
    int ret = 0;
    unsigned int h1;
    unsigned short g1;
    getNodeInfo(s1, h1, g1);
    for (int i = 0; i < idata.size(); i++)
    {
        auto it = idata[i];
        if (it != nullptr)
        {
            if (type == 0)
            {
                for (int k = 0; k < w; k++)
                {
                    int pos = h1 * w + k;
                    if (imap[i][pos])
                        ret += it->checkNodeWeight(-1, pos, g1, type);
                }
            }
            else
            {
                for (int k = 0; k < w; k++)
                {
                    int pos = k * w + h1;
                    if (imap[i][pos])
                        ret += it->checkNodeWeight(-1, pos, g1, type);
                }
            }
        }
    }
    return ret;
}