#pragma once
#include <iostream>
#include <vector>
#include <bitset>
#include <queue>
#include <map>
#include "hash_function.h"

using namespace std;

struct bucket
{
    unsigned short src;
    unsigned short dst;
    short weight;
    unsigned short subs;
    unsigned int idx;
};

class ITeM
{
private:
    int width;
    int f_length;
    int allocated_size;
    int cur;

public:
    bucket *matrix;
    vector<bucket *> variable_array;

    ITeM(int w, int f);
    ITeM(ITeM &it) : width(it.width), f_length(it.f_length), allocated_size(it.allocated_size), cur(0), matrix(it.matrix)
    {
    }
    ~ITeM()
    {
        delete matrix;
        vector<bucket *>::iterator IT = variable_array.begin();
        bucket *tmp;
        for (; IT != variable_array.end(); ++IT)
        {
            tmp = *IT;
            delete tmp;
        }
    }
    void insert(string s1, string s2, short weight);
    void insert(int pos, string s1, string s2, short weight);
    void insert(int pos, unsigned short g1, unsigned short g2, short weight, bool flag);
    void checkFSIT(int num, int pos, unsigned short g1, unsigned short g2, short weight);
    bool findDeepestNode(int num, int pos, unsigned short g1, unsigned short g2, short weight);

    int edgeWeightQuery(string s1, string s2);
    int edgeWeightQuery(int pos, unsigned short g1, unsigned short g2);
    int checkEdgeWeight(int num, int pos, unsigned short g1, unsigned short g2);

    int nodeWeightQuery(string s1, int type);
    int checkNodeWeight(int num, int pos, unsigned short g, int type);

    bool reachabilityQuery(string s1, string s2);
    bool checkReachability(int num, int pos, unsigned short g1, unsigned short g2);
    void getReachabilityNode(int num, int pos, int col, unsigned short g1, vector<unsigned int> &set);

    int getMaxSameLen(unsigned short g1, unsigned short g2);
};

ITeM::ITeM(int w, int f)
{
    width = w;
    f_length = f;
    allocated_size = 131072;
    cur = 0;
    matrix = new bucket[width * width];
    memset(matrix, 0, sizeof(bucket) * width * width);
}

void ITeM::insert(string s1, string s2, short weight)
{
    unsigned int hash1 = (*hfunc[0])((unsigned char *)(s1.c_str()), s1.length());
    unsigned int hash2 = (*hfunc[0])((unsigned char *)(s2.c_str()), s2.length());
    unsigned int tmp = (1 << f_length) - 1;
    unsigned short g1 = hash1 & tmp;
    if (g1 == 0)
        g1 += 1;
    unsigned int h1 = (hash1 >> f_length) % width;
    unsigned short g2 = hash2 & tmp;
    if (g2 == 0)
        g2 += 1;
    unsigned int h2 = (hash2 >> f_length) % width;

    int pos = h1 * width + h2;
    if (matrix[pos].src == 0)
    {
        matrix[pos].src = g1;
        matrix[pos].dst = g2;
        matrix[pos].weight = weight;
        matrix[pos].idx = (f_length << 4) + f_length;
        matrix[pos].subs = 0;
    }
    else
        checkFSIT(-1, pos, g1, g2, weight);
}

void ITeM::insert(int pos, string s1, string s2, short weight)
{
    unsigned int hash1 = (*hfunc[0])((unsigned char *)(s1.c_str()), s1.length());
    unsigned int hash2 = (*hfunc[0])((unsigned char *)(s2.c_str()), s2.length());
    unsigned int tmp = (1 << f_length) - 1;
    unsigned short g1 = hash1 & tmp;
    if (g1 == 0)
        g1 += 1;
    unsigned short g2 = hash2 & tmp;
    if (g2 == 0)
        g2 += 1;
    if (matrix[pos].src == 0)
    {
        matrix[pos].src = g1;
        matrix[pos].dst = g2;
        matrix[pos].weight = weight;
        matrix[pos].idx = (f_length << 4) + f_length;
        matrix[pos].subs = 0;
    }
    else
        checkFSIT(-1, pos, g1, g2, weight);
}

