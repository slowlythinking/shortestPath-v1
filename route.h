#ifndef __ROUTE_H__ #define __ROUTE_H__
#include <vector>
#include <limits.h>

void search_route(char *graph[5000], int edge_num, char *condition);

struct FirstCell
{
    int num;
    int weight;
    int isOrNotPassVertex;
    FirstCell()
    {
	num = -1;
	weight = INT_MAX;
	isOrNotPassVertex = 0;
    }
};


struct SecondCell
{
    int weight;
    std::vector<int> path;
    SecondCell()
    {
	weight = INT_MAX;
    }
};

//struct minPathCell
//{
//    int start;
//    int end;
//    int H;
//    int L;
//}

//struct nodeCell
//{
//    int x;
//    int y;
//    nodeCell()
//    {
//	x = 0;
//	y = 0;
//    }
//};

#endif
