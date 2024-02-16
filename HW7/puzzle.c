#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4
#define NxN (N * N)
#define TRUE 1
#define FALSE 0

struct state
{
    int tiles[N][N];
    int f, g, h;
    short zero_row, zero_column; /* location (row and colum) of blank tile 0 */
    struct state *next;
    struct state *parent; /* used to trace back the solution */
};

int goal_rows[NxN];
int goal_columns[NxN];

struct state *start, *goal;
struct state *fringe = NULL, *closed = NULL;
struct state *succ_states[4];

void print_a_state(struct state *pstate)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            printf("%2d ", pstate->tiles[i][j]);
        printf("\n");
    }
    printf("\n");
}

void initialize(char **argv)
{
    int i, j, k, index, tile;
    start = (struct state *)malloc(sizeof(struct state));
    index = 1;
    for (j = 0; j < N; j++)
        for (k = 0; k < N; k++)
        {
            tile = atoi(argv[index++]);
            start->tiles[j][k] = tile;
            if (tile == 0)
            {
                start->zero_row = j;
                start->zero_column = k;
            }
        }
    start->f = 0;
    start->g = 0;
    start->h = 0;
    start->next = NULL;
    start->parent = NULL;
    printf("initial state\n");
    print_a_state(start);
    goal = (struct state *)malloc(sizeof(struct state));
    goal_rows[0] = 3;
    goal_columns[0] = 3;
    for (index = 1; index < NxN; index++)
    {
        j = (index - 1) / N;
        k = (index - 1) % N;
        goal_rows[index] = j;
        goal_columns[index] = k;
        goal->tiles[j][k] = index;
    }
    goal->tiles[N - 1][N - 1] = 0; /* empty tile=0 */
    goal->f = 0;
    goal->g = 0;
    goal->h = 0;
    goal->next = NULL;
    printf("goal state\n");
    print_a_state(goal);
}

// Merge the states pointed by succ_states[i] to the fringe in order of f value
void merge_to_fringe()
{
    int i;
    struct state *pstate;
    for (i = 0; i < 4; i++)
    {
        pstate = succ_states[i];
        if (pstate != NULL)
        {
            if (fringe == NULL)
            {
                fringe = pstate;
                pstate->next = NULL;
            }
            else
            {
                if (pstate->f < fringe->f)
                {
                    pstate->next = fringe;
                    fringe = pstate;
                }
                else
                {
                    struct state *curr = fringe;
                    while (curr->next != NULL && pstate->f >= curr->next->f)
                        curr = curr->next;
                    pstate->next = curr->next;
                    curr->next = pstate;
                }
            }
        }
    }
}

// Swap the tiles pointed by (row1, column1) and (row2, column2)
void swap(int row1, int column1, int row2, int column2, struct state *pstate)
{
    int tile = pstate->tiles[row1][column1];
    pstate->tiles[row1][column1] = pstate->tiles[row2][column2];
    pstate->tiles[row2][column2] = tile;
}

int cal_h(struct state *pstate) // Manhattan distance
{
    int i, j, value, goal_row, goal_col, h = 0;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            value = pstate->tiles[i][j];
            if (value != 0)
            {
                goal_row = goal_rows[value];
                goal_col = goal_columns[value];
                h += abs(i - goal_row) + abs(j - goal_col);
            }
        }
    }

    return h;
}

// Update f, g, and h values for the state pointed by succ_states[i]
void update_fgh(int i)
{
    struct state *pstate = succ_states[i];

    pstate->h = cal_h(pstate);
    pstate->g = pstate->parent->g + 1;
    pstate->f = pstate->g + pstate->h;
}

/* 0 goes down by a row */
void move_down(struct state *pstate)
{
    swap(pstate->zero_row, pstate->zero_column, pstate->zero_row + 1, pstate->zero_column, pstate);
    pstate->zero_row++;
}
/* 0 goes right by a column */
void move_right(struct state *pstate)
{
    swap(pstate->zero_row, pstate->zero_column, pstate->zero_row, pstate->zero_column + 1, pstate);
    pstate->zero_column++;
}
/* 0 goes up by a row */
void move_up(struct state *pstate)
{
    swap(pstate->zero_row, pstate->zero_column, pstate->zero_row - 1, pstate->zero_column, pstate);
    pstate->zero_row--;
}
/* 0 goes left by a column */
void move_left(struct state *pstate)
{
    swap(pstate->zero_row, pstate->zero_column, pstate->zero_row, pstate->zero_column - 1, pstate);
    pstate->zero_column--;
}

