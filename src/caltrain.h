#ifndef LAB3_CALTRAIN_H
#define LAB3_CALTRAIN_H
#endif

#include <pthread.h>

struct station {
    int waitingRobots;
    int boardingRobots;
    int vacancies;
    pthread_mutex_t stationMutex;
    pthread_cond_t robotsComeAboard;
    pthread_cond_t robotFoundSeat;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
