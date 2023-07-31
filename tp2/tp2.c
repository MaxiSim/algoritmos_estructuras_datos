#include "tp2.h"
#include <stdlib.h>
#include <stdbool.h>

struct node;
typedef struct node node_t;

struct node {
    void* value;
    node_t* next;
    node_t* prev;
};

struct list {
    node_t* head;
    node_t* tail;
    size_t size;
};

struct list_iter {
    list_t* list;
    node_t* curr;
};




list_t *list_new(){
    list_t* list = malloc(sizeof(list_t));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


size_t list_length(const list_t *list){
    return list->size;
}


bool list_is_empty(const list_t *list){
    if (list_length(list) == 0){
        return true;
    }
    return false;
}


bool list_insert_head(list_t *list, void *value){
    node_t* new = malloc(sizeof(node_t));
    if(!new) return false;
    
    new->value = value;
    new->prev = NULL;
    if(list_length(list) == 0){
        list->head = new;
        list->tail = new;
        list->head->next = NULL;
    } else {
        list->head->prev = new;
        new->next = list->head;
        list->head = new;
    }
    list->size++;
    return true;
}


bool list_insert_tail(list_t *list, void *value){
    if(list_length(list) == 0) return list_insert_head(list, value);
    
    node_t* new = malloc(sizeof(node_t));
    if(!new) return false;
      
    new->value = value;
    new->next = NULL;
    list->tail->next = new;
    new->prev = list->tail;
    list->tail = new;
    list->size++;

    return true;
}


void *list_peek_head(const list_t *list){
    if(list_length(list) == 0) return NULL;
    return list->head->value;
}


void *list_peek_tail(const list_t *list){
    if(list_length(list) == 0) return NULL;
    return list->tail->value;
}


void *list_pop_head(list_t *list){
    if(list_length(list) == 0) return NULL;
    void* pop_value = list->head->value;
    node_t* pop_head = list->head;
   
    if (list_length(list) == 1){
        list->head = NULL;
        list->tail = NULL;
    } else if (list_length(list) > 1) {  
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    list->size--;
    free(pop_head);
    return pop_value;
}


void *list_pop_tail(list_t *list){
    if(list_length(list) == 0) return NULL;
    void* pop_value = list->tail->value;
    node_t* pop_tail = list->tail;

    if (list->size == 1){
        list->tail = NULL;
        list->head = NULL;
    } else if (list->size > 1){
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    } 
    list->size--;
    free(pop_tail);
    return pop_value;
}


void list_destroy(list_t *list, void destroy_value(void *)){
    if (list_length(list) == 0){
        free(list);
        return;
    }
    while (list_length(list) > 0){
        if (destroy_value){
            destroy_value(list_pop_tail(list));
        } else {
            list_pop_tail(list);
        }
    }
    free(list);
}


list_iter_t *list_iter_create_head(list_t *list){
    list_iter_t* new = malloc(sizeof (list_iter_t));
    if (!new) return NULL;
    new->list = list;
    new->curr = list->head;
    return new;
}


list_iter_t *list_iter_create_tail(list_t *list){
    list_iter_t* new = malloc(sizeof (list_iter_t));
    if (!new) return NULL;
    new->list = list;
    new->curr = list->tail;
    return new;
}


bool list_iter_forward(list_iter_t *iter){
    if(list_length(iter->list) == 0) return false;
    if(!iter->curr->next) return false;
    iter->curr = iter->curr->next;
    return true;
}


bool list_iter_backward(list_iter_t *iter){
    if(list_length(iter->list) == 0) return false;
    if(!iter->curr->prev) return false;
    iter->curr = iter->curr->prev;
    return true;
}


void *list_iter_peek_current(const list_iter_t *iter){
    if(list_length(iter->list) == 0) return NULL;
    return iter->curr->value;
}


bool list_iter_at_last(const list_iter_t *iter){
    if ((list_length(iter->list) == 0) || !iter->curr->next) return true;
    return false;
}


bool list_iter_at_first(const list_iter_t *iter){
    if ((list_length(iter->list) == 0) || !iter->curr->prev) return true;
    return false;
}


void list_iter_destroy(list_iter_t *iter){
    free(iter);
}


bool list_iter_insert_after(list_iter_t *iter, void *value){
    if (list_length(iter->list) == 0){
        bool result = list_insert_head(iter->list, value);
        iter->curr = iter->list->head;
        return result;
    } else if (list_iter_at_last(iter)){
        return list_insert_tail(iter->list, value);
    } 

    node_t* new = malloc(sizeof(node_t));
    if(!new) return false;

    new->value = value;
    new->next = iter->curr->next;
    new->prev = iter->curr;
    iter->curr->next = new;
    iter->curr->next->prev = new;
    iter->list->size++;

    return true;
}


bool list_iter_insert_before(list_iter_t *iter, void *value){
    if(list_length(iter->list) == 0) {
        bool result = list_insert_head(iter->list, value);
        iter->curr = iter->list->head;
        return result;
    } else if (list_iter_at_first(iter)){
        return list_insert_head(iter->list, value);
    } 

    node_t* new = malloc(sizeof(node_t));
    if(!new){
        return false;
    }

    new->value = value;
    new->prev = iter->curr->prev;
    new->next = iter->curr;
    iter->curr->prev->next = new;
    iter->curr->prev = new;
    iter->list->size++;

    return true;
}


void *list_iter_delete(list_iter_t *iter){
    if (list_length(iter->list) == 0) return NULL;

    void* del_value = iter->curr->value;
    node_t* del_curr = iter->curr;
    
    if (iter->list->size == 1){
        iter->list->head = NULL;
        iter->list->tail = NULL;
        iter->curr = iter->list->head;
    } else if (list_iter_at_last(iter)){
        list_iter_backward(iter);
        iter->curr->next = NULL;
        iter->list->tail = iter->curr;
    } else if (list_iter_at_first(iter)){
        list_iter_forward(iter);
        iter->curr->prev = NULL;
        iter->list->head = iter->curr;
    } else {
        list_iter_forward(iter);
        iter->curr->prev = del_curr->prev;
        del_curr->prev->next = iter->curr;
    }
    iter->list->size--;
    free(del_curr);
    return del_value;
}