// Generate the four successors of the state pointed by selected
void expand(struct state *selected)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        succ_states[i] = (struct state *)malloc(sizeof(struct state));
        memcpy(succ_states[i], selected, sizeof(struct state));
        succ_states[i]->next = NULL;
        succ_states[i]->parent = selected;
        switch (i)
        {
        case 0:
            if (succ_states[i]->zero_row < N - 1)
            {
                move_down(succ_states[i]);
                break;
            }
            else
            {
                free(succ_states[i]);
                succ_states[i] = NULL;
                break;
            }
        case 1:
            if (succ_states[i]->zero_column < N - 1)
            {
                move_right(succ_states[i]);
                break;
            }
            else
            {
                free(succ_states[i]);
                succ_states[i] = NULL;
                break;
            }
        case 2:
            if (succ_states[i]->zero_row > 0)
            {
                move_up(succ_states[i]);
                break;
            }
            else
            {
                free(succ_states[i]);
                succ_states[i] = NULL;
                break;
            }
        case 3:
            if (succ_states[i]->zero_column > 0)
            {
                move_left(succ_states[i]);
                break;
            }
            else
            {
                free(succ_states[i]);
                succ_states[i] = NULL;
                break;
            }
        }
    }
}

int states_same(struct state *a, struct state *b)
{
    int flg = FALSE;
    if (memcmp(a->tiles, b->tiles, sizeof(int) * NxN) == 0)
        flg = TRUE;
    return flg;
}

// Check if the state pointed by pstate is repeating in the list pointed by pstate_list
void filter(int i, struct state *pstate_list)
{
    struct state *pstate = succ_states[i];

    while (pstate_list != NULL)
    {
        if (states_same(pstate, pstate_list))
        {
            free(pstate); // Free the memory allocated for the repeating state
            succ_states[i] = NULL; // Set the pointer to NULL to avoid using the repeating state
            return;
        }
        pstate_list = pstate_list->next;
    }
}

int main(int argc, char **argv)
{
    int iter, cnt;
    struct state *curr_state, *cp, *solution_path;
    int ret, i, pathlen = 0, index[N - 1];
    solution_path = NULL;
    initialize(argv); /* init initial and goal states */
    fringe = start;
    iter = 0;
    while (fringe != NULL)
    {
        curr_state = fringe;
        fringe = fringe->next; /* get the first state from fringe to expand */
        /* SOME NOTES FOR DEBUGGING:
        * Fix segmentation faults first. If the program cannot produce correct
        results,
        * select an initial layout close to the goal layout, and debug the
        program with it.
        * gdb is recommended. You may also print data on the screen. But this
        is not as
        * convenient as using gdb.
        *
        * If your program does not crash, but the result is not correct, you
        can
        * print the state pointed by curr_state for the list of states
        examined during the search
        *
        */
        if (states_same(curr_state, goal))
        { /* a solution is found */
            do
            { /* trace back and add the states on the path to a list */
                curr_state->next = solution_path;
                solution_path = curr_state;
                curr_state = curr_state->parent;
                pathlen++;
            } while (curr_state != NULL);
            printf("Path (lengh=%d):\n", pathlen);
            curr_state = solution_path;
            // ... /* print out the states on the list */
            break;
        }
        expand(curr_state); /* generate new states */
        for (i = 0; i < 4; i++)
        {
            filter(i, fringe); /* Check succ_states[i] agaist fringe to see if
             it is repeatng */
            filter(i, closed); /* Check succ_states[i] agaist closed to see if
             it is repeatng */
            update_fgh(i);     /* evaluate the state if it is not repeating */
        }
        merge_to_fringe(); /* add new states to fringe */
        curr_state->next = closed;
        closed = curr_state; /* curr_state has been checked/expand, add it
          to closed */
        /* print out something so that you know your program is still making
        progress
        */
        if (iter++ % 1000 == 0)
            printf("iter %d\n", iter);
    }
    return 0;
} /* end of main */