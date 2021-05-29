#pragma once
//Orizma tou plithos twn processes pou tha dimiourgithoun
#define PEER_NUM 5
//Orizoyme tin parametro rythmoy tis tyxaias ekthetikis katanomis
#define LAMBDA 0.1
//Struct gia tin kathe eggrafi
typedef struct Entries{
    int readers;
    int writers;
} Entry;

int WriterFunc(int *, long double *, long int, int, sem_t *, int);
int ReaderFunc(int *, long double *, long int, int, sem_t *, sem_t *, int);
void AverageTime(int, int, long double, long double);
int MemorySweep(long int, int, double);

