#include "driver.h"
#define MAX_VAR_LIST_SIZE 16

int isTileValid(State *state, int i, int j)
{
    return ((i >= 0 && i < 4 && j >= 0 && j < 4) ? 1 : 0);
}

void debug_print(State *state)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("Value:%d, N:%d ", state->board[i][j].value, state->board[i][j].n);
            for (int k = 0; k < 16; k++)
            {
                printf(" %s ", state->board[i][j].var_list[k]);
            }
            printf("\n");
        }
    }

    printf("\n");
    printf("\n");
}

int isNameValid(State *state, char *var)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < state->board[i][j].n; i++)
            {
                if (strcmp(state->board[i][j].var_list[k], var) == 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}
void print_board(State *state)
{

    printf("_ _ _ _ _ _ _ _ _\n");

    for (int i = 0; i < 4; i++)
    {
        printf("| ");
        for (int j = 0; j < 4; j++)
        {
            if (state->board[i][j].value)
            {
                printf("%d ", state->board[i][j].value);
            }
            else
            {
                printf("  ");
            }

            printf("| ");
        }
        printf("\n_ _ _ _ _ _ _ _ _\n");
    }
}

void random_tile(State *state)
{
    //allot random tile to board anywhere
    int p, i, j;
    int count = 0;
    srand(time(NULL));
    do
    {
        p = rand() % 16;
        i = p / 4;
        j = p % 4;

    } while (state->board[i][j].value != 0 && (++count) <= 16);

    state->board[i][j].value = ((rand() % 2) ? 2 : 4); //50% chance for 2 or 50% chance for 4

    if (count > 16)
    {
        printf("MOVE NOT POSSIBLE! GAME OVER!\n");
        print_board(state);
        exit(0);
    }
}

void init_board(State *state)
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state->board[i][j].value = 0;
            state->board[i][j].n = 0;
            state->board[i][j].var_list = (char **)malloc((MAX_VAR_LIST_SIZE) * (sizeof(char *)));
        }
    }
}

State *compress(State *state, int *changed)
{

    // bool variable to determine if change happened or not

    //***************
    //empty grid
    // State newState;
    // init_board(&newState);

    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    // print_board(&newState);
    // here we will shift entries
    // of each cell to it's extreme 0 0 0 0 =>
    // left row by row
    // loop to traverse rows

    for (int i = 0; i < 4; i++)
    {
        int pos = 0;
        for (int j = 0; j < 4; j++)
        {
            if (state->board[i][j].value != 0)
            {
                // if cell is non empty then
                // we will shift it's number to
                // previous empty cell in that row
                // denoted by pos variable
                ans->board[i][pos] = state->board[i][j];

                //***************
                if (j != pos)
                {
                    *changed = 1;
                }
                pos = pos + 1;
            }
        }
    }

    // print_board(state);
    // ans = &newState;
    // print_board(ans);
    return ans;
}

State *merge(State *state, int *changed, int operation)
{
    // printf("MERGE");
    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);
    // int changed = 0;
    //****************
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // if current cell has same value as
            // next cell in the row and they
            // are non empty then
            if (state->board[i][j].value == state->board[i][j + 1].value && state->board[i][j].value != 0)
            {
                //double current cell value and
                //empty the next cell
                if (operation == 0)
                { //ADD
                    state->board[i][j].value += state->board[i][j + 1].value;
                    state->board[i][j + 1].value = 0;

                    if (state->board[i][j + 1].n != 0)
                    {
                        for (int c = 0; c < state->board[i][j + 1].n; c++)
                        {
                            int index = state->board[i][j].n;
                            state->board[i][j].var_list[index] = state->board[i][j + 1].var_list[c];
                            state->board[i][j].n++;
                        }

                        state->board[i][j + 1].var_list = (char **)malloc((MAX_VAR_LIST_SIZE) * (sizeof(char *)));
                        state->board[i][j + 1].n = 0;
                    }
                }
                else if (operation == 1)
                { //SUB
                    state->board[i][j].value -= state->board[i][j + 1].value;
                    state->board[i][j + 1].value = 0;

                    if (state->board[i][j + 1].n != 0)
                    {
                        for (int c = 0; c < state->board[i][j + 1].n; c++)
                        {
                            int index = state->board[i][j].n;
                            state->board[i][j].var_list[index] = state->board[i][j + 1].var_list[c];
                            state->board[i][j].n++;
                        }
                    }

                    state->board[i][j + 1].var_list = (char **)malloc((MAX_VAR_LIST_SIZE) * (sizeof(char *)));
                    state->board[i][j + 1].n = 0;

                    // state->board[i][j].var_list = malloc((MAX_VAR_LIST_SIZE) * (sizeof(char *)));
                    // state->board[i][j].n = 0;
                }
                else if (operation == 2)
                { //MULTIPLY
                    state->board[i][j].value *= state->board[i][j + 1].value;
                    state->board[i][j + 1].value = 0;

                    if (state->board[i][j + 1].n != 0)
                    {
                        for (int c = 0; c < state->board[i][j + 1].n; c++)
                        {
                            int index = state->board[i][j].n;
                            state->board[i][j].var_list[index] = state->board[i][j + 1].var_list[c];
                            state->board[i][j].n++;
                        }
                    }

                    state->board[i][j + 1].var_list = (char **)malloc((MAX_VAR_LIST_SIZE) * (sizeof(char *)));

                    state->board[i][j + 1].n = 0;
                }
                else
                { //DIV
                    state->board[i][j].value /= state->board[i][j + 1].value;
                    state->board[i][j + 1].value = 0;

                    if (state->board[i][j + 1].n != 0)
                    {
                        for (int c = 0; c < state->board[i][j + 1].n; c++)
                        {
                            int index = state->board[i][j].n;
                            state->board[i][j].var_list[index] = state->board[i][j + 1].var_list[c];
                            state->board[i][j].n++;
                        }
                    }

                    state->board[i][j + 1].var_list = (char **)malloc((MAX_VAR_LIST_SIZE) * (sizeof(char *)));
                    state->board[i][j + 1].n = 0;
                }

                //make bool variable True indicating
                //the new grid after merging is
                //different.
                *changed = 1;
            }
        }
    }

    ans = state;
    return ans;
}

