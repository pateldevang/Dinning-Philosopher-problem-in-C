#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
// Defining the states that phliosopher can have
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define N 5
#define LEFT (phnum+4)%N
#define RIGHT (phnum+1)%N
int state[N];
int phil[N]={0,1,2,3,4};

// Mutex declared
sem_t mutex;
sem_t S[N];

//MARK: - testing Function
void test(int phnum)
{
    // If the adjacent philosphers are not eating and the current philsopher is in HUNGRY state then
    if(state[N]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING)
    {
        // We change the state of the current Philosopher to EATING
        state[phnum]=EATING;
        sleep(2);
        printf("Philsospher %d takes chopstick %d and %d",phnum+1,LEFT+1,RIGHT+1);
        printf("Philosopher %d is Eating\n", phnum + 1);
        // Sem post required otherwise the process will be deadlock as it does not update the state of Philosopher
        sem_post(&S[phnum]);
    }
}

//MARK: - taking fork functin
void take_fork(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = HUNGRY;
    printf("Philosopher %d is Hungry\n", phnum + 1);
    test(phnum);
    sem_post(&mutex);
    sem_wait(&S[phnum]);
    sleep(1);
}

//MARK: - put fork function
void put_fork(int phnum)
{
    sem_wait(&mutex);
    // After eating the Philosopher goes to THINKING state and keeps the fork on the table
    state[phnum] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n",phnum + 1, LEFT + 1,
           phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);
    test(LEFT);
    test(RIGHT);
    // Sem post required otherwise the process will be deadlock as it does not update the state of Philosopher
    sem_post(&mutex);
}

//MARK: - initalize Philosopher function
void* philospher(void* num)
{
    while (1) {
        int* i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}

//MARK: - main function 
int main()
{
    int i;
    // Initialize the pthread_t objects with "n" arrays from the pthread.h library
    pthread_t thread_id[N];
    // initialize the mutex for the the above declared
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++)
        sem_init(&S[i], 0, 2);
    for (i = 0; i < N; i++) {
         //Create pthread for philospher
        pthread_create(&thread_id[i],NULL,philospher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }
    for (i = 0; i < N; i++){
        pthread_join(thread_id[i], NULL);
    }
    // Here sem_destoy function can be added if we want to destroy the the above declared Mutex(uncomment the below).
    //sem_destroy(&mutex);
    //sem_destroy(&S[N]);
}
