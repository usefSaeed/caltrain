#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"



void station_init(struct station *station)
{
    station->waitingRobots = 0;
    station->boardingRobots = 0;
    station->vacancies = 0;
    pthread_mutex_init(&station->stationMutex,NULL);
    pthread_cond_init(&station->robotsComeAboard,NULL);
    pthread_cond_init(&station->robotFoundSeat, NULL);
//    printf("Station created\n");
}

void station_load_train(struct station *station, int count)
{
    // ROBOTS THREADS CAN COME ABOARD BY BROADCAST
    // ROBOTS NUMBER THAT ENTER BUS = COUNT MAXIMUM
    if (!count) return;
    station->vacancies = count;
    pthread_cond_broadcast(&station->robotsComeAboard);
    pthread_mutex_lock(&station->stationMutex);
    while((station->vacancies && station->waitingRobots) || station->boardingRobots){
        pthread_cond_wait(&station->robotFoundSeat, &station->stationMutex);
    }
    pthread_mutex_unlock(&station->stationMutex);
    station->vacancies = 0;
}

void station_wait_for_train(struct station *station)
{
    // ROBOT AT STATION SHOULD WAIT UNTIL TRAIN ARRIVES AND THERE IS SPACE FOR HIM
    pthread_mutex_lock(&station->stationMutex);
    station->waitingRobots++;
    while(!station->vacancies) {
        pthread_cond_wait(&station->robotsComeAboard, &station->stationMutex);
    }
    station->waitingRobots--;
    station->vacancies--;
    station->boardingRobots++;
    pthread_mutex_unlock(&station->stationMutex);
}

void station_on_board(struct station *station)
{
    // EACH ROBOT TELLS TRAIN THAT HE IS ON BOARD WHEN HE FINDS A SEAT
    // TRAIN COULD KEEP TRACK OF HIS CAPACITY
    pthread_mutex_lock(&station->stationMutex);
    station->boardingRobots--;
    pthread_mutex_unlock(&station->stationMutex);
    pthread_cond_signal(&station->robotFoundSeat);
}

