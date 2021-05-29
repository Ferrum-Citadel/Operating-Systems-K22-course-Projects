#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include "../Stats.h"
#include "mem_lru.h"

void memory_manager_lru(int frames_num, int q, int max_refs, int pid, FILE *fp, IPT_ENTRY *ipt, int *age_array, STATS *statistics){

    int free_frame, hit, dirty, victim;
    int page_num;
    char *token, *operation, line[50], page_string[5];

    for(int i=0; i<q; i++){

        fgets(line, sizeof(line), fp);

        token = strtok(line, " \t");
        
        for(int i=0; i<5; i++){
            page_string[i] = token[i];
        }
        page_string[5] = '\0';

        token = strtok(NULL," \n");
        operation = token;
        //Metatrepoyme to page address se int gia eykolia se sygrisi
        page_num = (int)strtol(page_string, NULL, 16);
                     
        statistics->ref_num++;

        hit = search_ipt(ipt, frames_num, pid, page_num);
        
        //An to page kai to pid poy to kalei einai idi ston IPT prosthetoyme timestamp xrisis sto age_array
        if(hit != -1){

            set_timestamp(age_array, hit, frames_num);

            //Se periptwsi poy den einai ston IPT kai den einai gematos exoume page fault kai ta prosthetoyme
        }else if(hit == -1 && !is_full(ipt,frames_num, &free_frame)){
            statistics->page_fault_num++;
            statistics->read_num++;

            if(strcmp(operation, "R") == 0){
                dirty = 0;
            }else{
                dirty = 1;
            }
            insert_ipt_entry(ipt, free_frame, pid, page_num, dirty);
            set_timestamp(age_array, free_frame, frames_num);
            //Menei i periptwsi na min einai ston IPT kai parallila na einai gematos
        }else{
            statistics->read_num++;
            statistics->page_fault_num++;

            if(strcmp(operation, "R") == 0){
                dirty = 0;
            }else{
                dirty = 1;
            }
            //Vriskoyme to victime index sto age_array
            victim = get_victim(age_array, frames_num);

            //An exei ginei write to "swzoyme ston disko"
            if(ipt[victim].dirty == 1)  statistics->write_num++;

            //Vazoyme to neo page sto frame toy victim
            insert_ipt_entry(ipt, victim, pid, page_num, dirty);

            set_timestamp(age_array, victim, frames_num);
        }
            
        if(feof(fp)){ 
        break ;
        }
    }
        
    if(is_full(ipt, frames_num, &free_frame)){
        statistics->used_frame_num = frames_num;
    }else{
        statistics->used_frame_num = free_frame;
    }
}