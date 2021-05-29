#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "./data_structs/LRU.h"
#include "./inverted_page_table/IPT.h"
#include "Stats.h"
#include "./simulation_code/mem_lru.h"


int main(int argc, char *argv[]){
    int algo, curr_refs = 0;
    int frames_num, q, max_refs, ws_window;
    FILE *fp1, *fp2;
    STATS proc1, proc2;

    //metatropi ton orismatwn apo string se long int (epeidi i atoi einai depreciated)
    frames_num = (int)strtol(argv[1], NULL, 10);
    q = (int)strtol(argv[2], NULL, 10);
    max_refs = (int)strtol(argv[3], NULL, 10);

    //Elegxoyme poios algorithmos dowthike pros xrisi kai diaxeirizomaste ta orismata katallila
    if((strcmp(argv[4], "LRU") == 0) && argc == 6){
        ws_window = strtol(argv[5], NULL, 10);
        algo = 0;
    // }else if((strcmp(argv[4], "WS") == 0) && argc == 5){  //Trexoume mono ton LRU algorithmo
    //     algo = 1;
     }else{
        printf("Error: The given argumets are wrong, please refer to the README file.\n The programm will now stop...\n");
        exit(EXIT_FAILURE);
    }
    //Anoigma twn arxeiwn me ta reference trace
    fp1 = fopen("trace_first.txt", "r");
    if(fp1 == NULL) perror("fopen failed!");

    fp2 = fopen("trace_sec.txt", "r");
    if(fp2 == NULL) perror("fopen2 failed!");

    //Dimiourgia tou IPT me megethos ton dedomeno arithmo twn frames
    IPT_ENTRY *ipt = create_ipt(frames_num);

    //Dimiourgia toy array me ta timestamps 
    int *age_array = create_age_arr(frames_num);
    
    proc1.page_fault_num = proc1.read_num = proc1.ref_num = proc1.used_frame_num = proc1.write_num = 0;
    proc2.page_fault_num = proc2.read_num = proc2.ref_num = proc2.used_frame_num = proc2.write_num = 0;

    curr_refs = 2*q;
    int i=0;
    while(curr_refs <= max_refs){
        if(algo == 0){
            memory_manager_lru(frames_num, q, max_refs, 1, fp1, ipt, age_array, &proc1);      
            memory_manager_lru(frames_num, q, max_refs, 2, fp2, ipt, age_array, &proc2);            
        }
        curr_refs = curr_refs + 2*q;
        i++;
    }

    //Katastrofi tou inverted page table
    destroy_ipt(ipt, frames_num);

    //Katastrofi toy array me ta timestamps 
    destroy_age_arr(age_array);
    
    fclose(fp1);
    fclose(fp2);
    
    printf("----------STATISTICS FOR PROCESS: 1\n");
    printf("-Page faults: %d\n", proc1.page_fault_num);
    printf("-Reads from disk: %d\n", proc1.read_num);
    printf("-Writes to disk: %d\n", proc1.write_num);
    printf("-References examined: %d\n", proc1.ref_num);
    printf("-Frames used at exit: %d\n\n", proc1.used_frame_num);
    

    printf("----------STATISTICS FOR PROCESS: 2:\n");
    printf("-Page faults: %d\n", proc2.page_fault_num);
    printf("-Reads from disk: %d\n", proc2.read_num);
    printf("-Writes to disk: %d\n", proc2.write_num);
    printf("-References examined: %d\n", proc2.ref_num);
    printf("-Frames used at exit: %d\n\n", proc2.used_frame_num);

    printf("----------STATISTICS IN GENERAL\n");
    printf("-Page faults: %d\n", proc1.page_fault_num + proc2.page_fault_num);
    printf("-Reads from disk: %d\n", proc1.read_num + proc2.read_num);
    printf("-Writes to disk: %d\n", proc1.write_num + proc2.write_num);
    printf("-References examined: %d\n", proc1.ref_num + proc2.ref_num);
    printf("-Frames used at exit: %d\n\n", proc1.used_frame_num + proc2.used_frame_num);

}