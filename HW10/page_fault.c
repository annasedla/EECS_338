// Chris Fietkiewicz. Test program to generate page faults. Adapted from work by Michael Recachinas.
// Receives 1 command-line argument which is the desired quantity of minor page faults.
// The following is an example of using time utility in Linux to create 100 additional page faults:
//     /usr/bin/time -v ./page_fault 100
// The following is an example of doing the same using Mac OS, where the time utility is different:
//     /usr/bin/time -l ./page_fault 100
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char ** argv) {
    printf("Page size: %d\n", getpagesize());
    printf("Int size: %lu\n", sizeof(int));
    if (argc < 2){
        printf("Usage: ./page_fault <number of page faults>\n");
        return -1;
    }
    int i, j, N, page_size, int_size;
    N = atoi(argv[1]); /* Grab approximate number of desired page faults */
    int_size = sizeof(int); /* Get machine's size of representing signed integers */
    page_size = (int) getpagesize() / int_size; /* Entries per page for signed integers */
    int a[N][page_size]; /* 2D Array where the ROWS are the number of desired faults and the COLS have the size of a page */
    int x = 0;

    for (i = 0; i < page_size; i++) {
        for (j=0; j < N; j++) {
             x += a[j][i];
        }
    }
    return 1;
}
