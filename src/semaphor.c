#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define EATING 'E'
#define THINKING 'T'
#define HUNGRY 'H'

sem_t *p_sem;
sem_t crit_region;
sem_t print_region;
char *philosophers;
int n;
int all_together;

void print(){
    sem_wait(&print_region);

        int i=0;
        printf("%c", philosophers[i]);
        for(i=1;i<n;i++) printf(" %c", philosophers[i]);
        printf("\n");

    sem_post(&print_region);
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
        sem_post(&(p_sem[philosopher]));
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

                sem_wait(&crit_region);
                philosophers[index] = THINKING;
                print();
                test(left(index));
                test(right(index));
                sem_post(&crit_region);
                break;
            case HUNGRY:
                sem_wait(&crit_region);
                test(index);
                sem_post(&crit_region);
                sem_wait(&(p_sem[index]));
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

    p_sem = calloc(n, sizeof(sem_t));
    for(i=0;i<n;i++) sem_init(&(p_sem[i]), 0, 1);
    sem_init(&crit_region, 0, 1);
    sem_init(&print_region, 0, 1);
    philosophers = calloc(n, sizeof(philosophers));
    for(i=0;i<n;i++) philosophers[i] = HUNGRY;


    all_together=1;
    for(i=0;i<n;i++) {int *_i = calloc(1, sizeof(int)); *_i = i; pthread_create(&(th[n]), NULL, philosopher, _i);}
    all_together=0;
    print();
    while(1);
}

