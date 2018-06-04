#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
//gcc curses.c -lncurses -o curses
#define FINAL_DELAY 70000

typedef struct node {
    int x;
    int y;
    struct node * next;
} node_t;

// push to beginning of list
void push(node_t ** head, int x, int y) {
    node_t * new_node;
    new_node = malloc(sizeof(node_t));

    new_node->x = x;
    new_node->y = y;
    new_node->next = *head;
    *head = new_node;
}

//push to end
void push_end(node_t * head, int x, int y) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = malloc(sizeof(node_t));
    current->next->x = x;
    current->next->y = y;
    current->next->next = NULL;
}

//remove first item
void pop(node_t ** head) {
    node_t * next_node = NULL;

    if (*head == NULL) {
        return;
    } 
    next_node = (*head)->next;
    free(*head);
    *head = next_node;
}

// remove last item
void remove_last(node_t * head) {
    if (head->next == NULL) {
        free(head);
    }
    node_t * current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

enum direction{NORTH, SOUTH, EAST, WEST};

struct Snake{
    enum direction direction;
    int length;
    node_t * head;
};

void kbhit(int wait_us)
{
    struct timespec start, end;
    int ch;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    while(delta_us < wait_us){
        ch = getch();
        if (ch != ERR) {
            usleep(wait_us*1.1 - delta_us);
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    }
    ungetch(ch);
    // return ch;
}

int main()
{   
    initscr();          /* Start curses mode          */
    noecho(); // Don't echo any keypresses
    keypad(stdscr, TRUE);
    curs_set(FALSE); // Don't display a cursor
    nodelay(stdscr, true);
    cbreak();

    int x = 0, y = 0;
    int max_y = 0, max_x = 0;
    int direction_x = 1;
    int direction_y = 0;
    getmaxyx(stdscr, max_y, max_x);

    struct Snake mySnake;
    mySnake.direction = EAST;
    mySnake.head = malloc(sizeof(node_t));
    mySnake.head->y = max_y/2;
    mySnake.head->x = max_x/2;
    mySnake.head->next = NULL;

    bool playing = true;
    int DELAY = 150000;

    int score = 0;

    srand(time(NULL));
    int food_x = rand()%max_x;
    int food_y = rand()%max_y;

    do{
        usleep(DELAY);

        // kbhit(10000); get input
        switch(getch()){
            case 'w':
                if(mySnake.direction != SOUTH){
                    mySnake.direction = NORTH;
                    direction_x = 0;
                    direction_y = -1;
                }
                break;
            case 's':
                if(mySnake.direction != NORTH){
                    mySnake.direction = SOUTH;
                    direction_x = 0;
                    direction_y = 1;
                }
                break;
            case 'a':
                if(mySnake.direction != EAST){
                    mySnake.direction = WEST;
                    direction_x = -1;
                    direction_y = 0;
                }
                break;
            case 'd':
                if(mySnake.direction != WEST){
                    mySnake.direction = EAST;
                    direction_x = 1;
                    direction_y = 0;
                }
                break;
            default:
                break;
        }

        int next_x = mySnake.head->x + direction_x;
        int next_y = mySnake.head->y + direction_y;

        //detect self collision in here, shoudl be at the end, but here for efficiency
        node_t * current = mySnake.head;
        while (current != NULL) {
            if(current->y == next_y && current->x == next_x){
                playing = false;
                break;
            }
            current = current->next;
        }

        if(next_y < 0 || next_y >= max_y || next_x < 0 || next_x >= max_x){
            //collision with wall
            playing = false;
        }

        push(&mySnake.head, next_x, next_y);
        if(next_x == food_x && next_y == food_y){
            //increase score, new food
            score += 100;
            food_x = rand()%max_x;
            food_y = rand()%max_y;
            DELAY = FINAL_DELAY + (DELAY - FINAL_DELAY)*.9;
        }else{
            remove_last(mySnake.head);
        }

        clear();
        //print snake
        current = mySnake.head;
        while (current != NULL) {
            mvprintw(current->y, current->x, "o");
            current = current->next;
        }

        //print food
        mvprintw(food_y, food_x, "x");

        //print score
        char snum[12];
        sprintf(snum, "%d", score);
        mvprintw(1, max_x-6, &snum[0]);

        refresh();
    }while(playing);
    endwin();           /* End curses mode        */

    int food_count = -1; //don't count the initial head
    node_t * current = mySnake.head;
    while (current != NULL) {
        current = current->next;
        food_count ++;
    }
    printf("Congraduations, you ate %d food and had a final score of %d\n", food_count, score);
    return 0;
}













