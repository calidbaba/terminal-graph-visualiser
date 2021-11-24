#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>

//there can never be more than this many discovered nodes, in reality much smaller

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

grid **used_grid;
//height and width of grid
unsigned short height;
unsigned short width;
int queLength;

QUE *que;
int tail;
int head;

//how long the program sleeps between redraws
int sleepTime;


//emojis used
char *node;
char *discovered;
char *wall;
char *finished;

void breadthFirst(int x, int y);
void depthFirst(int x, int y);

int initialize_grid(ascii){
    struct winsize w;
    int result;

    result = ioctl(0, TIOCGWINSZ, &w);
    
    if(result != 0){
        return 1;
    }
    if (ascii){
        height = w.ws_row - 1;    
        width = w.ws_col;
    }
    else{
        height = w.ws_row - 1;    
        // when using emojeys width must be cut in 2 as emojis use 2 cols
        width = w.ws_col/2;
    }
    queLength = (height* width)/2 +1;
    used_grid = calloc(height, sizeof(grid));
    if (used_grid == NULL){
        puts("something went wrong with heap allocation");
    }
    for (int i=0; i<height; i++){
        used_grid[i] = calloc(width, sizeof(grid));
        if (used_grid[i] == NULL){
            puts("something went wrong with heap allocation");
        }
    }
    return 0;
}

int main(int argc, char *argv[]){
    
    int result;

    //check for options
    bool infiniteMode = false;
    bool help = false;
    bool breadth = false;
    bool fast = false;
    bool ascii = false;

    int (*funcPtr)(int, int);
    if (argc > 1){
        for (int i=1; i<argc; i++){
            if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--infinite") == 0 ) infiniteMode = true;
            else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) help = true; 
            else if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--breadth") == 0) breadth = true; 
            else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--speed") == 0) fast = true; 
            else if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--ascii") == 0) ascii = true; 
            else{
                printf("%s: invalid option %s\n", argv[0], argv[i]);
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
        printf("-s, %-20s %s\n","--speed", "speed it up");
        printf("-a, %-20s %s\n","--ascii", "ascii mode");
        printf("-h, %-20s %s\n","--help", "display help");
        exit(0);
    }
    result = initialize_grid();
    if(breadth){
        funcPtr = &breadthFirst;     
        que = calloc(queLength, sizeof(QUE));
        if(que == NULL){
            puts("Someting went wrong with heap allocation");
        }
    }
    else{
        funcPtr = &depthFirst;
    }
    if(result){
        puts("Someting went wrong in intiliazing the terminal");
        exit(1);
    }
    if(fast){
        sleepTime = 3000;
    }
    else{
        sleepTime = 30000;
    }
    if(ascii){
        node = "0";
        wall = "#";
        discovered = "*";
        finished = "=";
    }
    else{
        node = "🔴";
        wall = "⚫️";
        discovered = "🔵";
        finished = "🟢";
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
    for(int i=0; i<height; i++){
        for(int k=0; k<width; k++){
            fprintf(stdout, used_grid[i][k].letter);
        }
        fprintf(stdout, "\n");
    }
    fflush(stdout);
    usleep(sleepTime);

}

void makeGraph(){
    srand(time(NULL));
    int random;
    for(int i=0; i<height; i++){
        for (int k=0; k<width; k++){
            random = rand() % 3;
            if(random == 0){
                used_grid[i][k].letter = wall;
                used_grid[i][k].isNode = false;
            }
            else{
                used_grid[i][k].letter = node;
                used_grid[i][k].visited = false;
                used_grid[i][k].isNode = true;
            }
        }
    }
    // 0, 0 is always a node
    used_grid[0][0].letter = node;
    used_grid[0][0].visited = false;
    used_grid[0][0].isNode = true;

}
void depthFirst(y, x){
    used_grid[y][x].visited = true;
    used_grid[y][x].letter = finished;
    printGrid();
    if(y - 1 >= 0){
        if(!used_grid[y-1][x].visited && used_grid[y-1][x].isNode){
            depthFirst(y - 1,x);
        }
    }
    if(y + 1 < height){
        if(!used_grid[y+1][x].visited && used_grid[y+1][x].isNode){
            depthFirst(y + 1,x);
        }
    }
    if(x +1 < width){
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
    head = (head +1) % queLength;
}
QUE deque(){
    QUE returnValue;
    if (tail == head){
        que[tail].x = -1;
    }
    returnValue = que[tail];
    tail = (tail + 1) % queLength;
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
            used_grid[newY][x].letter = discovered;
            enque(newY, x);
            printGrid();
        }
    }
    if(y + 1 < height){
        newY = y+1;
        if(!used_grid[newY][x].visited && used_grid[newY][x].isNode){
            used_grid[newY][x].visited = true;
            used_grid[newY][x].letter = discovered;
            enque(newY, x);
            printGrid();
        }
    }
    if(x +1 < width){
        newX = x+1;
        if(!used_grid[y][newX].visited && used_grid[y][newX].isNode){
            used_grid[y][newX].visited = true;
            used_grid[y][newX].letter = discovered;
            enque(y, newX);
            printGrid();
        }
    }
    if(x - 1 >= 0){
        newX = x-1;
        if(!used_grid[y][newX].visited && used_grid[y][newX].isNode){
            used_grid[y][newX].visited = true;
            used_grid[y][newX].letter = discovered;
            enque(y, newX);
            printGrid();
        }
    }
    used_grid[y][x].letter = finished;
    printGrid();
}