void ITeM::insert(int pos, unsigned short g1, unsigned short g2, short weight, bool flag)
{
    if (matrix[pos].src == 0 || flag == false)
    {
        matrix[pos].src = g1;
        matrix[pos].dst = g2;
        matrix[pos].weight = weight;
        matrix[pos].idx = (f_length << 4) + f_length;
        matrix[pos].subs = 0;
    }
    else
        checkFSIT(-1, pos, g1, g2, weight);
}

void ITeM::checkFSIT(int num, int pos, unsigned short g1, unsigned short g2, short weight)
{
    bucket *op;
    if (num == -1)
        op = matrix;
    else
        op = variable_array[num];
    unsigned short og1 = op[pos].src;
    unsigned short og2 = op[pos].dst;
    int olen1 = op[pos].idx & ((1 << 4) - 1);
    int olen2 = (op[pos].idx >> 4) & ((1 << 4) - 1);
    int len1 = getMaxSameLen(g1, og1);
    int len2 = getMaxSameLen(g2, og2);
    if (len1 < olen1)
    {
        len2 = min(len2, olen2);

        if (cur == allocated_size || variable_array.size() == 0)
        {
            bucket *tmp = new bucket[allocated_size];
            memset(tmp, 0, sizeof(bucket) * allocated_size);
            variable_array.push_back(tmp);
            cur = 0;
        }
        int wp = variable_array.size() - 1;
        bucket *b1 = &variable_array[wp][cur + ((g1 >> len1) & 1)];
        b1->src = g1;
        b1->dst = g2;
        b1->weight = weight;
        b1->idx = (f_length << 4) + f_length;
        b1->subs = 0;
        bucket *b2 = &variable_array[wp][cur + ((og1 >> len1) & 1)];
        b2->src = og1;
        b2->dst = og2;
        b2->weight = op[pos].weight;
        b2->idx = op[pos].idx;
        b2->subs = op[pos].subs;

        op[pos].subs = cur / 2;
        op[pos].weight = 0;
        op[pos].idx = (wp << 8) + (len2 << 4) + len1;
        cur += 2;
        return;
    }
    else if (len2 < olen2)
    {
        len1 = olen1;

        if (cur == allocated_size || variable_array.size() == 0)
        {
            bucket *tmp = new bucket[allocated_size];
            memset(tmp, 0, sizeof(bucket) * allocated_size);
            variable_array.push_back(tmp);
            cur = 0;
        }
        int wp = variable_array.size() - 1;
        bucket *b1 = &variable_array[wp][cur + ((g2 >> len2) & 1)];
        b1->src = g1;
        b1->dst = g2;
        b1->weight = weight;
        b1->idx = (f_length << 4) + f_length;
        b1->subs = 0;
        bucket *b2 = &variable_array[wp][cur + ((og2 >> len2) & 1)];
        b2->src = og1;
        b2->dst = og2;
        b2->weight = op[pos].weight;
        b2->idx = op[pos].idx;
        b2->subs = op[pos].subs;

        op[pos].subs = cur / 2;
        op[pos].weight = 0;
        op[pos].idx = (wp << 8) + (len2 << 4) + len1;
        cur += 2;
        return;
    }
    else if (og1 == g1 && og2 == g2 && op[pos].weight != 0)
    {
        op[pos].weight += weight;
        return;
    }
    else if (op[pos].weight == 0)
    {
        findDeepestNode(num, pos, g1, g2, weight);
        return;
    }
    else
    {
        len1 = olen1;
        len2 = olen2;
        int nnum = op[pos].idx >> 8;
        int npos = op[pos].subs * 2;
        int n1 = variable_array[nnum][npos].idx & ((1 << 4) - 1);
        int n2 = variable_array[nnum][npos + 1].idx & ((1 << 4) - 1);

        if (len1 != n1 && len1 != n2)
        {
            checkFSIT(nnum, npos + ((g1 >> olen1) & 1), g1, g2, weight);
        }
        else
        {
            checkFSIT(nnum, npos + ((g2 >> olen2) & 1), g1, g2, weight);
        }
        return;
    }
}

