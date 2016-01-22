/*
   Ye Liang
   A program that finds the shortest path between 2 nodes in a graph using Dijkstra algorithm, created for CSCI2132 Assignment8
   Input:
	   It reads in input from command line, The first line of input is the number of vertices in the graph. 
	   This line is followed by a sequence of lines giving edge distances. 
	   Each line for edge distance will have three numbers i,j, d, separated by whitespace, where i and j are integers denoting nodes, 
	   and d is a foating-point (double) number denoting direct edge distance between these nodes.
   Output:
	   For each query about nodes i and j, the program produces one line of output in the format:
	   The shortest path from i to j is d meters.
	   There is a possibility that there is no path from the node i to node j in some graph, and in this case, the output is:
	   Cannot find path from i to j.

*/

#include <stdio.h>
#include <stdlib.h>
#define INF 41e6

//Structure of adjacency node
struct adjnode 
{
  int dest;
  double dist;
  struct adjnode* next;
};

//Structure of adjacency lists
struct adjlist
{
    struct adjnode* head;  // pointer to head node of list
};
 
//Structure of graph. 
struct graph
{
    int n; //number of vertices
    struct adjlist* array;//each element in the array is a linked list of adjnode
};

/******functions that deal w/ node and graph**********/
struct adjnode* newAdjListNode(int dest, double dist); //create new adjacency node
struct graph* createGraph(int n); // create new graph
void addEdge(struct graph* graph, int src, int dest, double dist); // add an edge to a graph
void freeGraph(struct graph* graph); // free space of a given graph
void printGraph(struct graph* graph); //display a graph

/******function that returns shortest path using Dijkstra algorithm**********/
double Dijkstra(struct graph* graph, int source, int dest); 

/*********main function***********/
int main()
{
   int n;
   scanf("%d\n",&n);
   
   //create a graph consists of n vertices
   struct graph* graph =createGraph(n);
   
   int src,dest;   
   double dist;
      
   //read in distances between nodes
   while(scanf("%d %d %lf\n",&src, &dest,&dist)==3&&!(src==0&&dest==0&&dist==0))
   {
      addEdge(graph, src, dest, dist);
   }
   //print the graph
   printGraph(graph);
   
   //read in pairs of nodes and print shortest distance between the 2 nodes
   while(scanf("%d %d",&src,&dest)==2 &&!(src==0&&dest==0))
   {
      double path;
      path=Dijkstra(graph, src, dest);
      if(path==INF)
         printf("Cannot find path from %d to %d.\n",src,dest);
      else
         printf("The shortest path from %d to %d is %.1lf meters.\n", src,dest,path);
   }

   freeGraph(graph);

   return 0;
}
 
// function to create a new node
struct adjnode* newAdjListNode(int dest, double dist)
{
    struct adjnode* newNode =(struct adjnode*) malloc(sizeof(struct adjnode));
    newNode->dest = dest;
    newNode->dist = dist;
    newNode->next = NULL;
    return newNode;
}
 
//function that creates a graph 
struct graph* createGraph(int n)
{
    struct graph* graph = (struct graph*) malloc(sizeof(struct graph));
    graph->n = n;
 
    // Create an array of lists
    graph->array = (struct adjlist*) malloc(n * sizeof(struct adjlist));
 
     // Initialize each list
    int i;
    for (i = 0; i < n; i++)
        graph->array[i].head = NULL;
 
    return graph;
}
 
//add an edge to an graph
void addEdge(struct graph* graph, int src, int dest, double dist)
{
    // add an edge from src to dest. 
    struct adjnode* newNode = newAdjListNode(dest,dist);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    // add an edge from dest to src.
    newNode = newAdjListNode(src,dist);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

//a helper function created to print the graph
void printGraph(struct graph* graph)
{
   struct adjnode* curr =(struct adjnode*) malloc(sizeof(struct adjnode));
   int i, n=graph->n;
   for(i=0;i<n;i++)
   {
      curr=graph->array[i].head;
      printf("Vertice %d: ", i);
      while(curr!=NULL)
      {
         printf("[%d, %lf]=>", curr->dest,curr->dist);
         curr=curr->next;
      }
      printf("\n");
   }
}

void freeGraph(struct graph* graph)
{
   free(graph->array);
   free(graph);
}

/* graph is a pointer to the graph structure, source and dest
   are source and destination nodes, and the function returns
   minimal distance between source and dest, or INF if there
   is no path */
double Dijkstra(struct graph* graph, int source, int dest)
{
   int n = graph->n;
   double dist[n]; /* VLA for shortest distances from the source */
   int visited[n]; /* indicator for visited nodes */
   int Q[n], Qlast; /* the set of unvisited nodes, sorted according to their current best distance from source */
   int i, v, u; /* auxiliary variables */
   for (i=0; i<n; ++i) 
   {
      dist[i] = INF; 
      visited[i] = 0; 
      Q[i] = i;
   }
   Qlast = n-1;
   dist[source] = 0;
   for(i=0; Q[i]!=source; ++i) 
      ; /* find source in Q */
   while (i<Qlast && dist[source] < dist[Q[i+1]]) 
   {
      Q[i] = Q[i+1]; 
      Q[i+1] = source; 
      i++;
   } /* move source at the end of Q since it has lowest dist */
   
   //struct adjnode* curr= (struct adjnode*) malloc(sizeof(struct adjnode));
   
   while (Qlast >= 0) 
   { /* while Q is not empty */
      v = Q[Qlast--]; /* remove node with smallest dist from Q */
      if (dist[v] == INF) 
         return INF; /* This means that all nodes that can be reached are reached, and Q now contains only unreachable nodes */
      if (v == dest) 
         return dist[v]; /* found shortest path to dest */
      visited[v] = 1;
      
      
      struct adjnode* curr= graph->array[v].head;
      while(curr!=NULL)
      //for all neighbours u of v 
      {
         u=curr->dest;
         if (visited[u]==0) 
         {
            if (dist[u] > dist[v]+curr->dist) 
            {
               dist[u] = dist[v]+curr->dist;
                     /* We need to keep Q sorted */
               for(i=0; Q[i]!=u; ++i) 
                  ; /* find u in Q */
               while (i<Qlast && dist[u] < dist[Q[i+1]]) 
               {
                  Q[i] = Q[i+1]; 
                  Q[i+1] = u; 
                  i++;
               } /* move u to the appropriate place in Q */
            }
         }
         curr=curr->next;  
      } 
   }
   
   return INF; /* This line should never be reached */
}
