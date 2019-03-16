#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void get_walltime(double *wct);


int main(int argc, char** argv)
{
    int i,j,k;
    double ts,te;
    float *arrayA,*arrayB,*arrayC;
    float *pa,*pb,*pc;
    float sum;
    arrayA = (float*) malloc(N * N * sizeof(float));
    arrayB = (float*) malloc(N * N * sizeof(float));
    arrayC = (float*) malloc(N * N * sizeof(float));


    if(arrayA == NULL)                     
    {
        printf("Error! memory not allocated.A");
        exit(0);
    }
    
        if(arrayB == NULL)                     
    {
        printf("Error! memory not allocated.B");
        free(arrayA);
        exit(0);
        
    }
    
    if(arrayC == NULL)                     
    {
        printf("Error! memory not allocated.C");
        free(arrayA);
        free(arrayB);
        exit(0);
    }
    
    //Initialize arrays
    for(i = 0; i< N; i++)
    {
        for(j = 0 ; j < N ; j++)
        {
			arrayA[i*N+j] = 4+i;
			arrayB[i*N+j] = i+2+j;
			arrayC[i*N+j] = i+j;
        }
    }
    
    
    get_walltime(&ts);
    pc = arrayC;
    for(k = 0; k<N;k++)
    {
        pb = arrayB;
        for(j = 0 ; j< N;j++)
        {
            pa = arrayA + k*N;
            sum = 0.0;
            for(i = 0 ; i<N;i++)
            {
                sum += (*pa) * (*pb);
                pa++;
                pb++;
            }
            *pc = sum;
            pc++;
        }
    }
    get_walltime(&te);
    
    // for(i= 0; i<N;i++)
    // {
    //     for(j=0;j<N;j++)
    //     {
    //         
    //         if(arrayC[j*N+i] != 6*N)
    //         {
    //             printf("Error");
    //             exit(3);
    //         }
    //     }
    //     
    // }
    
 	float mflops = (N* N) / ((te - ts) * 1e6);
	printf("mflops: %f\n", mflops);
 
 
    free(arrayA);
    free(arrayB);
    free(arrayC);
    return 0;
}


void get_walltime(double *wct)
{
    struct timeval tp;
    gettimeofday(&tp,NULL);
    *wct = (double) (tp.tv_sec+tp.tv_usec /1000000.0);
}
