#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "small.h" // Me da la adjecent matrix y los registros

#define INF 99999999 //Infinite cost for i==j
#define MAX_PARTIALS 32

//Arrays and counters to save the partial routes of DFS and BFS
struct PartialRoute partialsDFS[MAX_PARTIALS];
int partial_count_DFS = 0;
struct PartialRoute partialsBFS[MAX_PARTIALS];
int partial_count_BFS = 0;

void PrintCityName(int city_id){ //para imprimir el nombre de la ciudad por si aca
    printf("City: %s", citiesInfo[city_id].city_name);
}

/*
void printRoadMap(struct RoadMap*head){ //Recorre la llista enllaçada RoadMap i imprime cada ciudad y coste acumulado
    struct RoadMap*temp=head;
    while (temp!=NULL){
        PrintCityName(temp->city_id);
        printf(" %d \n", temp->total_cost);
        temp=temp->next;
    }
}
*/

void addToRoadMap(struct RoadMap**head, int city_id, int total_cost){ // Afegeix una ciutat al final de la llista enllaçada
    if (*head != NULL){
        struct RoadMap*last = *head;
        while (last->next != NULL) last = last->next;
        if (last->city_id == city_id) return; // Evita duplicados consecutivos
    }

    struct RoadMap *new_node = malloc(sizeof(struct RoadMap));
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
        prev[i] = -1;
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
    if (!roadmap) return 0; //Si la llista esta buida
    while (roadmap->next != NULL) roadmap = roadmap->next; //Va fins al final de la llista enllaçada
    return roadmap->total_cost; // Torna el cost total acumulat desde l'ultim node
}

//DFS

