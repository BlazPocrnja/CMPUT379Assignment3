// Quicksort implementation taken from
// https://www.programmingalgorithms.com/algorithm/quick-sort-iterative?lang=C
// (c) ProgrammingAlgorithms.com
// and modified to suit assigment.
#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

#define  MAX_LEVELS  300 // Quicksort will sort up to 2^300 iterations without error

void process()
{

    int  piv, beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R, swap, length;

    printf("\nEnter number of elements to sort: ");
    scanf ("%d", &length); // Gets amount of numbers to generate
    printf ("Quick Sorting %1d elements\n", length);

    //seed48(seed);
    for (i = 0; i < length; i++) put(i, lrand48());    // Generating random numbers

    beg[0]=0;
    end[0]=length;

    i = 0;

    while (i>=0)
    {
        L=beg[i];
        R=end[i]-1;

        if (L<R)
        {
            piv = get(L); // piv=arr[L];

            while (L<R)
            {
                while (get(R)>=piv && L<R) R--;

                if (L<R) {put(L++, get(R));} //arr[L++]=arr[R];

                while (get(L)<=piv && L<R) L++;

                if (L<R) {put(R--, get(L));} //arr[R--]=arr[L];
            }

            put(L, piv); //arr[L]=piv;
            beg[i+1]=L+1;

            end[i+1]=end[i];
            end[i++]=L;

            if (end[i]-beg[i]>end[i-1]-beg[i-1])
            {
                swap=beg[i];

                beg[i]=beg[i-1];
                beg[i-1]=swap;

                swap=end[i];

                end[i]=end[i-1];
                end[i-1]=swap;
            }
        }
        else
        {
            i--;
        }
    }

    printf("\nSorted: \n");
    // Printing for awesomeness
    for (i = 0; i < length; i++)
        printf("%i \n", get(i));

    done();
}







