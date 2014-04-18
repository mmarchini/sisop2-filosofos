#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define EATING 'E'
#define THINKING 'T'
#define HUNGRY 'H'

pthread_mutex_t print_region = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t crit_region = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t *mutex_p;
pthread_cond_t *cond_p;

char *philosophers;
int n;
int all_together;

void print(){
    pthread_mutex_lock(&print_region);

        int i=0;
        printf("%c", philosophers[i]);
        for(i=1;i<n;i++) printf(" %c", philosophers[i]);
        printf("\n");

    pthread_mutex_unlock(&print_region);
}

int left(int philosopher){
    if(philosopher==0){
        return n-1;
    }
    else return philosopher-1;
}

int right(int philosopher){
    if(philosopher==n-1){
        return 0;
    }
    else return philosopher+1;
}

void test(int philosopher){
    if (philosophers[philosopher] == HUNGRY && philosophers[left(philosopher)] != EATING && philosophers[right(philosopher)] != EATING) {
        philosophers[philosopher] = EATING;
        print();
        pthread_cond_broadcast(&(cond_p[philosopher]));
    }
}

void *philosopher(void *args){
    while(all_together);
    int index;
    index = *(int *)args;

    while(1){
        switch(philosophers[index]){
            case THINKING:
                sleep((rand()%10)+1);
                philosophers[index] = HUNGRY;
                print();
                break;
            case EATING:
                sleep((rand()%10)+1);

                pthread_mutex_lock(&crit_region);
                    philosophers[index] = THINKING;
                    print();
                    test(left(index));
                    test(right(index));
                pthread_mutex_unlock(&crit_region);
                break;
            case HUNGRY:
                pthread_mutex_lock(&crit_region);
                    test(index);
                pthread_mutex_unlock(&crit_region);

                if(philosophers[index]!=EATING){
                    pthread_mutex_lock(&(mutex_p[index]));
                    pthread_cond_wait(&(cond_p[index]), &(mutex_p[index]));
                }

                break;
        }
    }

}

int main(int argn, char *argv[]){
    if(argn==1){
        printf("Missing paramenters\n");
        exit(EXIT_FAILURE);
    }
    int i;
    sscanf(argv[1], "%d", &n);
    pthread_t th[n];

    srand(time(NULL));

    cond_p = calloc(n, sizeof(pthread_cond_t));
    for(i=0;i<n;i++) pthread_cond_init(&(cond_p[i]), NULL);
    mutex_p = calloc(n, sizeof(pthread_mutex_t));
    for(i=0;i<n;i++) pthread_mutex_init(&(mutex_p[i]), NULL);

    philosophers = calloc(n, sizeof(philosophers));
    for(i=0;i<n;i++) philosophers[i] = HUNGRY;


    all_together=1;
    for(i=0;i<n;i++) {int *_i = calloc(1, sizeof(int)); *_i = i; pthread_create(&(th[n]), NULL, philosopher, _i);}
    all_together=0;
    print();
    while(1);
}

