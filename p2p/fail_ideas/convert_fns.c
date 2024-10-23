#include "convert_fns.h"


typedef struct{
    void* pointer;
    /* char* type; */
    FT_pointers_list* next;
}FT_pointers_list;
// 11111111111111111111111111111111

FT_pointers_list* crFT_pointers_list(void* ptr/* , char* ptr_type */){
    FT_pointers_list list = (FT_pointers_list*)malloc(sizeof(FT_pointers_list));
    list->pointer = ptr;
    /* list->type = (char*)malloc(strlen(ptr_type));
    strcpy(list->type, ptr_type); */
    list->next = NULL;
    return list;
}
FT_pointers_list* tail_FT_pointers_list(FT_pointers_list* list){
    FT_pointers_list* tail = list;
    while(tail->next != NULL){
        tail = tail->next;
    }
    return tail;
}
void expand_FT_pointers_list(FT_pointers_list* list, void* ptr/* , char* ptr_type */){
    FT_pointers_list* new_node = crFT_pointers_list(ptr/* , ptr_type */);
    tail_FT_pointers_list(FT_pointers_list* list)->next = new_node;
}
void drFT_pointers_list(FT_pointers_list* list){
    FT_pointers_list* current = list;
    if(current->next == NULL){
        /* free(current->type); */
        free(current->pointer);
        free(current);
    }
    else{
        FT_pointers_list* next = list->next;
        while(next != NULL){
            next = current->next;
            /* free(current->type); */
            free(current->pointer);
            free(current);
            current = next;
        }
    }
}


FTint wrap_int(int val, FTpointers_list* list){
    FTint result = (char*)malloc(sizeof(val));
    for(int i = sizeof(val) / 8 - 1; i >= 0; i--){
        result[i] = val >> (i * 8);
    }
    expand_FT_pointers_list(list, result/* , "int" */);
    return result;
}

FTlong_int wrap_long_int(long val, FTpointers_list* list){
    FTint result = (char*)malloc(sizeof(val));
    for(int i = sizeof(val) / 8 - 1; i >= 0; i--){
        result[i] = val >> (i * 8);
    }
    expand_FT_pointers_list(list, result/* , "long" */);
    return result;
}

FTshort_int wrap_short_int(short val, FTpointers_list* list){
    FTint result = (char*)malloc(sizeof(val));
    for(int i = sizeof(val) / 8 - 1; i >= 0; i--){
        result[i] = val >> (i * 8);
    }
    expand_FT_pointers_list(list, result/* , "short" */);
    return result;
}

FTfloat wrap_float(float val, FTpointers_list* list){
    FTint result = (char*)malloc(sizeof(val));
    for(int i = sizeof(val) / 8 - 1; i >= 0; i--){
        result[i] = val >> (i * 8);
    }
    expand_FT_pointers_list(list, result/* , "float" */);
    return result;
}

FTdouble wrap_double(double val, FTpointers_list* list){
    FTint result = (char*)malloc(sizeof(val));
    for(int i = sizeof(val) / 8 - 1; i >= 0; i--){
        result[i] = val >> (i * 8);
    }
    expand_FT_pointers_list(list, result/* , "double" */);
    return result;
}
