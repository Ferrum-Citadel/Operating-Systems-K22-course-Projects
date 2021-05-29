#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "IPT.h"


//---------------Orismos synartisew gia Inverted Page Table

IPT_ENTRY *create_ipt(int length){

    //Desmeysi mnimis gia ipt
    IPT_ENTRY *new_ipt = (IPT_ENTRY *)malloc(length * sizeof(IPT_ENTRY));
    if(new_ipt == NULL){
        perror("malloc failed ");
        return NULL;
    } 

    //Arxikopoiisi tou pinaka
    for(int i=0; i<length; i++){
        new_ipt[i].valid = 0; // To frame i den xrisimopoieitai
    }

    return new_ipt;
}

void insert_ipt_entry(IPT_ENTRY *ipt, int index, int pid, int vpn, int dirty){
    //Egxwrisi twn timwn toy entry
    ipt[index].pid = pid;
    ipt[index].vpn = vpn;
    ipt[index].valid = 1;
    ipt[index].dirty = dirty;
} 

int search_ipt(IPT_ENTRY *ipt, int length, int pid, int vpn){
   
    //Anazitisi sto IPT gia arithmo page poy xrisimopoieitai apo sugekrimeno process
    int i;
    for(i=0; i<length; i++){
        if(ipt[i].valid && ipt[i].pid == pid && ipt[i].vpn == vpn){
            break;
        }
    }

    //Epistrefoyme to index an vrethike kai -1 an den vrethike
    if(i < length){
        return i;
    }else {
        return -1;
    }
}


void destroy_ipt(IPT_ENTRY *ipt, int length){
    free(ipt);
}

int is_full(IPT_ENTRY *ipt, int length, int *first_free_frame){
    int i, full=1;
    *first_free_frame = -1;
    
    for(i=0; i<length; i++){
        //An vroume entry me mideniko valid bit o pinakas den einai gematos 
        if(ipt[i].valid == 0){
            full = 0;
            *first_free_frame = i;
            break;
        }
    }

    return full;
}




