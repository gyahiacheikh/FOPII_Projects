#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "small.h" // Me da la adjecent matrix y los registros

#define INF 99999999 //Infinite cost for i==j
/*
// 1. Defining structures
struct RoadMap { //una linked list para ir guardando el recorrido
    int city_id;
    int total_cost;
    struct RoadMap*next; // guarda el siguiente nodo (la siguiente ciudad)

};

struct FamilyTreeNode{ //nodo del family tree
    char name_mother[10];
    char name_father[10];
    int city_id;
    struct FamilyTreeNode*mother_parents; //pointers a los papis de la madre y el padre
    struct FamilyTreeNode*father_parents;
};
*/
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

int main(){
    struct RoadMap*roadmap=NULL;

    //PRUEBAAAAAS
    addToRoadMap(&roadmap, 0, 0);
    addToRoadMap(&roadmap, 1, 50);   
    addToRoadMap(&roadmap, 2, 180);

    printf("Actual route: (DEMO)\n");
    printRoadMap(roadmap);
    deleteAllRoadMap(&roadmap);

    return 0;
}