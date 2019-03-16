#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>//use for sse2 

void get_walltime(double *wct);
void PVEC(__m128 a);


int main(int argc, char** argv)
{
   
    __m128 vA, vB;
    double ts,te;
    float *arrayA,*arrayB,*arrayC;
    int i,j,k;
    
    i=0;
    i = posix_memalign((void**)&arrayA,16,N*N*sizeof(float));
    if (i!=0)
    {
        exit(1);
    }
    
    i = posix_memalign((void**)&arrayB,16,N*N*sizeof(float));
    if (i!=0)
    {
        free(arrayA);
        exit(2);
    }
    
    i = posix_memalign((void**)&arrayC,16,N*N*sizeof(float));
    if (i!=0)
    {
        free(arrayA);
        free(arrayB);
        exit(3);
    }
    
    
    for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			arrayA[i*N+j] = 4+i;
			arrayB[i*N+j] = i+2+j;
			arrayC[i*N+j] = i+j;
		}
		//printf("\n");
	}
	  
    get_walltime(&ts);
    for( i = 0; i < N; ++i) 
    {
        for(j = 0; j < N; j+=4) 
        {   // vectorize over this loop
            __m128 vR = _mm_setzero_ps();
            for( k = 0; k < N; k++) 
            {   // not this loop
                //arrayC[i][j] += arrayA[i][k] * arrayB[k][j];
                vA = _mm_set1_ps(arrayA[i*N+k]);  // load+broadcast 
                vB = _mm_load_ps(&arrayB[k*N+j]);  // array[k][j+0..3]
                vR = _mm_add_ps(vR, _mm_mul_ps(vA, vB));
                //PVEC(vA);
            }
            _mm_store_ps(&arrayC[i*N+j], vR);
        }
    }
    get_walltime(&te);
    
    
    /*
    for(i= 0; i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            printf("%f ",arrayC[i*N+j]);
        }
        printf("\n");
    }
    */
    
    
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


//macro to print a __m128 vector
void PVEC(__m128 a)						
 {							
    float tp[4];					
    _mm_storeu_ps ( tp ,a);				
    printf("VEC %7.1f %7.1f %7.1f %7.1f \n",		
	   tp[0],					
	   tp[1],					
	   tp[2],					
	   tp[3]);					
 }
 
 
