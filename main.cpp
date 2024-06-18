#include "function.h"
#include "sliding_function.h"
using namespace std;

int main(int argc, char **argv)
{
    string dataset = "wiki_talk_en";
    int width = 1000, fpl = 12, ts = 0, size = 30;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-dataset") == 0)
        {
            dataset = argv[++i];
        }
        if (strcmp(argv[i], "-fpl") == 0)
        {
            fpl = atoi(argv[++i]);
        }
        if (strcmp(argv[i], "-width") == 0)
        {
            width = atoi(argv[++i]);
        }
        if (strcmp(argv[i], "-ts") == 0)
        {
            ts = atoi(argv[++i]);
        }
        if (strcmp(argv[i], "-size") == 0)
        {
            size = atoi(argv[++i]);
        }
    }

    string data = "./dataset/" + dataset + "/out." + dataset + ".txt";
    string edge = "./dataset/" + dataset + "/in." + dataset + ".txt";
    string nodeOut = "./dataset/" + dataset + "/in.node1." + dataset + ".txt";
    string nodeIn = "./dataset/" + dataset + "/in.node2." + dataset + ".txt";
    string sdata = "./dataset/" + dataset + "/sliding/";

    if (ts == 0)
    {
        auto test = new ITeM(width, fpl);
        insertData(test, data);
        edgeQuery(test, edge);
        nodeQuery(test, nodeOut, 0);
        nodeQuery(test, nodeIn, 1);
    }
    else
    {
        auto test = new SITeM(size, width, fpl);
        slidingQuery(test, sdata, dataset);
    }

    return 0;
}