struct FamilyTreeNode*createFamilyTreeDFS(int city_id){
    struct FamilyTreeNode*node=malloc(sizeof(struct FamilyTreeNode));
    if (node==NULL){
        printf("ERROR: memory allocation for the node failed.\n");
        return NULL;
    }
    // Copia els noms dels papis i el id de la ciutat desde citiesInfo
    strcpy(node->motherName, citiesInfo[city_id].mother_name);
    strcpy(node->fatherName, citiesInfo[city_id].father_name);
    node->city_id=city_id;
    node->mother_parents=NULL; //Initialitza els punters dels papis com a NULL
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

// DFS
void DFSroute(struct FamilyTreeNode* node, struct RoadMap** roadmap, int current_city) {
    if (node == NULL) return;

    if (node->mother_parents != NULL) { //PEDIR MAS EXPLICACION A COPILOT
        int offset = getCurrentTotalCost(*roadmap);
        RouteSearch(current_city, node->mother_parents->city_id, roadmap, offset);
        partialsDFS[partial_count_DFS].from = current_city;
        partialsDFS[partial_count_DFS].to = node->mother_parents->city_id;
        partial_count_DFS++;
        DFSroute(node->mother_parents, roadmap, node->mother_parents->city_id);
    }
    if (node->father_parents != NULL) {
        int offset = getCurrentTotalCost(*roadmap);
        RouteSearch(current_city, node->father_parents->city_id, roadmap, offset);
        partialsDFS[partial_count_DFS].from = current_city;
        partialsDFS[partial_count_DFS].to = node->father_parents->city_id;
        partial_count_DFS++;
        DFSroute(node->father_parents, roadmap, node->father_parents->city_id);
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

// BFS
void BFSroute(struct FamilyTreeNode* root, struct RoadMap** roadmap) {
    if (root == NULL) return;

    struct FamilyTreeNode* queue[NUMBER_CITIES];
    int city_queue[NUMBER_CITIES];
    int front = 0, rear = 0;
    queue[rear] = root;
    city_queue[rear++] = root->city_id;

    while (front < rear) {
        struct FamilyTreeNode* current = queue[front];
        int from_city = city_queue[front++];
        if (current->mother_parents != NULL) {
            int offset = getCurrentTotalCost(*roadmap);
            RouteSearch(from_city, current->mother_parents->city_id, roadmap, offset);
            partialsBFS[partial_count_BFS].from = from_city;
            partialsBFS[partial_count_BFS].to = current->mother_parents->city_id;
            partial_count_BFS++;
            queue[rear] = current->mother_parents;
            city_queue[rear++] = current->mother_parents->city_id;
        }
        if (current->father_parents != NULL) {
            int offset = getCurrentTotalCost(*roadmap);
            RouteSearch(from_city, current->father_parents->city_id, roadmap, offset);
            partialsBFS[partial_count_BFS].from = from_city;
            partialsBFS[partial_count_BFS].to = current->father_parents->city_id;
            partial_count_BFS++;
            queue[rear] = current->father_parents;
            city_queue[rear++] = current->father_parents->city_id;
        }
    }
}


void printFamilyTreeDFS(struct FamilyTreeNode* node, int level){
    if (node==NULL) return;
    for (int i = 0; i < level; i++) printf("-> ");
    printf("%s and %s (", node->motherName, node->fatherName);
    PrintCityName(node->city_id);
    printf(")\n");

    printFamilyTree(node->mother_parents, level + 1); //Crida recursivament per imprimir el sub arbre de la mama/papa pujant el nivell
    printFamilyTree(node->father_parents, level + 1);
}

void printFamilyTreeBFS(struct FamilyTreeNode* root) {
    if (root == NULL) return;

    // Define a queue and corresponding level tracker
    struct FamilyTreeNode* queue[NUMBER_CITIES];
    int levels[NUMBER_CITIES];  // to track levels for indentation

    int front = 0, rear = 0;
    queue[rear] = root;
    levels[rear++] = 0;

    while (front < rear) {
        struct FamilyTreeNode* current = queue[front];
        int level = levels[front++];
        
        // Print the current node with indentation
        for (int i = 0; i < level; i++) printf("-> ");
        printf("%s and %s (", current->motherName, current->fatherName);
        PrintCityName(current->city_id);
        printf(")\n");

        // Enqueue children with incremented level
        if (current->mother_parents) {
            queue[rear] = current->mother_parents;
            levels[rear++] = level + 1;
        }

        if (current->father_parents) {
            queue[rear] = current->father_parents;
            levels[rear++] = level + 1;
        }
    }
}


/*
int main(){
    struct RoadMap*roadmap=NULL;
    // mas pruebas
    int source=0;
    int destination=3;

    addToRoadMap(&roadmap, source,0);

    int total_cost= RouteSearch(source, destination, &roadmap);

    printf("\nFinal route (DEMO): \n");
    printRoadMap(roadmap);
    printf("\nTotal cost: %d\n", total_cost);

    deleteAllRoadMap(&roadmap);

    printf("Test: root = %s and %s\n", citiesInfo[0].mother_name, citiesInfo[0].father_name);

    printf("\nDFS Ancestor Tree:\n");
    struct FamilyTreeNode *tree = createFamilyTreeDFS(0);  // Empiezas por ciudad 0 (Barcelona)
    printFamilyTree(tree, 0);

    return 0;
}
*/

void printPartialRoute(struct RoadMap* roadmap, int from, int to) {
    struct RoadMap* curr = roadmap; // Es posa al inici del roadmap
    int started = 0, cost = 0; //acumular coste i saber si ha empezado
    while (curr && curr->next) { // recurre la llista
        if (!started && curr->city_id == from) started = 1; // marca que comença si no ha començat
        if (started) {
            printf("%s", citiesInfo[curr->city_id].city_name); // Imprime la ciudad actual
            int step = adjacency_matrix[curr->city_id][curr->next->city_id]; // Calcula el cost al següent node
            if (curr->next->city_id != to) printf("-"); //si no es el destino un guion ou yeah
            cost += step; //suma el cost del tram
        }
        if (started && curr->next->city_id == to) { //si ya ha arribat al desti
            printf("-%s %d\n", citiesInfo[to].city_name, cost + adjacency_matrix[curr->city_id][to]); // imprimeix el desti i el cost total
            break;
        }
        curr = curr->next; // passa al seguent node
}


/*
HOLIIIIIS tenemos un par d errores en el DFS. Esto es lo que printea:abortDFS -> Names:
Maria and Jordi (City: Barcelona)
-> Louise and Paul (City: Paris)
-> -> Anna and Kazimierz (City: Varsovia)
-> -> Agnese and Leonardo (City: Rome)
-> Eva and Albert (City: Zurich)
-> -> Madalena and Louren├ºo (City: Lisbon)
-> -> Amber and Finn (City: Amsterdam)

DEBERIA printear esto:
-> Louise and Pol (Paris)
-> Eva and Albert (Zurich)
->-> Anna and Kazimierz (Varsovia)
->-> Agnese and Leonardo (Rome)
->-> Madalena and Lourenc¸o (Lisboa)
->-> Amber and Fin (Amsterdam)

Es decir, los que tienen solo una flecha -> deberian ir primero. Asi la suma de los costos seria menor (deberia serlo)

*/



int main(){
    // DFS BLOCK
    printf("Ancestors’ tree:\n");
    printf("DFS -> Names:\n");

    struct FamilyTreeNode* treeDFS = createFamilyTreeDFS(0); // Crea l'abre DFS cpmençant x barcelona (0)
    printFamilyTreeDFS(treeDFS,0); //printeow

    struct RoadMap* roadmapDFS = NULL; //inicia el roadmap null
    DFSroute(treeDFS, &roadmapDFS, treeDFS->city_id); //cridem ald DFSroute per calcular els trajectes i omplir el roadmap i els partial routes

    printf("\nPartial road map:\n");
    for (int i = 0; i < partial_count_DFS; i++) {
        printPartialRoute(roadmapDFS, partialsDFS[i].from, partialsDFS[i].to);
    }

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
    printFamilyTreeBFS(treeBFS);

    struct RoadMap* roadmapBFS = NULL;
    BFSroute(treeBFS, &roadmapBFS);

    printf("\nPartial road map:\n");
    for (int i = 0; i < partial_count_BFS; i++) {
        printPartialRoute(roadmapBFS, partialsBFS[i].from, partialsBFS[i].to);
    }

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