bool ITeM::findDeepestNode(int num, int pos, unsigned short g1, unsigned short g2, short weight)
{
    bucket *op;
    if (num == -1)
        op = matrix;
    else
        op = variable_array[num];
    unsigned short og1 = op[pos].src;
    unsigned short og2 = op[pos].dst;
    int olen1 = op[pos].idx & ((1 << 4) - 1);
    int olen2 = (op[pos].idx >> 4) & ((1 << 4) - 1);
    int len1 = getMaxSameLen(g1, og1);
    int len2 = getMaxSameLen(g2, og2);
    if (len1 < olen1 || len2 < olen2)
    {
        return false;
    }
    else if (og1 == g1 && og2 == g2 && op[pos].weight != 0)
    {
        op[pos].weight += weight;
        return true;
    }
    else
    {
        len1 = olen1;
        len2 = olen2;
        int nnum = op[pos].idx >> 8;
        int npos = op[pos].subs * 2;

        int n1 = variable_array[nnum][npos].idx & ((1 << 4) - 1);
        int n2 = variable_array[nnum][npos + 1].idx & ((1 << 4) - 1);

        if ((len1 != n1 && len1 != n2) && findDeepestNode(nnum, npos + ((g1 >> olen1) & 1), g1, g2, weight))
        {
            return true;
        }
        else if ((len1 == n1 || len1 == n2) && findDeepestNode(nnum, npos + ((g2 >> olen2) & 1), g1, g2, weight))
        {
            return true;
        }
        else if (op[pos].weight == 0)
        {
            op[pos].src = g1;
            op[pos].dst = g2;
            op[pos].weight = weight;
            return true;
        }
        else
            return false;
    }
}

int ITeM::edgeWeightQuery(string s1, string s2)
{
    unsigned int hash1 = (*hfunc[0])((unsigned char *)(s1.c_str()), s1.length());
    unsigned int hash2 = (*hfunc[0])((unsigned char *)(s2.c_str()), s2.length());
    unsigned int tmp = (1 << f_length) - 1;
    unsigned short g1 = hash1 & tmp;
    if (g1 == 0)
        g1 += 1;
    unsigned int h1 = (hash1 >> f_length) % width;
    unsigned short g2 = hash2 & tmp;
    if (g2 == 0)
        g2 += 1;
    unsigned int h2 = (hash2 >> f_length) % width;

    int pos = h1 * width + h2;
    if (matrix[pos].src == 0)
    {
        return 0;
    }
    else if (matrix[pos].src == g1 && matrix[pos].dst == g2 && matrix[pos].weight != 0)
        return matrix[pos].weight;
    else
        return checkEdgeWeight(-1, pos, g1, g2);
}

int ITeM::edgeWeightQuery(int pos, unsigned short g1, unsigned short g2)
{
    if (matrix[pos].src == 0)
    {
        return 0;
    }
    else if (matrix[pos].src == g1 && matrix[pos].dst == g2 && matrix[pos].weight != 0)
        return matrix[pos].weight;
    else
        return checkEdgeWeight(-1, pos, g1, g2);
}

