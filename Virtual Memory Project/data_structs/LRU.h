#pragma once

//-----------Dilwsi sinartisewn
//Dimioyrgei ton pinaka me ta ages gia kathe zeygos pid kai page number
int *create_age_arr(int length);

//Diagrafi toy age table
void destroy_age_arr(int *age_array);

//Epistrefei to index sto opoio antistoixei to palaiotero entry
int get_victim(int *age_array, int length);

//Anathetei  se sygekrimeno index to trexwn timestamp
void set_timestamp(int *age_array, int index, int length);