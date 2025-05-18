//ESTAS CON CORRECCIONES D CHATGPT, pa q sirvan de guia, no para entregar :)

// Este SI compila (los nuestros no :/)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "small.h"

/* --------------------------------------------------------------------------
  CONSTANTS                                                                 
-------------------------------------------------------------------------- */
#define INF 1000000

/* -------------------------------------------------------------------------- 
  FUNCTION PROTOTYPES                                                       
-------------------------------------------------------------------------- */
void dijkstra(int graph[NUMBER_CITIES][NUMBER_CITIES], int n, int start,
              int dist[], int prev[]);

int  RouteSearch(struct CivilRegistry *citiesInfo,
                 int id_sourcecity, int id_destcity,
                 struct RoadMap **roadmap);

void addToRoadMap(struct RoadMap **head, int city_id, int step_cost);
void printRoadMap(const struct RoadMap *head);
void deleteAllRoadMap(struct RoadMap **head);

struct FamilyTreeNode *createFamilyTreeDFS(int city_id);
void DFSroute(struct FamilyTreeNode *node, struct RoadMap **roadmap);
void printFamilyTree(const struct FamilyTreeNode *node, int level);
void freeFamilyTree(struct FamilyTreeNode *node);


void dijkstra(int graph[NUMBER_CITIES][NUMBER_CITIES], int n, int start,
              int dist[], int prev[]) {
    int visited[NUMBER_CITIES] = {0};

    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (int count = 0; count < n - 1; ++count) {
        int min = INF, u = -1;
        /* pick the un‑visited node with the smallest distance */
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u   = v;
            }
        }
        if (u == -1) break;              /*   unreachable nodes left   */
        visited[u] = 1;
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph[u][v] &&               /* edge exists  */
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/*  ROUTE SEARCH + PATH RECONSTRUCTION                                        */
/* -------------------------------------------------------------------------- */
int RouteSearch(struct CivilRegistry *citiesInfo,
                int id_sourcecity, int id_destcity,
                struct RoadMap **roadmap) {
    int src = citiesInfo[id_sourcecity].city_id;
    int dst = citiesInfo[id_destcity].city_id;

    /* -------------------------------------------------- direct connection */
    if (adjacency_matrix[src][dst] != 0) {
        addToRoadMap(roadmap, dst, adjacency_matrix[src][dst]);
        return adjacency_matrix[src][dst];
    }

    /* -------------------------------------------------- Dijkstra fallback */
    int distance[NUMBER_CITIES];
    int previous[NUMBER_CITIES];
    dijkstra(adjacency_matrix, NUMBER_CITIES, src, distance, previous);

    if (distance[dst] == INF) {
        printf("No route exists from %s to %s.\n",
               citiesInfo[src].city_name, citiesInfo[dst].city_name);
        return INF;
    }

    /* reconstruct path in reverse */
    int path[NUMBER_CITIES];
    int count = 0;
    for (int v = dst; v != -1; v = previous[v]) path[count++] = v;

    /* add nodes to roadmap in forward order */
    for (int k = count - 1; k >= 0; --k) {
        int step_cost = 0;
        if (k < count - 1) {   /* edge cost from path[k] to path[k+1] */
            step_cost = adjacency_matrix[path[k]][path[k + 1]];
        }
        addToRoadMap(roadmap, path[k], step_cost);
    }

    return distance[dst];
}

/* -------------------------------------------------------------------------- */
/*  ROADMAP LIST UTILITIES                                                    */
/* -------------------------------------------------------------------------- */
void addToRoadMap(struct RoadMap **head, int city_id, int step_cost) {
    struct RoadMap *new_node = malloc(sizeof *new_node);
    if (!new_node) {
        perror("malloc RoadMap");
        exit(EXIT_FAILURE);
    }
    new_node->city_id   = city_id;
    new_node->total_cost = step_cost;
    new_node->next      = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        struct RoadMap *tail = *head;
        while (tail->next) tail = tail->next;
        tail->next = new_node;
    }
}

