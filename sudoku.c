/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Michael Bishara 1042715
   Dated:     08/09/2021

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3        /* sudoku dimension, size of each inner square */
#define NDIG (NDIM*NDIM)
/* total number of values in each row */
#define NGRP 3        /* number of sets each cell is a member of */
#define NSET (NGRP*NDIG)
/* total number of sets in the sudoku */
#define NCLL (NDIG*NDIG)
/* total number of cells in the sudoku */

#define ERROR    (-1)    /* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being completely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM * NDIM] = {
        /* the first group of nine sets describe the sudoku's rows */
        {0,  1,  2,  3,  4,  5,  6,  7,  8},
        {9,  10, 11, 12, 13, 14, 15, 16, 17},
        {18, 19, 20, 21, 22, 23, 24, 25, 26},
        {27, 28, 29, 30, 31, 32, 33, 34, 35},
        {36, 37, 38, 39, 40, 41, 42, 43, 44},
        {45, 46, 47, 48, 49, 50, 51, 52, 53},
        {54, 55, 56, 57, 58, 59, 60, 61, 62},
        {63, 64, 65, 66, 67, 68, 69, 70, 71},
        {72, 73, 74, 75, 76, 77, 78, 79, 80},
        /* the second group of nine sets describes the sudoku's columns */
        {0,  9,  18, 27, 36, 45, 54, 63, 72},
        {1,  10, 19, 28, 37, 46, 55, 64, 73},
        {2,  11, 20, 29, 38, 47, 56, 65, 74},
        {3,  12, 21, 30, 39, 48, 57, 66, 75},
        {4,  13, 22, 31, 40, 49, 58, 67, 76},
        {5,  14, 23, 32, 41, 50, 59, 68, 77},
        {6,  15, 24, 33, 42, 51, 60, 69, 78},
        {7,  16, 25, 34, 43, 52, 61, 70, 79},
        {8,  17, 26, 35, 44, 53, 62, 71, 80},
        /* the last group of nine sets describes the inner squares */
        {0,  1,  2,  9,  10, 11, 18, 19, 20},
        {3,  4,  5,  12, 13, 14, 21, 22, 23},
        {6,  7,  8,  15, 16, 17, 24, 25, 26},
        {27, 28, 29, 36, 37, 38, 45, 46, 47},
        {30, 31, 32, 39, 40, 41, 48, 49, 50},
        {33, 34, 35, 42, 43, 44, 51, 52, 53},
        {54, 55, 56, 63, 64, 65, 72, 73, 74},
        {57, 58, 59, 66, 67, 68, 75, 76, 77},
        {60, 61, 62, 69, 70, 71, 78, 79, 80},
};


/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function 
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void
fill_c2s() {
    int s = 0, d = 0, c;
    for (; s < NSET; s++) {
        /* record the first set number each cell is part of */
        for (c = 0; c < NDIM * NDIM; c++) {
            c2s[s2c[s][c]][d] = s;
        }
        if ((s + 1) % (NGRP * NDIM) == 0) {
            d++;
        }
    }
#if 0
    /* this code available here if you want to see the array
       cs2[][] that gets created, just change that 0 two lines back
       to a 1 and recompile */
    for (c=0; c<NCLL; c++) {
        printf("cell %2d: sets ", c);
        for (s=0; s<NGRP; s++) {
            printf("%3d", c2s[c][s]);
        }
        printf("\n");
    }
    printf("\n");
#endif
    return;
}

