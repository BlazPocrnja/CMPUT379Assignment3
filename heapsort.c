// This implementation of heapsort derived from
// https://rosettacode.org/wiki/Sorting_algorithms/Heapsort#C (c) RosettaCode.org
// And modified to suit needs of assigment.s

#include "simulator.h"

/** finds index of max value within heap **/
int max (int n, int i, int j, int k)
{
    int m = (i);

    if (j < n && get(j) > get(m)) // n is an index
    {
        m = j;
    }

    if (k < n && get(k) > get(m))
    {
        m = k;
    }

    return m;
}

/** heapifys the data **/
void downheap (int n, int i)
{
    while (1)
    {
        int j = max(n, (i), (2 * i + 1), (2 * i + 2)); // j is an index value //n is len()

        if (j == i)
        {
            break;
        }

        int t = get(i);
        put(i, get(j));
        put(j, t);

        i = j;
    }
}

void process()
{

    int i;
    int n;

    printf("\nEnter number of elements to sort: ");
    scanf ("%d", &n); // Gets amount of numbers to generate
    printf ("\nHeap Sorting %1d elements\n", n);

    for (i = 0; i < n; i++) put(i, lrand48());    // Generating random numbers

    for (i = (n - 2) / 2; i >= 0; i--)
    {
        downheap(n, i); /** remove list as argument for dheap **/
    }

    for (i = 0; i < n; i++)
    {
        int t = get(n - i - 1);
        put(n - i - 1, get(0));
        put(0, t);
        downheap(n - i - 1, 0);
    }


    // Prints data for the lulz
    printf("\nSorted: \n");
    for (i = 0; i < n; i++)
    {
        printf("%i \n", get(i));
    }


    done(); 
}



