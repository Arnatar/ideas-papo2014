#include<stdio.h>
#include <stdlib.h>

#define FIELDSIZE 10

// structs
struct idea{
    int complexity;
    int persuasiveness;
};

// globals
struct idea ideas[FIELDSIZE][FIELDSIZE];


// methods
void initIdeas(int Number, int maxTraits);
void genOutput();
void doStep();

int main() {
    initIdeas(10, 6);
    for(int i = 0; i < 10; i++){
        doStep();
        genOutput();
    }
    return 0;
}


//
void initIdeas(int Number, int maxTraits){
    int i = 0;
    while(i < Number){
        int x = rand() % FIELDSIZE;
        int y = rand() % FIELDSIZE;
         // hier noch bug, da rand() von 0 bis randmax geht
        if(ideas[x][y].complexity == 0 && ideas[x][y].persuasiveness == 0){
            ideas[x][y].complexity = rand() % maxTraits;
            ideas[x][y].persuasiveness = rand() % (maxTraits - ideas[x][y].complexity);
            i++;
        }
    }
}

void genOutput(){
    for(int x = 0; x < 10; x ++){
    for(int y = 0; y < 10; y ++){
        printf("(%d %d) ", ideas[x][y].complexity, ideas[x][y].persuasiveness);
    }
    printf("\n");
  }
  printf("\n");
}

void doStep(){

}
