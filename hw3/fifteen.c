/**
 * fifteen.c
 *
 * Refer to Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

#ifndef INT_MAX
    #define INT_MAX 12345678
#endif // INT_MAX

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void greet(void);
void init(void);
void draw(void);
short move(int tile);
short won(void);
int get_int();

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        printf("\n");

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Get an non-negative integer from user input
 * If the input is not non-negative integer, return INT_MAX
 */
 int get_int()
 {
    int input = 0;
    short invalid = 0;

    char c = getchar();
    if (c == '\n')
        return INT_MAX;

    while (c != '\n') 
    {
        if ((c >= '0') && ( c <= '9'))
        {
            input = input * 10 + (c - '0');
        } else {
            invalid = 1;
        }

        c = getchar();
    }

    if (invalid)
        return INT_MAX;
    else 
        return input;
 }

/**
 * Greets player.
 */
void greet(void)
{    
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int squareVal = d*d - 1;
    int dTotal = d*d;

    for(int row = 0; row < d; row++) {
        for(int col = 0; col < d; col++) {

            board[row][col] = squareVal;
            squareVal--;
        }
    }


    if(dTotal % 2 == 0) {
        //makes the second tile from the gap have a value of 1
        board[d-1][d-3] = 1;
        //makes the tile right to the left of the gap have a value of 2
        board[d-1][d-2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{

    for (int i = 0; i < d; i++) {
        for(int j = 0; j < d; j++) {

            if(board[i][j] == 0) {
                printf(" __");
            }
            
            else {
                printf("%4d", board[i][j]);
            }
        }

        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns 1, else
 * returns 0.
 */
short move(int tile)
{
    if(tile > d*d - 1) {
        return 0;
    }

    if(tile < 1) {
        return 0;
    }
    int rowLoc = 0;
    int colLoc = 0;
    for (int row = 0; row < d; row++) {
        for (int col = 0; col < d; col++) {

            if(board[row][col] == tile) {
                rowLoc = row;
                colLoc = col;
            }
        }
    }


    if(rowLoc + 1 < d && board[rowLoc + 1][colLoc] == 0) {
        board[rowLoc + 1][colLoc] = board[rowLoc][colLoc];
        board[rowLoc][colLoc] = 0;
        return 1;
    }
    else if (colLoc + 1 < d && board[rowLoc][colLoc + 1] == 0) {
        board[rowLoc][colLoc + 1] = board[rowLoc][colLoc];
        board[rowLoc][colLoc] = 0;
        return 1;
    } 
    else if (colLoc - 1 >= 0 && board[rowLoc][colLoc - 1] == 0) {
        board[rowLoc][colLoc - 1] = board[rowLoc][colLoc];
        board[rowLoc][colLoc] = 0;
        return 1;
    }
    else if(rowLoc - 1 >= 0 && board[rowLoc - 1][colLoc] == 0) {
        board[rowLoc - 1][colLoc] = board[rowLoc][colLoc];
        board[rowLoc][colLoc] = 0;
        return 1;
    }

    return 0;
}

/**
 * Returns 1 if game is won (i.e., board is in winning configuration),
 * else 0.
 */
short won(void)
{
    int winChecker = 1;
    int maxTiles = d*d;

    for(int i = 0; i < d; i++) {
        for(int j = 0; j < d; j++) {
            if(winChecker != maxTiles && board[i][j] != winChecker) {
                return 0;
            }
            winChecker++;
        }
    }
    return 1;
}
