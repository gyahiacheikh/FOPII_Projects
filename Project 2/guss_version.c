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

// 2. RoadMap type shi
// RoadMap es una llista enllaçada on cada nodo es una ciutat, guarda el cost total fins la ciutat aquella i apunta al seguent ciutat (node)
void addToRoadMap(struct RoadMap**head, int city_id, int total_cost){ //afegeix un nou node al final de la llista
    struct RoadMap *new_node= malloc(sizeof(struct RoadMap));
    new_node->city_id=city_id; 
    new_node->total_cost =total_cost;
    new_node-> next= NULL; 

    if (*head == NULL){
        *head= new_node; //If list empty creates node
    } else{ // Sino pues al final ou yeah
        struct RoadMap*temp= *head;
        while(temp->next!= NULL) temp=temp->next;
        temp->next= new_node;
    }

}

void printRoadMap(struct RoadMap*head){ //pa ver la rout
    struct RoadMap*temp=head;
    while (temp!=NULL){
        PrintCityName(temp->city_id);
        printf(" %d \n", temp->total_cost);
        temp=temp->next;
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

/*
FALTA:
    - muderfucking fuck heuristics (el routesearch)
    - El DFS (deberia follar serpientes) y BFS (bolleras, fumetas, sinceras)
    - el printeo de los trees
    - y bueno otras cosas que seguro que me dejo
sharaut sara del 13 de mayo si estas viendo esto dimelou

*/

int RouteSearch(int source, int destination, struct RoadMap**roadmap){
    int visited[NUMBER_CITIES]={0};
    int current=source;
    int total_cost=0;
    while (current!=destination){
        visited[current]=1;
        int next_city=-1;
        int min_cost=INF; //garanteix que qualsevol ciutat sigui mes barata 

        //provem si es pot directament
        if (adjacency_matrix[current][destination]>0 && adjacency_matrix[current][destination]<min_cost){
            next_city=destination;
            min_cost=adjacency_matrix[current][destination];
        }
        //aqui busque, la ciutat mes barata connectada a current 
        for(int i=0;i<NUMBER_CITIES;i++){
            if(!visited[i]&&adjacency_matrix[current][i]>0 && adjacency_matrix[current][i]<min_cost){
                next_city=i;
                min_cost=adjacency_matrix[current][i];
            }
        }
        // si no hi han mes ciutats disponibles doncs que salti un missatge
        if (next_city==-1){
            printf("We couldn't find a rout from %d to %d. \n", source, destination);
            return -1;
        }

        //Actualizamos
        total_cost+=min_cost;
        addToRoadMap(roadmap,next_city,total_cost);
        current=next_city;
    }
    return total_cost;
}

//DFS

struct FamilyTreeNode*createFamilyTreeDFS(int city_id){
    struct FamilyTreeNode*node=malloc(sizeof(struct FamilyTreeNode));
    if (node==NULL){
        printf("ERROR: No se pudo reservar memoria para el nodo.\n");
        return NULL;
    }
    strcpy(node->motherName, citiesInfo[city_id].mother_name);
    strcpy(node->fatherName, citiesInfo[city_id].father_name);
    node->city_id=city_id;
    node->mother_parents=NULL;
    node->father_parents=NULL;

    if(citiesInfo[city_id].mother_parents_city_id != -1){
        node->mother_parents =createFamilyTreeDFS(citiesInfo[city_id].mother_parents_city_id);
    }
    if(citiesInfo[city_id].father_parents_city_id != -1){
        node->father_parents =createFamilyTreeDFS(citiesInfo[city_id].father_parents_city_id);
    }

    return node;
}

void printFamilyTree(struct FamilyTreeNode* node, int level){
    if (node == NULL) {
        printf("-> NULL node\n");
        return;
    }
    for (int i = 0; i < level; i++) printf("-> ");
    printf("%s and %s (", node->motherName, node->fatherName);
    PrintCityName(node->city_id);
    printf(")\n");

    printFamilyTree(node->mother_parents, level + 1);
    printFamilyTree(node->father_parents, level + 1);
}

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