int ITeM::checkEdgeWeight(int num, int pos, unsigned short g1, unsigned short g2)
{
    bucket *op;
    if (num == -1)
        op = matrix;
    else
        op = variable_array[num];
    unsigned short og1 = op[pos].src;
    unsigned short og2 = op[pos].dst;
    int olen1 = op[pos].idx & ((1 << 4) - 1);
    int olen2 = (op[pos].idx >> 4) & ((1 << 4) - 1);
    int len1 = getMaxSameLen(g1, og1);
    int len2 = getMaxSameLen(g2, og2);
    if (len1 < olen1 || len2 < olen2)
    {
        return 0;
    }
    else if (og1 == 0)
    {
        return 0;
    }
    else if (og1 == g1 && og2 == g2 && op[pos].weight != 0)
    {
        return op[pos].weight;
    }
    else
    {
        len1 = olen1;
        len2 = olen2;
        int nnum = op[pos].idx >> 8;
        int npos = op[pos].subs * 2;

        int n1 = variable_array[nnum][npos].idx & ((1 << 4) - 1);
        int n2 = variable_array[nnum][npos + 1].idx & ((1 << 4) - 1);

        if (len1 != n1 && len1 != n2)
        {
            return checkEdgeWeight(nnum, npos + ((g1 >> olen1) & 1), g1, g2);
        }
        else
        {
            return checkEdgeWeight(nnum, npos + ((g2 >> olen2) & 1), g1, g2);
        }
    }
}

int ITeM::nodeWeightQuery(string s1, int type)
{
    unsigned int hash = (*hfunc[0])((unsigned char *)(s1.c_str()), s1.length());
    unsigned int tmp = (1 << f_length) - 1;
    unsigned short g = hash & tmp;
    if (g == 0)
        g += 1;
    unsigned int h = (hash >> f_length) % width;
    int res = 0;

    if (type == 0)
    {
        for (int i = 0; i < width; i++)
        {
            int pos = h * width + i;
            res += checkNodeWeight(-1, pos, g, type);
        }
    }
    else
    {
        for (int i = 0; i < width; i++)
        {
            int pos = i * width + h;
            res += checkNodeWeight(-1, pos, g, type);
        }
    }
    return res;
}

int ITeM::checkNodeWeight(int num, int pos, unsigned short g, int type)
{
    bucket *op;
    if (num == -1)
        op = matrix;
    else
        op = variable_array[num];
    int res = 0;
    if (type == 0)
    {
        unsigned short og1 = op[pos].src;
        int olen1 = op[pos].idx & ((1 << 4) - 1);
        int olen2 = (op[pos].idx >> 4) & ((1 << 4) - 1);
        int len1 = getMaxSameLen(g, og1);
        if (len1 < olen1 || og1 == 0)
        {
            return 0;
        }
        if (og1 == g)
        {
            res += op[pos].weight;
        }
        if (olen1 != f_length || olen2 != f_length)
        {
            int nnum = op[pos].idx >> 8;
            int npos = op[pos].subs * 2;
            res += checkNodeWeight(nnum, npos, g, type);
            res += checkNodeWeight(nnum, npos + 1, g, type);
        }
    }
    else
    {
        unsigned short og2 = op[pos].dst;
        int olen1 = op[pos].idx & ((1 << 4) - 1);
        int olen2 = (op[pos].idx >> 4) & ((1 << 4) - 1);
        int len2 = getMaxSameLen(g, og2);
        if (len2 < olen2 || og2 == 0)
        {
            return 0;
        }
        if (og2 == g)
        {
            res += op[pos].weight;
        }
        if (olen1 != f_length || olen2 != f_length)
        {
            int nnum = op[pos].idx >> 8;
            int npos = op[pos].subs * 2;
            res += checkNodeWeight(nnum, npos, g, type);
            res += checkNodeWeight(nnum, npos + 1, g, type);
        }
    }
    return res;
}

