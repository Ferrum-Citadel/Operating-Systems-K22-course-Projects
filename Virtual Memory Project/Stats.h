#pragma once

typedef struct Stat_str{
    int read_num;
    int write_num;
    int page_fault_num;
    int ref_num;
    int used_frame_num;
} STATS;

