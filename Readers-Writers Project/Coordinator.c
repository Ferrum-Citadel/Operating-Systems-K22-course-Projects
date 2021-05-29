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


int main(int argc, char *argv[]) {
    
    int rcounter = 0, wcounter = 0, status = 0, shmid = 0;
    double rw_ratio,writers_perc;
    long double  reader_sum_time = 0, writer_sum_time = 0;
    pid_t child_pid=0;
    sem_t mutex, wsem;
    // Entry *temp_entry;
    
    //metatropi ton orismatwn apo string se long int (epeidi i atoi einai depreciated)
    long int iteration_num = strtol(argv[1], NULL, 10);
    long int entries_num = strtol(argv[2], NULL, 10);
    
    rw_ratio = atof(argv[3]);
    writers_perc = 1 / (rw_ratio + 1);

    //Desmeysi shared memory segment 
    shmid = shmget(IPC_PRIVATE, entries_num * sizeof(Entry), IPC_CREAT|0666);
    if(shmid == -1) perror("Error: shmget failed ");
    
    //Dimioyrgia kai arxikopoiisi POSIX semaphores gia ylopoisi me proteraiotita stoys readers
    if(sem_init(&mutex, 1, 1) != 0) perror("Error: sem_init failed ");
    if(sem_init(&wsem, 1, 1) != 0) perror("Error: sem_init failed ");

    //Dimioyrgia paidiwn (peers) apo ton idio gonea (coordinator). To PEER_NUM orizetai sto Coordinator.h
    for(int i=0; i < PEER_NUM; i++){
        child_pid = fork();
        if(child_pid < 0) {
            perror("Error: fork failed ");
        }else if(child_pid == 0){//An to pid einai 0 simainei oti to sugekrimeno process einai paidi opote trexoyme ton kwdika twn peers
            
            //Kanoume seed tin rand me to pid kathe process gia na min paroyme tous idious arithmous
            srand(getpid());
            for(int n=0; n < iteration_num; n++){

                //Tyxaios double arithmos apo 0 mexri 1
                double small_rand = ((double)rand() / RAND_MAX); 

                //Ypologizoyme tyxaio index enos entry pros prospelasi
                int rand_entry = rand() % entries_num;

                //Ypologizoyme tyxaio ekthetika katanemimeno xrono (TO  LAMDA orizetai sto Coordinator.h)
                double exponential_rand = -log(1 - small_rand) / LAMBDA;

                //Metatrepoyme ton xrono se nanodeyterolepta me xronous desmeysis <= 0.01 deyterolepta
                long int critical_time = exponential_rand * 10000000;

                //Gia arithmo mikrotero apo to writers_perc ekteleite o kwdikas twn writer
                if(small_rand <= writers_perc){ 
                    
                    WriterFunc(&wcounter, &writer_sum_time, critical_time, rand_entry, &wsem, shmid);
                
                }else {//Alliws ekteleite o kwdikas twn reader

                    ReaderFunc(&rcounter, &reader_sum_time, critical_time, rand_entry, &wsem, &mutex, shmid);
                }
            }
            printf("-------------------------PROCESS:%d-------------------------\n", getpid());
            printf("Writer iterations: %d\n", wcounter);
            printf("Reader iterations: %d\n", rcounter);
            
            //Ypologismos kai ektypwsi meswn xronwn anamonis
            AverageTime(wcounter, rcounter, writer_sum_time, reader_sum_time);
            
            exit(EXIT_SUCCESS);
        }
    }
    while (wait(&status) > 0);  //Trexei mexri na min yparxoyn alla paidia na perimenei opou epistrefetai -1
    //Diatrexoume tin mnimi kai ektypwnoyme prospelaseis
    MemorySweep(entries_num, shmid, rw_ratio);

    //Katastrofi shared memory segment
    if(shmctl(shmid, IPC_RMID, 0) == -1) perror("Error: shmctl couldn't remove shared memory segment ");
    //Katastrofi semaphores
    if(sem_destroy(&mutex) == -1) perror("Error: sem_destroy failed ");
    if(sem_destroy(&wsem) == -1) perror("Error: sem_destroy failed ");
    return(EXIT_SUCCESS);
}


