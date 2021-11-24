#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HEIGHT 40
#define WIDTH 60

typedef struct {
    char *letter;
    bool visited;
    bool isNode;
} grid;

//initialize grid
grid used_grid[HEIGHT][WIDTH];

int main(int argc, char *argv[]){
    bool infiniteMode = false;
    bool help = false;
    if (argc > 1){
        for (int i=1; i<argc; i++){
            if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--infinite") == 0 ) infiniteMode = true;
            else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) help = true; 
            else{
                printf("%s: invalid option %s", argv[0], argv[i]);
                printf("try %s --help for more information.", argv[0]);
                exit(2);
            }
        }
    }
    if(help){
        printf("Usage: %s [OPTION]\n", argv[0]);
        puts("Graph traversal visualiser");
        puts("--------------------------------------------------");
        printf("-i, %-20s %s\n", "--infinite",  "runs until stopped with new graphs");
        printf("-h, %-20s %s\n","--help", "display help");
        exit(0);
    }
    while(infiniteMode){
        fprintf(stdout, "\033[H\033[J");
        makeGraph();
        depthFirst(0, 0);
    }
    /* clear the console */    
    fprintf(stdout, "\033[H\033[J");
    makeGraph();
    depthFirst(0, 0);
}

void printGrid(){
    //go to the begining of the terminal 
    fprintf(stdout, "\033[%d;%dH", 0, 0);
    for(int i=0; i<HEIGHT; i++){
        for(int k=0; k<WIDTH; k++){
            fprintf(stdout, used_grid[i][k].letter);
        }
        fprintf(stdout, "\n");
    }
    fflush(stdout);

}

void makeGraph(){
    srand(time(NULL));
    int random;
    for(int i=0; i<HEIGHT; i++){
        for (int k=0; k<WIDTH; k++){
            random = rand() % 3;
            if(random == 0){
                used_grid[i][k].letter = "⚫️";
                used_grid[i][k].isNode = false;
            }
            else{
                used_grid[i][k].letter = "🔴";
                used_grid[i][k].visited = false;
                used_grid[i][k].isNode = true;
            }
        }
    }
}
void depthFirst(y, x){
    used_grid[y][x].visited = true;
    used_grid[y][x].letter = "🟢";
    printGrid();
    usleep(30000);
    if(y - 1 >= 0){
        if(!used_grid[y-1][x].visited && used_grid[y-1][x].isNode){
            depthFirst(y - 1,x);
        }
    }
    if(y + 1 < HEIGHT){
        if(!used_grid[y+1][x].visited && used_grid[y+1][x].isNode){
            depthFirst(y + 1,x);
        }
    }
    if(x +1 < WIDTH){
        if(!used_grid[y][x+1].visited && used_grid[y][x+1].isNode){
            depthFirst(y,x + 1);
        }
    }
    if(x - 1 >= 0){
        if(!used_grid[y][x-1].visited && used_grid[y][x-1].isNode){
            depthFirst(y,x - 1);
        }
    }
}
