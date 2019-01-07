
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "utils.h"
#include <pthread.h>
       
pthread_mutex_t mutex;

double F_x( 	double x){
	double out = 4/(1+(x*x)); 
	return(out);
}
pthread_t* initThreads(int count_t)
{
	pthread_t* threads =  (pthread_t* )malloc(count_t * sizeof(pthread_t));
	return(threads);
}
//struct für die Parameterübergabe an Threads
struct args {
		double pi_approx;
		double delta_x;
		int idx;
		
	};
/*die paralelisierungsmethode für den Thradaufruf
 * es wird für jeden Abstand das Integral berechnet 
 * und auf das gesamt ergebniss addiert
 */
 
 void *compute_x(void *input){
	double delta = ((struct args*)input)->delta_x;
	int i = ((struct args*)input)->idx;
	double x=(i+0.5)*delta;
	 // pthread_mutex_lock (&mutex);
	((struct args*)input)->pi_approx += F_x(x)*delta;
	 // pthread_mutex_unlock (&mutex);

	
	return(0);
 }

double int_pi_t(double N, int count_t){
	//struct aus Aurgumenten für den thread-aufruf
	struct args *Comp_args = (struct args *)malloc(sizeof(struct args));
	Comp_args->pi_approx = 0;
	Comp_args->delta_x = 1/N;
	pthread_t* threads = initThreads(count_t);
	for(int i = 0; i < N; ++i){
		Comp_args->idx = i;
		pthread_create(&threads[i%count_t], NULL, &compute_x, (void *) Comp_args);
		pthread_join(threads[i%count_t], NULL);
	}
	return(((struct args*)Comp_args)->pi_approx);
}

int main (int argc, char* argv[]) {
	int count_t = strtol(argv[1], NULL, 10);
	int N = strtol(argv[2], NULL, 10);
	time_t now;
	time_t not_now;
	time_t cylces = 0;
	rdtsc(now);
	// pthread_mutex_init (&mutex, NULL);
	double erg = int_pi_t(N, count_t);
	rdtsc(not_now);
	cylces = not_now - now;
	printf ("Cylcles  = %ld\n", cylces);
	printf("pi = %.13lf\n", erg  );
	printf("threads = %d\n", count_t  );
	printf("iter = %d\n\n", N  );
	
}

