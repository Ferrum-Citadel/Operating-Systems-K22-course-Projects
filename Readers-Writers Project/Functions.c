#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "Coordinator.h"

//Epistrefei EXIT_FAILURE se apotyxia kai EXIT_SUCCESS se epituxia
int WriterFunc(int  *writercount, long double *wait_time, long int use_time, int entry_index, sem_t *wsem, int mem_id){
    
    clock_t start = clock();
    //Kleidwnoyme tin prosvasi se allous writer 
    if(sem_wait(wsem) == -1){
        perror("Error: sem_wait failed");
        return(EXIT_FAILURE);
    }
    clock_t end = clock();

    (*writercount)++;
    //Mapping tou shared memory panw sto peer process
    Entry *entry_start = (Entry *) shmat(mem_id, NULL, 0);
    if((intptr_t)entry_start == -1){
        perror("Error: shmat failed");
        return(EXIT_FAILURE);
    }    
    //Ayksanoyme ton arithmo twn writer sto tyxaio entry poy desmeysame
    (entry_start + entry_index)->writers++;

    //Apokolisi toy shared memory segment
    if(shmdt((Entry *)entry_start) == -1){
        perror("Error: shmdt failed ");
        return(EXIT_FAILURE);
    }

    //Simulation prosartisis mias kataxwrisis gia tyxaio ekthetika katanomimeno xrono
    struct timespec ts = {0, use_time};
    nanosleep(&ts, NULL);

    //Apeleytherwnoyme to entry 
    if(sem_post(wsem) == -1){
        perror ("Error:sem_post failed ");
        return (EXIT_FAILURE);
    }
    
    *wait_time += (long double)(end - start) / CLOCKS_PER_SEC;
    return(EXIT_SUCCESS);
}

//Epistrefei EXIT_FAILURE se apotyxia kai EXIT_SUCCESS se epituxia
int ReaderFunc(int *readercount, long double *wait_time, long int use_time, int entry_index, sem_t *wsem, sem_t *mutex, int mem_id){

    static int current_readers=0;
    
    clock_t readcount_start = clock();
    //Kleidwnoyme tin prosvasi se alloys reader
    if(sem_wait(mutex) == -1) {
        perror("Error: sem_wait failed ");
        return (EXIT_FAILURE);
    }
    clock_t readcount_end = clock();

    (*readercount)++;
    current_readers++;
    if(current_readers == 1){//Ean exoyme ton prwto reader
        //Kleidwnoyme tin prosvasi se writer (paramenei etsi mexri na apwxwrisoyn  oloi oi readers)
        if(sem_wait(wsem) == -1) perror("Error: sem_wait failed ");
    }
    
    //Mapping tou shared memory panw sto peer process
    Entry *entry_start = (Entry *) shmat(mem_id, NULL, 0);
    if((intptr_t)entry_start == -1){
     perror("Error: shmat failed ");
     return(EXIT_FAILURE);
    }

    //Ayksanoyme ton arithmo twn reader sto tyxaio entry poy desmeysame
    (entry_start + entry_index)->readers++;

    //Apokolisi toy shared memory segment
    if(shmdt((Entry *)entry_start) == -1){
        perror("Error: shmdt failed ");
        return(EXIT_FAILURE);
    } 

    //Epitrepoyme tin prosvasi se alloys reader
    if(sem_post(mutex) == -1){
        perror("Error: sem_post failed ");
        return(EXIT_FAILURE);
    } 


    //Montelopoisi prosartisis mias kataxwrisis gia xrono critical_time
    struct timespec ts = {0, use_time};
    nanosleep(&ts, NULL);
    
    clock_t start = clock();
    //Kleidwnoyme tin prosvasi se alloys reader
    if(sem_wait(mutex) == -1) perror("Error: sem_wait failed ");
    clock_t end = clock();

    current_readers--;
    if(current_readers == 0){//An apoxwrisei o teleytaios reader
        //Apeleytherwnoyme to entry gia writers
        if(sem_post(wsem) == -1) {
            perror ("Error:sem_post failed ");
            return(EXIT_FAILURE);
        }
    }

    //Epitrepoyme tin prosvasi se alloys reader
    if(sem_post(mutex) == -1){
        perror("Error: sem_post failed ");
        return(EXIT_FAILURE);
    } 

    //O xronos poy pithanon perimene enas reader xwrizetai se 2 meri
    *wait_time = (((long double)(readcount_end - readcount_start) + (long double)(end - start))) / CLOCKS_PER_SEC;
    return(EXIT_SUCCESS);
}


void AverageTime(int writer_count, int reader_count, long double writer_time, long double reader_time){
    long double reader_average, writer_average, overall_average;
    //Pernoyme periptwseis gia na min diairesoyme me to 0
    if(reader_count == 0){
        reader_average = 0;
    }else {
        reader_average = reader_time / reader_count;
    }
        
    if(writer_count == 0){
        writer_average = 0;
    }else {
        writer_average = writer_time / writer_count;
    }
    overall_average = (writer_time + reader_time) / (writer_count + reader_count);   

    printf("Average waiting time for writers: %Lf microseconds\n", writer_average * 1000000);
    printf("Average waiting time for readers: %Lf microseconds\n", reader_average * 1000000);
    printf("Overall average waiting time: %Lf microseconds\n\n", overall_average * 1000000);
}

//Epistrefei EXIT_FAILURE se apotyxia kai EXIT_SUCCESS se epituxia
int MemorySweep(long int entries_num, int mem_id, double ratio){
    int entry_writes = 0, entry_reads = 0;
    //Mapping tou shared memory panw sto coordinator process
    Entry *temp_entry = (Entry *) shmat(mem_id, NULL, 0);
    if((intptr_t)temp_entry == -1){
        perror("Error: shmat failed ");
        return(EXIT_FAILURE);
    } 

    //Sarwnoyme to shared memory kai typwnoyme athroismata reads kai writes ana entry
    for(int i=0; i<entries_num; i++){
        entry_writes += (temp_entry + i)->writers;
        entry_reads += (temp_entry + i)->readers;
    }
    int sum = entry_reads + entry_writes;
    //Apokolisi toy shared memory segment
    if(shmdt((Entry *)temp_entry) == -1){
        perror("Error: shmdt failed ");
        return(EXIT_FAILURE);
    } 
    
    printf("-/-/-/-/-/--/-/-/-/--SHARED MEMORY STATS--/-/-/-/-/-/-/-/-/-/-/\n");
    printf("Total reads in entries: %d\nTotal writes in entries: %d\nGiven ratio: %lf\nSUM: %d\n", entry_reads, entry_writes, ratio, sum);
    return(EXIT_SUCCESS);
}
