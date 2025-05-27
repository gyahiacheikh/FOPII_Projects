#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "small.h" // Me da la adjecent matrix y los registros

#define INF 99999999 //Infinite cost for i==j

void PrintCityName(int city_id){ //para imprimir el nombre de la ciudad por si aca
    printf("City: %s", citiesInfo[city_id].city_name);
}

// Va per tota la llista RoadMap i imprimeix cada ciutat amb el cost acumulat
void printRoadMap(struct RoadMap*head){ //pa ver la rout
    struct RoadMap*temp=head;
    while (temp!=NULL){
        PrintCityName(temp->city_id);
        printf(" %d \n", temp->total_cost);
        temp=temp->next;
    }
}


void addToRoadMap(struct RoadMap**head, int city_id, int total_cost){
    if (*head != NULL){
        struct RoadMap*last = *head;
        while (last->next != NULL) last = last->next;
        if (last->city_id == city_id) return; // Evita duplicados consecutivos
    }

    struct RoadMap *new_node = malloc(sizeof(struct RoadMap)); //Crea nou node amb city_id i totalcost i el fica al final de la llista
    new_node->city_id = city_id; 
    new_node->total_cost = total_cost;
    new_node->next = NULL;

    if (*head == NULL){
        *head = new_node;
    } else {
        struct RoadMap*temp = *head;
        while(temp->next != NULL) temp = temp->next;
        temp->next = new_node;
    }
}

void deleteAllRoadMap(struct RoadMap **head){//LLAMAR AL FINAL DEL PROGRAMA SOLO EH
    struct RoadMap*temp;
    while(*head!=NULL){
        temp=*head;
        *head=(*head)->next;
        free(temp);
    }

}



int RouteSearch(int source, int destination, struct RoadMap** roadmap, int cost_offset) {
    // Initialize distance array with INF, visited array with 0, and prev array with -1
    int dist[NUMBER_CITIES], visited[NUMBER_CITIES] = {0}, prev[NUMBER_CITIES];
    for (int i = 0; i < NUMBER_CITIES; i++) {
        dist[i] = INF; // Set initial distances to infinity
        prev[i] = -1; // es para reconstruir el camino
    }
    dist[source] = 0;

    // Dijkstra's algorithm main loop
    for (int count = 0; count < NUMBER_CITIES - 1; count++) {
        int u = -1, min = INF;
        // Find the unvisited node with the smallest distance
        for (int i = 0; i < NUMBER_CITIES; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }

        if (u == -1) break;  // No reachable unvisited node left
        visited[u] = 1;  // Mark the node as visited

        // Update distances to adjacent unvisited nodes
        for (int v = 0; v < NUMBER_CITIES; v++) {
            if (adjacency_matrix[u][v] > 0 && !visited[v]) {
                // Calculate new distance
                int new_dist = dist[u] + adjacency_matrix[u][v];
                if (new_dist < dist[v]) {
                    // Update shortest distance
                    dist[v] = new_dist;
                    // Track the path
                    prev[v] = u;
                }
            }
        }
    }

    // If the destination is unreachable:
    if (dist[destination] == INF) {
        printf("No route from %s to %s.\n", citiesInfo[source].city_name, citiesInfo[destination].city_name);
        return -1;
    }

    // Rebuild the path in correct order
    int path[NUMBER_CITIES];
    int length = 0, current = destination;
    while (current != -1) {
        // Add current node to path
        path[length++] = current;
        // Move to previous node
        current = prev[current];
    }

    // Find the tail of the roadmap
    struct RoadMap* last = *roadmap;
    int last_id = -1;
    while (last && last->next) last = last->next;
    if (last) last_id = last->city_id;

    // Add only *new* cities from the path
    for (int i = length - 1; i >= 0; i--) {
        int city = path[i];
        if (city == last_id) continue; // skip if already added
        addToRoadMap(roadmap, city, dist[city] + cost_offset);  // Add city with total cost
    }

    return dist[destination];
}


int getCurrentTotalCost(struct RoadMap* roadmap) {
    if (!roadmap) return 0;
    while (roadmap->next != NULL) roadmap = roadmap->next;
    return roadmap->total_cost;
}