static void printCityName(int city_id) {
    printf("%s", citiesInfo[city_id].city_name);
}

void printRoadMap(const struct RoadMap *head) {
    const struct RoadMap *curr = head;
    int running_total          = 0;
    while (curr) {
        printCityName(curr->city_id);
        running_total += curr->total_cost;
        printf("  (+%d)\n", curr->total_cost);
        curr = curr->next;
    }
    printf("Total cost: %d\n", running_total);
}

void deleteAllRoadMap(struct RoadMap **head) {
    while (*head) {
        struct RoadMap *tmp = *head;
        *head               = (*head)->next;
        free(tmp);
    }
}

/* -------------------------------------------------------------------------- */
/*  FAMILY TREE DFS CREATION                                                  */
/* -------------------------------------------------------------------------- */
struct FamilyTreeNode *createFamilyTreeDFS(int city_id) {
    struct FamilyTreeNode *node = malloc(sizeof *node);
    if (!node) {
        perror("malloc FamilyTreeNode");
        exit(EXIT_FAILURE);
    }
    strcpy(node->motherName, citiesInfo[city_id].mother_name);
    strcpy(node->fatherName, citiesInfo[city_id].father_name);
    node->city_id        = city_id;
    node->mother_parents = NULL;
    node->father_parents = NULL;

    if (citiesInfo[city_id].mother_parents_city_id != -1) {
        node->mother_parents =
            createFamilyTreeDFS(citiesInfo[city_id].mother_parents_city_id);
    }
    if (citiesInfo[city_id].father_parents_city_id != -1) {
        node->father_parents =
            createFamilyTreeDFS(citiesInfo[city_id].father_parents_city_id);
    }
    return node;
}

/* -------------------------------------------------------------------------- */
/*  BUILD ROADMAP FOLLOWING DFS TREE                                          */
/* -------------------------------------------------------------------------- */
void DFSroute(struct FamilyTreeNode *node, struct RoadMap **roadmap) {
    if (!node) return;

    /* mother side first */
    if (node->mother_parents) {
        RouteSearch(citiesInfo, node->city_id, node->mother_parents->city_id,
                    roadmap);
        DFSroute(node->mother_parents, roadmap);
    }
    /* then father side */
    if (node->father_parents) {
        RouteSearch(citiesInfo, node->city_id, node->father_parents->city_id,
                    roadmap);
        DFSroute(node->father_parents, roadmap);
    }
}

/* -------------------------------------------------------------------------- */
/*  TREE VISUALISATION & DELETION                                             */
/* -------------------------------------------------------------------------- */
void printFamilyTree(const struct FamilyTreeNode *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; ++i) printf("->");
    printf("%s and %s (%s)\n", node->motherName, node->fatherName,
           citiesInfo[node->city_id].city_name);
    printFamilyTree(node->mother_parents, level + 1);
    printFamilyTree(node->father_parents, level + 1);
}

void freeFamilyTree(struct FamilyTreeNode *node) {
    if (!node) return;
    freeFamilyTree(node->mother_parents);
    freeFamilyTree(node->father_parents);
    free(node);
}

/* -------------------------------------------------------------------------- */
/*  MAIN                                                                      */
/* -------------------------------------------------------------------------- */
int main(void) {
    struct RoadMap *roadmap = NULL;

    /* 1. build DFS ancestor tree starting from city 0 (Barcelona) */
    struct FamilyTreeNode *root = createFamilyTreeDFS(0);

    /* 2. initial city in roadmap */
    addToRoadMap(&roadmap, 0, 0);

    /* 3. traverse the tree and build journey */
    DFSroute(root, &roadmap);

    /* 4. output */
    printf("Ancestors tree (DFS):\n");
    printFamilyTree(root, 0);

    printf("\nRoad map (DFS order):\n");
    printRoadMap(roadmap);

    /* 5. cleanup */
    deleteAllRoadMap(&roadmap);
    freeFamilyTree(root);

    return 0;
}
