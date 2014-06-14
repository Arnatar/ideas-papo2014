#include<stdio.h>
#include <stdlib.h>
#include <time.h>


#define FIELDSIZEX 10
#define FIELDSIZEY 7

// structs
struct idea{
    int complexity;
    int persuasiveness;
};

// globals
struct idea ideas[FIELDSIZEY][FIELDSIZEX];


// methods
void initIdeas(int Number, int maxTraits);
void genOutput();
void doStep();

int main() {
    time_t t;
    srand((unsigned) time(&t));
    initIdeas(5, 6);
    for(int i = 0; i < 10; i++){
        doStep();
        genOutput();
    }
    return 0;
}


// bei negetivem Input gilt BIBO
void initIdeas(int Number, int maxTraits){
    int i = 0;
    while(i <= Number){
        int x = rand() % FIELDSIZEX;
        int y = rand() % FIELDSIZEY;
        if(ideas[y][x].complexity == 0 && ideas[y][x].persuasiveness == 0){
            ideas[y][x].complexity = rand() % maxTraits;
            ideas[y][x].persuasiveness = rand() % (maxTraits - ideas[y][x].complexity);
            i++;
        }
    }
}

void genOutput(){
    for(int y = 0; y < FIELDSIZEY; y++){
        for(int x = 0; x < FIELDSIZEX; x++){
            printf("(%d %d) ", ideas[y][x].complexity, ideas[y][x].persuasiveness);
        }
        printf("\n");
    }
    printf("\n");
}

void doStep(){
    for(int y = 0; y < FIELDSIZEY; y++){
        for(int x = 0; x < FIELDSIZEX; x++){
            if(!(ideas[y][x].complexity == 0 || ideas[y][x].persuasiveness == 0)){
                int xoffset = (rand() % 2) - (rand() % 2);
                int newx = x + xoffset;
                int yoffset = (rand() % 2) - (rand() % 2);
                int newy = y + yoffset;
//              printf("x: %d. y: %d. xoffest: %d. yoffset: %d. \n", x, y, xoffset, yoffset); bug, wird nicht Number-oft aufgerufen, idk why
                if(newx >= 0 && newx < FIELDSIZEX && newy >= 0 && newy < FIELDSIZEY && (ideas[newy][newx].complexity == 0 && ideas[newy][newx].persuasiveness == 0)){
                    ideas[newy][newx].complexity = ideas[y][x].complexity;
                    ideas[newy][newx].persuasiveness = ideas[y][x].persuasiveness;
                    ideas[y][x].complexity = 0;
                    ideas[y][x].persuasiveness = 0;
                }

            }
        }
    }
}