//DFS

struct FamilyTreeNode*createFamilyTreeDFS(int city_id){
    struct FamilyTreeNode*node=malloc(sizeof(struct FamilyTreeNode));
    if (node==NULL){
        printf("ERROR: memory allocation for the node failed.\n");
        return NULL;
    }
    // Copy the names of the parents from the citiesInfo array
    strcpy(node->motherName, citiesInfo[city_id].mother_name);
    strcpy(node->fatherName, citiesInfo[city_id].father_name);
    node->city_id=city_id;
    node->mother_parents=NULL;
    node->father_parents=NULL;

    // Recursively build the subtree for mother's side
    if(citiesInfo[city_id].mother_parents_city_id != -1){
        node->mother_parents =createFamilyTreeDFS(citiesInfo[city_id].mother_parents_city_id);
    }
    // Recursively build the subtree for father's side
    if(citiesInfo[city_id].father_parents_city_id != -1){
        node->father_parents =createFamilyTreeDFS(citiesInfo[city_id].father_parents_city_id);
    }

    return node;
}

// Perform DFS traversal on the family tree and generate route map
void DFSroute(struct FamilyTreeNode*node, struct RoadMap**roadmap){
    if (node==NULL) return;

    // If mother’s parents exist, go there
    if (node->mother_parents != NULL){
        int offset = getCurrentTotalCost(*roadmap);
        RouteSearch(node->city_id, node->mother_parents->city_id, roadmap, offset);
        DFSroute(node->mother_parents, roadmap); //pedazo recursion aqui chaval
    }
    //mismo deal con el papa
    if (node->father_parents != NULL){
        int offset = getCurrentTotalCost(*roadmap);
        RouteSearch(node->city_id, node->father_parents->city_id, roadmap, offset);
        DFSroute(node->father_parents, roadmap); //pedazo recursion aqui chaval
    }
}


//BFS
/*Pseudocode:
create a queue Q 
mark v as visited and put v into Q 
while Q is non-empty 
    remove the head u of Q 
    mark and enqueue all (unvisited) neighbours of u


 Function to perform BFS codigo plantilla
*/


struct FamilyTreeNode* createFamilyTreeBFS(int city_id)
{
    struct FamilyTreeNode* root = malloc(sizeof(struct FamilyTreeNode));
    if (!root) return NULL;

    strcpy(root->motherName, citiesInfo[city_id].mother_name);
    strcpy(root->fatherName, citiesInfo[city_id].father_name);
    root->city_id = city_id;

    // Initialize pointers to parent nodes as NULL
    root->mother_parents = root->father_parents = NULL;

    // Queue for BFS traversal using an array
    struct FamilyTreeNode* queue[NUMBER_CITIES];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        struct FamilyTreeNode* curr = queue[front++]; // Dequeue a node

        // Get mother's and father's parent city IDs
        int mid = citiesInfo[curr->city_id].mother_parents_city_id;
        int fid = citiesInfo[curr->city_id].father_parents_city_id;

        if (mid != -1) {
            struct FamilyTreeNode* mother = malloc(sizeof(struct FamilyTreeNode));
            strcpy(mother->motherName, citiesInfo[mid].mother_name);
            strcpy(mother->fatherName, citiesInfo[mid].father_name);
            mother->city_id = mid;
            mother->mother_parents = mother->father_parents = NULL;
            curr->mother_parents = mother;
            queue[rear++] = mother;
        }
        if (fid != -1) {
            struct FamilyTreeNode* father = malloc(sizeof(struct FamilyTreeNode));
            strcpy(father->motherName, citiesInfo[fid].mother_name);
            strcpy(father->fatherName, citiesInfo[fid].father_name);
            father->city_id = fid;
            father->mother_parents = father->father_parents = NULL;
            curr->father_parents = father;
            queue[rear++] = father;
        }
    }
    return root;
}


