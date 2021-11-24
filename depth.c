#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HEIGHT 40
#define WIDTH 60
//there can never be more than this many discovered nodes, in reality much smaller
#define QUELENGTH (((HEIGHT*WIDTH) / 2) + 1)

typedef struct {
    char *letter;
    bool visited;
    bool isNode;
    //only used by breadth first
    bool done;
} grid;

typedef struct{
    int x;
    int y;
} QUE;

//initialize grid
grid used_grid[HEIGHT][WIDTH];

QUE que[QUELENGTH];
int tail;
int head;


void breadthFirst(int x, int y);
void depthFirst(int x, int y);

int main(int argc, char *argv[]){
    
    //check for options
    bool infiniteMode = false;
    bool help = false;
    bool breadth = false;

    int (*funcPtr)(int, int);
    if (argc > 1){
        for (int i=1; i<argc; i++){
            if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--infinite") == 0 ) infiniteMode = true;
            else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) help = true; 
            else if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--breadth") == 0) breadth = true; 
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
        printf("-b, %-20s %s\n","--breadth", "run breadth first traversal");
        printf("-h, %-20s %s\n","--help", "display help");
        exit(0);
    }
    if(breadth) funcPtr = &breadthFirst;     
    else{
        funcPtr = &depthFirst;
    }
    while(infiniteMode){
        fprintf(stdout, "\033[H\033[J");
        makeGraph();
        (*funcPtr)(0, 0);
        sleep(1);
    }
    /* clear the console */    
    fprintf(stdout, "\033[H\033[J");
    makeGraph();
    (*funcPtr)(0, 0);
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
    usleep(30000);

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
    // 0, 0 is always a node
    used_grid[0][0].letter = "🔴";
    used_grid[0][0].visited = false;
    used_grid[0][0].isNode = true;

}
void depthFirst(y, x){
    used_grid[y][x].visited = true;
    used_grid[y][x].letter = "🟢";
    printGrid();
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
void enque(y, x){
    que[head].x = x;
    que[head].y = y;
    head = (head +1) % QUELENGTH;
}
QUE deque(){
    QUE returnValue;
    if (tail == head){
        que[tail].x = -1;
    }
    returnValue = que[tail];
    tail = (tail + 1) % QUELENGTH;
    return returnValue;
}
void breadthFirst(x, y){
    used_grid[x][y].visited = true;
    QUE nextNode;

    discoverNodes(x, y);
    while(true){
        nextNode = deque();
        if(nextNode.x == -1){
            break;
        }
        discoverNodes(nextNode.y, nextNode.x);
    }
}
void discoverNodes(y, x){
    int newY;
    int newX;
    if(y - 1 >= 0){
        newY = y-1;
        if(!used_grid[newY][x].visited && used_grid[newY][x].isNode){
            used_grid[newY][x].visited = true;
            used_grid[newY][x].letter = "🔵";
            enque(newY, x);
            printGrid();
        }
    }
    if(y + 1 < HEIGHT){
        newY = y+1;
        if(!used_grid[newY][x].visited && used_grid[newY][x].isNode){
            used_grid[newY][x].visited = true;
            used_grid[newY][x].letter = "🔵";
            enque(newY, x);
            printGrid();
        }
    }
    if(x +1 < WIDTH){
        newX = x+1;
        if(!used_grid[y][newX].visited && used_grid[y][newX].isNode){
            used_grid[y][newX].visited = true;
            used_grid[y][newX].letter = "🔵";
            enque(y, newX);
            printGrid();
        }
    }
    if(x - 1 >= 0){
        newX = x-1;
        if(!used_grid[y][newX].visited && used_grid[y][newX].isNode){
            used_grid[y][newX].visited = true;
            used_grid[y][newX].letter = "🔵";
            enque(y, newX);
            printGrid();
        }
    }
    used_grid[y][x].letter = "🟢";
    printGrid();
}