State *reverse(State *state)
{

    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ans->board[i][j] = state->board[i][3 - j];
        }
    }

    return ans;
}

// //purely struct
State *transpose(State *state)
{
    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ans->board[i][j] = state->board[j][i];
        }
    }

    return ans;
}

State *move_left(State *state, int operation)
{

    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    int changed1 = 0;
    int changed2 = 0;

    ans = compress(state, &changed1);
    // print_board(ans);

    ans = merge(ans, &changed2, operation);
    // print_board(ans);

    int changed = changed1 || changed2;

    ans = compress(ans, &changed);
    // print_board(ans);

    random_tile(ans);

    return ans;
}

State *move_right(State *state, int operation)
{
    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    ans = reverse(state);

    ans = move_left(ans, operation);

    ans = reverse(ans);

    // random_tile(ans);

    return ans;
}

State *move_up(State *state, int operation)
{
    //function to update the matrix
    //if we move / swipe up
    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    ans = transpose(state);

    ans = move_left(ans, operation);

    ans = transpose(ans);
    // return changed;
    // random_tile(ans);

    return ans;
}

// //function to update the matrix
// //if we move / swipe down
State *move_down(State *state, int operation)
{
    //function to update the matrix
    //if we move / swipe up

    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    ans = transpose(state);

    ans = move_right(ans, operation);

    ans = transpose(ans);
    // return changed;

    // random_tile(ans);

    return ans;
}

State *assign_var_name(State *state, int *err, int i, int j, char *var)
{
    if (!isTileValid(state, i, j))
    {
        printf("2048> ERROR: There is no tile like that. The tile co-ordinates must be in the range 1,2,3,4.\n");
        *err = 1;
        // fprintf(stderr, "-1\n");
        return state;
    }
    else if (state->board[i][j].value == 0)
    {
        printf("2048> ERROR: Tile is currently empty! Cannot be assigned a name !\n");
        *err = 1;
        // fprintf(stderr, "-1\n");
        return state;
    }
    else if (!isNameValid(state, var))
    {

        printf("2048> ERROR: Variable name already assigned! Please enter a different name for tile.\n");
        *err = 1;
        // fprintf(stderr, "-1\n");
        return state;
    }

    State *ans = (State *)malloc(sizeof(State));
    init_board(ans);

    int index = state->board[i][j].n;
    state->board[i][j].var_list[index] = var;
    state->board[i][j].n++;

    return (ans = state);
}

State *set_value(State *state, int *err, int i, int j, int val)
{
    if (!isTileValid(state, i, j))
    {
        printf("2048> ERROR: There is no tile like that. The tile co-ordinates must be in the range 1,2,3,4.\n");
        *err = 1;
        // fprintf(stderr, "-1\n");
        return state;
    }
    else if (val < 0)
    {
        printf("2048> ERROR: Please pass positive values to tile. Negative values not allowed.\n");
        *err = 1;
        // fprintf(stderr, "-1\n");
        return state;
    }
    else if(val == 0){
    state->board[i][j].value = 0;
    state->board[i][j].n = 0;
    state->board[i][j].var_list = (char **)malloc((MAX_VAR_LIST_SIZE) * (sizeof(char *)));


    }

    state->board[i][j].value = val;
    return state;
}

int get_value(State *state, int *err, int i, int j)
{
    if (!isTileValid(state, i, j))
    {
        printf("2048> ERROR: There is no tile like that. The tile co-ordinates must be in the range 1,2,3,4.\n");
        // fprintf(stderr, "-1\n");
        *err = 1;
        return -1;
    }
    else
    {
        *err = 5;
        return state->board[i][j].value;
    }
}

void print_error_board(State *state)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(stderr, "%d ", state->board[i][j].value);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int index = state->board[i][j].n;
            if (index != 0)
            {
                fprintf(stderr, "%d,%d", i + 1, j + 1);
                for (int k = 0; k < index; k++)
                {
                    if (k <= index - 2)
                        fprintf(stderr, "%s,", state->board[i][j].var_list[k]);
                    else
                    {
                        fprintf(stderr, "%s ", state->board[i][j].var_list[k]);
                    }
                }
            }
        }
    }

    fprintf(stderr, "\n");
}