/* find the row number a cell is in, counting from 1
*/
int
rownum(int c) {
    return 1 + (c / (NDIM * NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int
colnum(int c) {
    return 1 + (c % (NDIM * NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int
sqrnum(int c) {
    return 1 + 3 * (c / NSET) + (c / NDIM) % NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration 
*/


/****************************************************************/
#define VERT_DIV "| "
#define HOR_DIV "------+-------+------\n"
#define TRUE 1
#define FALSE 0


void solve(int **arr, int sol, int pos);

int strategy_one(int **arr, int n, int *unknown);

void print_grid(int arr[], int n);

int count_unknown(const int arr[], int n);

int authenticate(int const arr[]);

void print_violation(int s, int count, int n);

char *get_group_type(int s);

void print_assign(int c, int n);

int to_set(int s);

int get_group_num(int s);


/****************************************************************/

/* main program controls all the action
*/
int
main(int argc, char *argv[]) {
    int arr[NCLL], n = 0, unknown;
    int *arr_ptr = arr, *unknown_ptr = &unknown;

    /* STAGE 1 */
    /* Get values from stdin */
    while (scanf("%d", &arr[n]) == 1)
        n++;
    printf("\n");
    print_grid(arr, n);
    unknown = count_unknown(arr, n);
    printf("\n%2d cells are unknown\n\n", unknown);

    /* STAGE 2 */
    fill_c2s();
    if (authenticate(arr)) {
        /* STAGE 3 */
        while (strategy_one(&arr_ptr, n, unknown_ptr) > 0);
        print_grid(arr, n);
    }

    /* Moment of truth... */
    if (unknown == 0) {
        printf("\n%2d cells are unknown\n\n", unknown);
        printf("ta daa!!!\n");
    }

    /* algorithms are fun */
    /* all done, so pack up bat and ball and head home */
    return 0;
}

int strategy_one(int **arr, int n, int *unknown) {
    int c, g, i, s, j, count, found = FALSE, assign;
    int *arr_ptr = *arr;

    /* Store solutions while printing */
    int results[2][*unknown], total_results = 0;

    printf("strategy one\n");

    for (c = 0; c < n; c++) {
        if (arr_ptr[c] == 0) {
            count = 0;
            for (s = 1; s < NDIG + 1; s++) {
                found = FALSE;
                for (g = 0; g < NGRP && !found; g++) {
                    for (i = 0; i < NDIG && !found; i++) {
                        if (arr_ptr[s2c[c2s[c][g]][i]] == s) {
                            found = TRUE;
                        }
                    }
                }
                if (!found) {
                    count++;
                    assign = s;
                }
            }
            if (count == 1) {
                print_assign(c, assign);
                --*unknown;
                results[0][total_results] = c;
                results[1][total_results++] = assign;
            }
        }
    }
    printf("\n");

    /* Solve each found solution */
    for (j = 0; j < total_results; j++) {
        solve(&arr_ptr, results[1][j], results[0][j]);
    }

    return *unknown;
}

/* Solve the sudoku with the solution and the cell position */
void solve(int **arr, int sol, int pos) {
    int *arr_ptr = *arr;
    arr_ptr[pos] = sol;
}


/* Grid formatting printed at intervals */
void print_grid(int arr[], int n) {
    int i;

    for (i = 0; i < n; i++) {
        /* Replace 0's with '.'*/
        if (arr[i] != 0)
            printf("%d", arr[i]);
        else
            printf("%c", '.');

        /* Print grid formatting */

        /* Space after value */
        if ((i + 1) % 1 == 0 && (i + 1) % 9 != 0)
            printf(" ");
        /* Vertical dividers */
        if ((i + 1) % 3 == 0 && (i + 1) % 9 != 0)
            printf(VERT_DIV);
        /* New line */
        if ((i + 1) % 9 == 0)
            printf("\n");
        /* Horizontal divider */
        if ((i + 1) % 27 == 0 && i < 80)
            printf(HOR_DIV);
    }
}

/* Counts 0's in array */
int count_unknown(const int arr[], int n) {
    int i, count = 0;

    for (i = 0; i < n; i++) {
        if (arr[i] == 0)
            count++;
    }
    return count;
}

/* Scans Sudoku values for structural violations */
int authenticate(int const arr[]) {
    int s, j, i, count, set_violation = 0, total_violations = 0,
            set_dup = FALSE, auth = TRUE;

    for (s = 0; s < NSET; s++) {
        set_dup = FALSE;
        for (j = 1; j < NDIG + 1; j++) {
            count = 0;
            for (i = 0; i < NDIG; i++)
                if (arr[s2c[s][i]] == j)
                    count++;
            if (count > 1) {
                print_violation(s, count, j);
                total_violations++;
                set_dup = TRUE;
            }
        }
        if (set_dup) {
            set_violation++;
            auth = FALSE;
        }
    }
    /* Print violations if inauthentic */
    if (!auth) {
        printf("\n%d different sets have violations\n", set_violation);
        printf("%d violations in total\n", total_violations);
    }

    return auth;
}

void print_assign(int c, int n) {
    printf("row %d col %d must be %d\n", rownum(c), colnum(c), n);
}

/* Print Sudoku structure violations  */
void print_violation(int s, int count, int n) {
    printf("set %2d (%s %d): %1d instances of %d\n", s, get_group_type(s),
           to_set(s), count, n);
}

/* Get group name */
char *get_group_type(int s) {
    int grp = get_group_num(s);
    if (grp == 0)
        return "row";
    else if (grp == 1)
        return "col";
    else if (grp == 2)
        return "sqr";
    else
        return "err";
}

/* Get human-readable set number */
int to_set(int s) {
    return s % NDIG + 1;
}

/* Get group number 0 - 2 */
int get_group_num(int s) {
    return (s / 9) % 3;
}

/****************************************************************/