bool ITeM::reachabilityQuery(string s1, string s2)
{
    unsigned int hash1 = (*hfunc[0])((unsigned char *)(s1.c_str()), s1.length());
    unsigned int hash2 = (*hfunc[0])((unsigned char *)(s2.c_str()), s2.length());
    unsigned int tmp = (1 << f_length) - 1;
    unsigned short g1 = hash1 & tmp;
    if (g1 == 0)
        g1 += 1;
    unsigned int h1 = (hash1 >> f_length) % width;
    unsigned short g2 = hash2 & tmp;
    if (g2 == 0)
        g2 += 1;
    unsigned int h2 = (hash2 >> f_length) % width;
    int pos;
    map<unsigned int, bool> checked;
    queue<unsigned int> q;
    unsigned int e = (h1 << f_length) + g1;
    q.push(e);
    checked[e] = true;
    map<unsigned int, bool>::iterator IT;

    while (!q.empty())
    {
        e = q.front();
        h1 = e >> f_length;
        g1 = e % (1 << f_length);
        int pos = h1 * width + h2;
        if (checkReachability(-1, pos, g1, g2))
            return true;
        vector<unsigned int> nodeset;
        for (int i = 0; i < width; i++)
        {
            pos = h1 * width + i;
            getReachabilityNode(-1, pos, i, g1, nodeset);
        }

        for (int i = 0; i < nodeset.size(); i++)
        {
            IT = checked.find(nodeset[i]);
            if (IT == checked.end())
            {
                q.push(nodeset[i]);
                checked[nodeset[i]] = true;
            }
        }
        q.pop();
    }
    return false;
}

bool ITeM::checkReachability(int num, int pos, unsigned short g1, unsigned short g2)
{
    bucket *op;
    if (num == -1)
        op = matrix;
    else
        op = variable_array[num];
    unsigned short og1 = op[pos].src;
    unsigned short og2 = op[pos].dst;
    int olen1 = op[pos].idx & ((1 << 4) - 1);
    int olen2 = (op[pos].idx >> 4) & ((1 << 4) - 1);
    int len1 = getMaxSameLen(g1, og1);
    int len2 = getMaxSameLen(g2, og2);
    if (len1 < olen1 || len2 < olen2)
    {
        return false;
    }
    else if (og1 == 0)
    {
        return false;
    }
    else if (og1 == g1 && og2 == g2 && op[pos].weight != 0)
    {
        return true;
    }
    else
    {
        len1 = olen1;
        len2 = olen2;
        int nnum = op[pos].idx >> 8;
        int npos = op[pos].subs * 2;
        int n1 = variable_array[nnum][npos].idx & ((1 << 4) - 1);
        int n2 = variable_array[nnum][npos + 1].idx & ((1 << 4) - 1);
        if (len1 != n1 && len1 != n2)
        {
            return checkReachability(nnum, npos + ((g1 >> olen1) & 1), g1, g2);
        }
        else
        {
            return checkReachability(nnum, npos + ((g2 >> olen2) & 1), g1, g2);
        }
    }
}

void ITeM::getReachabilityNode(int num, int pos, int col, unsigned short g1, vector<unsigned int> &set)
{
    bucket *op;
    if (num == -1)
        op = matrix;
    else
        op = variable_array[num];
    unsigned short og1 = op[pos].src;
    unsigned short og2 = op[pos].dst;
    int olen1 = op[pos].idx & ((1 << 4) - 1);
    int olen2 = (op[pos].idx >> 4) & ((1 << 4) - 1);
    int len1 = getMaxSameLen(g1, og1);
    if (len1 < olen1 || og1 == 0)
    {
        return;
    }
    if (og1 == g1 && op[pos].weight != 0)
    {
        unsigned int tmpk = (col << f_length) + og2;
        set.push_back(tmpk);
    }
    if (olen1 != f_length || olen2 != f_length)
    {
        int nnum = op[pos].idx >> 8;
        int npos = op[pos].subs * 2;
        getReachabilityNode(nnum, npos, col, g1, set);
        getReachabilityNode(nnum, npos + 1, col, g1, set);
    }
    return;
}

int ITeM::getMaxSameLen(unsigned short g1, unsigned short g2)
{
    int ret = 0;
    for (int i = 0; i < f_length; i++)
    {
        if (((g1 >> i) & 1) == ((g2 >> i) & 1))
        {
            ret++;
        }
        else
            break;
    }
    return ret;
}
