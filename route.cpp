#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

//你要完成的功能总入口

//dfs算法
int findPathWeight = INT_MAX;
int recordPathWeight;
vector<int> recordPath;
vector<int> findPath;
int jishuT;

void dfs(int current,int *jishu,int must_pass_len,int visitedH[],int visitedL[],SecondCell pvG[][50],SecondCell endPath[])
{
    if((int)(recordPath.size()) == must_pass_len && (endPath[current].weight < INT_MAX))
    {
	if (findPathWeight > (recordPathWeight + endPath[current].weight))
	{
	    findPathWeight = recordPathWeight + endPath[current].weight;
	    findPath = recordPath;
	    jishuT = (*jishu);
	}
    }
    else
    {
	for(int i = 0;i != must_pass_len;i++)
	{
	    if (visitedH[i] == 0 && visitedL[i] == 0 && (pvG[current][i].weight < INT_MAX))
	    {
		visitedH[current] = 1;
		visitedL[i] = 1;
		recordPath.push_back(i);
		recordPathWeight += pvG[current][i].weight;

		dfs(i,jishu,must_pass_len,visitedH,visitedL,pvG,endPath);

		visitedH[current] = 0;
		visitedL[i] = 0;
		recordPath.pop_back();
		recordPathWeight -= pvG[current][i].weight;

	    }
	}
    }
}


//Dijkstra算法
void Dijkstra(const int numOfVertex,const int startVertex,const int endVertex,FirstCell vG[][600],int *distance,int *prevVertex)  
{  
    vector<bool> isInS;                 //是否已经在S集合中  
    isInS.reserve(0);  
    isInS.assign(numOfVertex, false);   //初始化，所有的节点都不在S集合中  
      
    for(int i =0; i < numOfVertex; ++i)  
    {  
	if (vG[startVertex][i].isOrNotPassVertex == 1 && i != endVertex)
	    distance[i] = INT_MAX;
	else
	    distance[i] = vG[startVertex][i].weight;  
        if(distance[i] < INT_MAX)  
            prevVertex[i] = startVertex;  
        else  
            prevVertex[i] = -1;       //表示还不知道前一个节点是什么  
    }  
    prevVertex[startVertex] = -1;  
      
    isInS[startVertex] = true;          //开始节点放入S集合中  
      
    int u = startVertex;  
      
    for (int i = 1; i < numOfVertex; i ++)      //这里循环从1开始是因为开始节点已经存放在S中了，还有numOfVertex-1个节点要处理  
    {  
        int nextVertex = u;  
        int tempDistance = INT_MAX;  
        for(int j = 0; j < numOfVertex; ++j)  
        {  
            if((isInS[j] == false) && (distance[j] < tempDistance))//寻找不在S集合中的distance最小的节点  
            {  
                nextVertex = j;  
                tempDistance = distance[j];  
            }  
        }  
        isInS[nextVertex] = true;//放入S集合中  
        u = nextVertex;//下一次寻找的开始节点  
          
        for (int j =0; j < numOfVertex; j ++)  
	{  
	    if ((isInS[j] == false) && (vG[u][j].weight < INT_MAX))  
	    {  
		if ((vG[u][j].isOrNotPassVertex == 1) && (j != endVertex))
		    ;
		else
		{
		    int temp = distance[ u ] + vG[u][j].weight;  
		    if (temp < distance[ j ])  
		    {  
			distance[ j ] = temp;  
			prevVertex[ j ] = u;  
		    }  
		    if (j == endVertex)
			return;
		}
            }  
        }  
    }  
      
      
      
}  

