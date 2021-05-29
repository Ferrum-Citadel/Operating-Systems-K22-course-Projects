#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "LRU.h"

//---------------------------Orismos synartisewn gia lru algorithmo

int *create_age_arr(int length){
    //Desmeysi mnimis gia ipt
    int *new_age_arr = (int *)malloc(length * sizeof(int));
    if(new_age_arr == NULL){
        perror("malloc failed ");
        return NULL;
    } 

    //Arxikopoiisi tou pinaka
    for(int i=0; i<length; i++){
        new_age_arr[i] = 0; // To frame i den xrisimopoieitai
    }

    return new_age_arr;
}

void destroy_age_arr(int *age_array){
    free(age_array);
}

int get_victim(int *age_array, int length){
    int lru_index = 0, min = age_array[0];

    for(int i=1; i<length; i++){
        if(age_array[i] < min){
            min = age_array[i];
            lru_index = i;
        }
    }

    return lru_index;
}

void set_timestamp(int *age_array, int index, int length){
    age_array[index] = clock();
}