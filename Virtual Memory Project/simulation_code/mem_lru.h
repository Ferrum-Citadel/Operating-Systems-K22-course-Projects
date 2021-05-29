#pragma once
#include "../data_structs/LRU.h"
#include "../inverted_page_table/IPT.h"

//Synartisi poy ylopoiei tin prosomoiwsi toy memory management gia sygekrimeni eikoniki diergasia.
//LRU algorithmos antikatastasis
void memory_manager_lru(int frames_num, int q, int max_refs, int pid, FILE *fp, IPT_ENTRY *ipt, int *age_array, STATS *statistics);