void search_route(char *topo[5000], int edge_num, char *demand)
{
    FirstCell vG[600][600];
    int vertex_num=-1;
    vector<int> must_pass_vertex;

   //初始化demand数据
    int start=atoi(strtok(demand,",|"));
    int end=atoi(strtok(NULL,",|"));
    char *tempp;
    must_pass_vertex.push_back(start);
    while(tempp=strtok(NULL,",|"))
	must_pass_vertex.push_back(atoi(tempp));
    must_pass_vertex.push_back(end);

   //建立只包含demand的图
   int must_pass_len;
   size_t len = must_pass_vertex.size();
   must_pass_len= (int)len-2;
   int visitedH[must_pass_len];
   int visitedL[must_pass_len];
   SecondCell pvG[must_pass_len][50];
   SecondCell startPath[must_pass_len];
   SecondCell endPath[must_pass_len];

    //初始化图数据存到邻接矩阵vG中
   for(int i = 0; i < edge_num; i++)
    {
	int pathnum=atoi(strtok(topo[i],","));
	int startpoint=atoi(strtok(NULL,","));
	int endpoint=atoi(strtok(NULL,","));
	int pathweigth=atoi(strtok(NULL,","));
	if (vertex_num < startpoint)
	    vertex_num = startpoint;
	if (vertex_num < endpoint)
	    vertex_num = endpoint;
	if (pathweigth < vG[startpoint][endpoint].weight)
	{
	    vG[startpoint][endpoint].num = pathnum;
	    vG[startpoint][endpoint].weight = pathweigth;
	}
    }
   vertex_num++;

//所有以必经节点为出口的边中isOrNotPassVertex均设为1
   for(size_t iter = 0;iter != len;iter++)
   {
       int temp = must_pass_vertex[iter];
       for (int i = 0;i < vertex_num;i++)
       {
	   vG[i][temp].isOrNotPassVertex = 1;
       }
   }

   //填充pvG数据结构
   int pvG_i = 0;
   int endPathnum = 0;
   int startPathnum = 0;
   for(size_t iter = 0;iter != len-1;iter++)
   {
       int pvG_j = 0;
       for(size_t itertemp = 1;itertemp != len;itertemp++)
       {

	   int pass_vertex_start = must_pass_vertex[iter];
	   int pass_vertex_end = must_pass_vertex[itertemp];
	   int distance[vertex_num];
	   int preVertex[vertex_num];
	   Dijkstra(vertex_num,pass_vertex_start,pass_vertex_end,vG,distance,preVertex);
	   int pass_index = must_pass_vertex[itertemp];

	   if (iter == 0 && itertemp != len -1)
	   {
	       if(vG[pass_vertex_start][pass_vertex_end].weight > distance[pass_index])
	       {
		   startPath[startPathnum].weight = distance[pass_index];
		   while (preVertex[pass_index] != -1)
		   {
		       (startPath[startPathnum].path).push_back(vG[preVertex[pass_index]][pass_index].num);
		       pass_index = preVertex[pass_index];
		   }
	       }
	       else if(vG[pass_vertex_start][pass_vertex_end].weight < distance[pass_index] ||(vG[pass_vertex_start][pass_vertex_end].weight == distance[pass_index] && distance[pass_index]!= INT_MAX))
	       {
		   startPath[startPathnum].weight = vG[pass_vertex_start][pass_vertex_end].weight;
		   startPath[startPathnum].path.push_back(vG[pass_vertex_start][pass_vertex_end].num);
	       }
	       startPathnum++;
	   }

	   else if(iter != 0 && itertemp == len -1)
	   {
	       if(vG[pass_vertex_start][pass_vertex_end].weight > distance[pass_index])
	       {
		   endPath[endPathnum].weight = distance[pass_index];
		   while (preVertex[pass_index] != -1)
		   {
		       (endPath[endPathnum].path).push_back(vG[preVertex[pass_index]][pass_index].num);
		       pass_index = preVertex[pass_index];
		   }
	       }
	       else if(vG[pass_vertex_start][pass_vertex_end].weight < distance[pass_index] ||(vG[pass_vertex_start][pass_vertex_end].weight == distance[pass_index] && distance[pass_index]!= INT_MAX))
	       {
		   endPath[endPathnum].weight = vG[pass_vertex_start][pass_vertex_end].weight;
		   endPath[endPathnum].path.push_back(vG[pass_vertex_start][pass_vertex_end].num);

	       }
	       endPathnum++;

	   }

	   else if(iter == 0 && itertemp == len - 1)
	   {}

	   else
	   {
	       if(vG[pass_vertex_start][pass_vertex_end].weight > distance[pass_index])
	       {
		   pvG[pvG_i][pvG_j].weight = distance[pass_index];
		   while (preVertex[pass_index] != -1)
		   {
		       (pvG[pvG_i][pvG_j].path).push_back(vG[preVertex[pass_index]][pass_index].num);
		       pass_index = preVertex[pass_index];
		   }
	       }
	       else if(vG[pass_vertex_start][pass_vertex_end].weight < distance[pass_index] ||(vG[pass_vertex_start][pass_vertex_end].weight == distance[pass_index] && distance[pass_index]!= INT_MAX))
	       {
		   pvG[pvG_i][pvG_j].weight = vG[pass_vertex_start][pass_vertex_end].weight;
		   pvG[pvG_i][pvG_j].path.push_back(vG[pass_vertex_start][pass_vertex_end].num);
	       }
	       pvG_j++;
	   }
       }
       if (iter != 0)
	   pvG_i++;
   }

//寻找最短路径
   for(int i = 0;i < must_pass_len; i++)
   {
       visitedH[i] = 0;
       visitedL[i] = 0;
   }
   for (int i = 0;i != startPathnum;i++)
   {
       if (startPath[i].weight < INT_MAX)
       {
	   recordPath.push_back(i);
	   recordPathWeight = startPath[i].weight;
	   int jishu = i;
	   dfs(i,&jishu,must_pass_len,visitedH,visitedL,pvG,endPath);
       }
       
   }

   int i = jishuT;
   for (size_t te = startPath[i].path.size()-1; te != -1;te--)
   {
       record_result(startPath[i].path[te]);
   }
   for (size_t j = 0; j != findPath.size(); j++)
   {
       for (size_t te = pvG[i][findPath[j]].path.size()-1; te != -1;te--)
       {
	   record_result(pvG[i][findPath[j]].path[te]);
       }
       i = findPath[j];
   }
   for (size_t te = endPath[i].path.size()-1; te != -1;te--)
   {
       record_result(endPath[i].path[te]);
   }

}
