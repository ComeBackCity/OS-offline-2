#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

using namespace std;

#define no_of_cyclists 15
#define no_of_services 5
#define payment_room_capacity 3
#define inf 9999

pthread_mutex_t mtx[no_of_services];
pthread_mutex_t mtx2, mtx3, servicing, bufferlock;
sem_t payment_room_filled;
int currently_servicing = 0, departed = 0;
queue<int> buffer;

int random_time(int min, int max)
{
    srand(time(NULL));
    int s = (rand() % (max - min + 1)) + 1;
    return s;
}

void *cyclist(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < no_of_services; i++)
    {
        pthread_mutex_lock(&mtx[i]);
        if (i == 0)
        {
            while (buffer.size() > 0)
            {
                pthread_mutex_lock(&mtx2);
            }
            pthread_mutex_lock(&servicing);
            currently_servicing++;
            pthread_mutex_unlock(&servicing);
        }
        printf("%d started taking service from serviceman %d\n", id, i + 1);
        int s = random_time(1, 1000000);
        usleep(s);
        printf("%d finished taking service from serviceman %d\n", id, i + 1);
        pthread_mutex_unlock(&mtx[i]);
    }
    pthread_mutex_lock(&servicing);
    currently_servicing--;
    pthread_mutex_unlock(&servicing);
    /*if (currently_servicing == 0)
    {
        pthread_mutex_unlock(&mtx3);
    }*/
    sem_wait(&payment_room_filled);
    printf("%d started paying the service bill\n", id);
    int x = random_time(1, 1000000);
    usleep(x);
    sem_post(&payment_room_filled);
    printf("%d finished paying the service bill\n", id);
    pthread_mutex_lock(&bufferlock);
    buffer.push(id);
    pthread_mutex_unlock(&bufferlock);
}

void *leaving(void *arg)
{
    while (departed != no_of_cyclists)
    {
        if (!buffer.empty())
        {
            if (currently_servicing == 0)
            {
                while (!buffer.empty())
                {
                    int id = buffer.front();
                    pthread_mutex_lock(&bufferlock);
                    buffer.pop();
                    pthread_mutex_unlock(&bufferlock);
                    int s = random_time(1, 1000000);
                    usleep(s);
                    printf("%d has departed\n", id);
                    departed++;
                }
                pthread_mutex_unlock(&mtx2);
            }
            /*else
            {
                pthread_mutex_lock(&mtx3);
            }*/
        }
    }
}

int main(int argc, char *argv[])
{
    int res;
    pthread_t cyclists[no_of_cyclists], leavers;

    for (int i = 0; i < no_of_services; i++)
    {
        res = pthread_mutex_init(&mtx[i], NULL);
        if (res != 0)
        {
            printf("Failed\n");
        }
    }

    res = pthread_mutex_init(&mtx2, NULL);
    if (res != 0)
    {
        printf("Failed\n");
    }

    res = pthread_mutex_init(&mtx3, NULL);
    if (res != 0)
    {
        printf("Failed\n");
    }

    res = pthread_mutex_init(&servicing, NULL);
    if (res != 0)
    {
        printf("Failed\n");
    }

    res = pthread_mutex_init(&bufferlock, NULL);
    if (res != 0)
    {
        printf("Failed\n");
    }

    res = sem_init(&payment_room_filled, 0, payment_room_capacity);
    if (res != 0)
    {
        printf("Failed\n");
    }

    res = pthread_create(&leavers, NULL, leaving, NULL);
    if (res != 0)
    {
        cout << "Could not create a thread" << endl;
    }

    for (int i = 0; i < no_of_cyclists; i++)
    {
        int *id = new int(i + 1);

        res = pthread_create(&cyclists[i], NULL, cyclist, (void *)id);
        if (res != 0)
        {
            cout << "Could not create a thread" << endl;
        }
    }

    for (int i = 0; i < no_of_cyclists; i++)
    {
        void *result;
        pthread_join(cyclists[i], &result);
    }

    void *Res;
    pthread_join(leavers, &Res);

    for (int i = 0; i < no_of_services; i++)
    {
        res = pthread_mutex_destroy(&mtx[i]);
        if (res != 0)
        {
            printf("Failed1\n");
        }
    }

    res = pthread_mutex_destroy(&mtx2);
    if (res != 0)
    {
        printf("Failed2\n");
    }

    res = pthread_mutex_destroy(&mtx3);
    if (res != 0)
    {
        printf("Failed3\n");
    }

    res = pthread_mutex_destroy(&servicing);
    if (res != 0)
    {
        printf("Failed4\n");
    }

    res = pthread_mutex_destroy(&bufferlock);
    if (res != 0)
    {
        printf("Failed5\n");
    }
}