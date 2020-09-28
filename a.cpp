#include<bits/stdc++.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>
#include<semaphore.h>

using namespace std;

#define no_of_cyclists 7
#define no_of_services 3
#define payment_room_capacity 3
#define inf 9999

pthread_mutex_t mtx[no_of_services];
pthread_mutex_t mtx2;
sem_t payment_room_filled;
queue<int> buffer;

int random_time(int min, int max){
    srand(time(NULL));
    int s = (rand() % (max - min + 1)) + 1;
    return s;
}

void* cyclist(void* arg){
    int id = *(int*)arg;
    for(int i=0; i<no_of_services; i++){
        pthread_mutex_lock(&mtx[i]);
        printf("%d started taking service from serviceman %d\n", id , i+1);
        int s = random_time(1,4);
        sleep(s);
        printf("%d finished taking service from serviceman %d\n", id , i+1);
        pthread_mutex_unlock(&mtx[i]); 
    }
    sem_wait(&payment_room_filled);
    printf("%d started paying the service bill\n", id);
    int x = random_time(1,9);
    sleep(x);
    printf("%d finished paying the service bill\n", id);
    sem_post(&payment_room_filled);
}

int main(int argc, char* argv[]){
    int res;
    pthread_t cyclists[no_of_cyclists];

    for(int i=0; i<no_of_services; i++){
        res = pthread_mutex_init(&mtx[i],NULL);
        if(res != 0){
            printf("Failed\n");
        }
    }

    res = pthread_mutex_init(&mtx2,NULL);
    if(res != 0){
        printf("Failed\n");
    }

    res = sem_init(&payment_room_filled,0,payment_room_capacity);
    if(res != 0){
        printf("Failed\n");
    }

    for(int i=0; i<no_of_cyclists; i++){
        int* id = new int(i+1);

        res = pthread_create(&cyclists[i], NULL, cyclist, (void *)id);

        if(res!=0){
            cout << "Could not create a thread" << endl;
        }
    }

    for(int i = 0; i < no_of_cyclists; i++){
        void *result;
        pthread_join(cyclists[i],&result);
        //printf("%s",(char*)result);
    }

    //cout << buffer.size() << endl;

    for(int i=0; i<no_of_services; i++){
        res = pthread_mutex_destroy(&mtx[i]);
        if(res != 0){
            printf("Failed\n");
        }
    }

    res = pthread_mutex_init(&mtx2,NULL);
        if(res != 0){
            printf("Failed\n");
        }
}