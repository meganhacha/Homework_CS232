/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, const char** argv){

    int lowerBound, upperBound;

    if(argc != 3){
        fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
        return -1;
    }
  
    lowerBound = atoi(argv[1]);
    upperBound = atoi(argv[2]);
  
    if(lowerBound < 1 || upperBound < 1){
        fprintf(stderr, "ERROR: both the lowerBound (%d) and the upperBound (%d)"
	          " must be positive.\n", lowerBound, upperBound);
        return -2;
    }
  
    {
        int primeFlag = 0;

        if(upperBound < lowerBound) {
            lowerBound = 4;
            upperBound = lowerBound;
            /*setting them to the nearest even number so they don't have any modulo values. 
                         This is a product of sleep deprivation and I'm sure there's a more efficient way*/
        }
        
        if(lowerBound == 1 && upperBound == 1) {
            lowerBound = 4;
            upperBound = lowerBound; 
        }
        
        if (lowerBound == 1) {
            lowerBound++;
        }
        if(lowerBound == 2) {
            printf("%d\n", lowerBound);
            lowerBound++;
        }

        for(int i = lowerBound; i <= upperBound; i++) {
            primeFlag = 1;
            
            if(i == 2) {
                printf("%d\n", i);
            }

            for(int j = 2; j <= sqrt(i); j++) {
                
                if(i % j == 0) {
                    primeFlag = 0;
                    break;
                }
            }

            if( primeFlag == 1) {
                printf("%d \n", i);
            }
        }

    }

    return 0;
}
