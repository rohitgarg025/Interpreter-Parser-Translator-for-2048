#ifndef _DRIVER_H
#define _DRIVER_H

#define MAX_VAR_LIST_SIZE 16
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>    // time()

typedef struct
{
    int value;       //value of tile
    int n;           //number of varnames
    char **var_list; //list of varnames

} Tile;

typedef struct
{
    Tile board[4][4];
} State;

extern int isTileValid(State* state,int i,int j);

extern int isNameValid(State *state, char *var);

extern void print_board(State *state);

extern void random_tile(State *state);

extern void init_board(State *state);

extern State *compress(State *state, int *changed);

extern State *merge(State *state, int *changed, int operation);

extern State *reverse(State *state);

extern State *transpose(State *state);

extern State *move_left(State *state, int operation);

extern State *move_right(State *state, int operation);

extern State *move_up(State *state, int operation);

extern State *move_down(State *state, int operation);

extern State *assign_var_name(State *state, int *err, int i, int j, char *var);

extern State *set_value(State *state, int *err, int i, int j, int val);

extern int get_value(State *state,int* err, int i, int j);

extern void print_error_board(State* state);


#endif