void BFSroute(struct FamilyTreeNode* root, struct RoadMap** roadmap) {
    if (root == NULL) return;

    // Create a queue for BFS
    struct FamilyTreeNode* queue[NUMBER_CITIES];  // Cola de nodos juajauaja
    int front = 0, rear = 0;

    // Enqueue root and initialize rear
    queue[rear++] = root;

    while (front < rear) {
        // Print front of queue and remove it from queue
        struct FamilyTreeNode* current = queue[front++];

        if (current->mother_parents != NULL) {
            int offset = getCurrentTotalCost(*roadmap);
            RouteSearch(current->city_id, current->mother_parents->city_id, roadmap, offset);
            queue[rear++] = current->mother_parents;
        }

        // same here with the papa
        if (current->father_parents != NULL) {
            int offset = getCurrentTotalCost(*roadmap);
            RouteSearch(current->city_id, current->father_parents->city_id, roadmap, offset);
            queue[rear++] = current->father_parents;
        }
    }
}



void printFamilyTree(struct FamilyTreeNode* node, int level){
    if (node==NULL) return;
    for (int i = 0; i < level; i++) printf("-> ");
    printf("%s and %s (", node->motherName, node->fatherName);
    PrintCityName(node->city_id);
    printf(")\n");

    printFamilyTree(node->mother_parents, level + 1);
    printFamilyTree(node->father_parents, level + 1);
}


void printFormattedRoadMap(struct RoadMap* head) {
    if (!head || !head->next) return;

    struct RoadMap* current = head;
    struct RoadMap* next = current->next;

    int segment_cost = 0;

    printf("%s", citiesInfo[current->city_id].city_name);

    while (next != NULL) {
        int from = current->city_id;
        int to = next->city_id;
        int cost = adjacency_matrix[from][to];

        // Si hay una conexión válida, es parte del mismo segmento
        if (cost > 0) {
            printf("-%s", citiesInfo[to].city_name);
            segment_cost += cost;
        }
        else {
            // Si no hay conexión directa, nuevo segmento
            printf(" %d\n", segment_cost);
            segment_cost = 0;
            printf("%s", citiesInfo[to].city_name);
        }

        current = next;
        next = next->next;
    }

    // Último tramo
    printf(" %d\n", segment_cost);
}



int main(){
    // DFS BLOCK
    printf("Ancestors’ tree:\n");
    printf("DFS -> Names:\n");

    struct FamilyTreeNode* treeDFS = createFamilyTreeDFS(0);
    printFamilyTree(treeDFS, 0);

    struct RoadMap* roadmapDFS = NULL;
    DFSroute(treeDFS, &roadmapDFS);

    printf("\nPartial road map:\n");
    printFormattedRoadMap(roadmapDFS);

    printf("Complete Road Map:\n");
    struct RoadMap* temp = roadmapDFS;
    while (temp != NULL) {
        printf("%s", citiesInfo[temp->city_id].city_name);
        if (temp->next != NULL) printf("-");
        temp = temp->next;
    }

    int total = getCurrentTotalCost(roadmapDFS);

    printf("\nTotal cost: %d\n", total);

    deleteAllRoadMap(&roadmapDFS);

    printf("----------------------------------\n");

    //BFS BLOCK
    printf("BFS -> Names:\n");

    struct FamilyTreeNode* treeBFS = createFamilyTreeBFS(0);
    printFamilyTree(treeBFS, 0);

    struct RoadMap* roadmapBFS = NULL;
    BFSroute(treeBFS, &roadmapBFS);

    printf("\nPartial road map:\n");
    // igual que DFS pero siguiendo orden BFS
    printFormattedRoadMap(roadmapBFS);

    printf("Complete Road Map:\n");
    // igual que DFS
    struct RoadMap* temp2 = roadmapBFS;
    while (temp2 != NULL) {
        printf("%s", citiesInfo[temp2->city_id].city_name);
        if (temp2->next != NULL) printf("-");
        temp2 = temp2->next;
    }

    int total2 = getCurrentTotalCost(roadmapBFS);
    printf("\nTotal cost: %d\n\n\n", total2);

    deleteAllRoadMap(&roadmapBFS);

    return 0;
}