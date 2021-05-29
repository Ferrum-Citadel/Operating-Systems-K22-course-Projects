#pragma once

//----------------Orismos struct gia to kathe entry tou inverted page table
typedef struct ipt_entry_str{
    int pid;    //process id 
    int vpn;    //arithmos page(virtual page number)
    int valid;  //Flag gia to an to frame (deixnetai apo to index) xrisimopoieitai    
    int dirty;   //Gia 0 den exoume allages. Gia 1 prepei na apothikeysoume tis allages ston disko
} IPT_ENTRY;

//------------------------Dilwnoyme synartiseis gia ton IPT

//Dimioyrgia Inverted Page Table megethous length.
IPT_ENTRY *create_ipt(int length);

//Eisagwgi entry se pinaka ipt
void insert_ipt_entry(IPT_ENTRY *ipt, int ppn, int pid, int vpn, int dirty); 

//Anazitisi enos entry se pinaka ipt. epistrefei to index tou an vrethei kai -1 an den vrethei
int search_ipt(IPT_ENTRY *ipt, int length, int pid, int vpn);

//Katastrofi pinaka ipt
void destroy_ipt(IPT_ENTRY *ipt, int length);

//Epistrefei 1 an o IPT einai gematos kai 0 an dei einai
//Se periptwsi poy den einai gematos, epistrefei kai
//to index toy prwtoy eleutheroy frame. Alliws -1
int is_full(IPT_ENTRY *ipt, int length, int *first_free_frame);
