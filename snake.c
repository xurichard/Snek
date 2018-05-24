#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Board{
    int height;
    int width;
    char board[10][10];
};

typedef struct node {
    int x;
    int y;
    struct node * next;
} node_t;

void printBoard(struct Board b){
    for(int i = 0; i < b.width; i++){
        for(int j = 0; j < b.height; j++){
            printf("| %c ", b.board[j][i]);
        }
        printf("|\n");
    }
}

enum directions{North, South, East, West};
struct Snake{
    enum directions direction;
    int length;
    node_t * head;
};

// |   |   |   |   |   | * |
// |   | o | o | o | O |   |
//                  head
//
//

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

    /* now we can add a new variable */
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
    /* if there is only one item in the list, remove it */
    if (head->next == NULL) {
        free(head);
    }

    /* get to the second to last node in the list */
    node_t * current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    /* now current points to the second to last item of the list, so let's remove current->next */
    free(current->next);
    current->next = NULL;
}


int main()
{
    struct Board myBoard;
    myBoard.height = 10;
    myBoard.width = 10;
    // myBoard already allocated
    for(int i = 0; i < myBoard.height; i++){
        for(int j = 0; j < myBoard.width; j++){
            myBoard.board[i][j] = ' ';
        }
    }

    struct Snake mySnake;
    mySnake.length = 1;
    mySnake.direction = North;
    mySnake.head = malloc(sizeof(node_t));
    mySnake.head->y = myBoard.height/2;
    mySnake.head->x = myBoard.width/2;
    mySnake.head->next = NULL;

    bool playing = true;
    char input;
    while(playing){
        //add snake to board then clear board
        node_t * current = mySnake.head;
        while (current != NULL) {
            //printf("%d\n", current->x);
            myBoard.board[current->x][current->y] = 'o';
            current = current->next;
        }
        printBoard(myBoard);
        for(int i = 0; i < myBoard.height; i++){
            for(int j = 0; j < myBoard.width; j++){
                myBoard.board[i][j] = ' ';
            }
        }

        int nextx = mySnake.head->x;
        int nexty = mySnake.head->y;
        //update snake - push new node onto head, pop off tail
        if(mySnake.direction == South){
            nexty = mySnake.head->y + 1;
        } else if(mySnake.direction == North){
            nexty = mySnake.head->y - 1;
        } else if(mySnake.direction == East){
            nextx = mySnake.head->x + 1;
        } else{
            nextx = mySnake.head->x - 1;
        }

        if(nextx >= myBoard.width || nextx < 0 || nexty >= myBoard.height || nexty < 0){
            playing = false;
        } else{
            push(&mySnake.head, nextx, nexty);
            remove_last(mySnake.head);
        }

        scanf(" %c", &input);
        if(input == 'w'){
            mySnake.direction = North;
        } else if(input == 'a'){
            mySnake.direction = West;
        } else if(input == 's'){
            mySnake.direction = South;
        } else if(input == 'd'){
            mySnake.direction = East;
        }
    }

    puts("I think you lost");
}






