#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 1024		//2^10

typedef struct node{
	unsigned int key;
	int value;
	struct node *next;
	struct node *previous;
} node;

node *hash_table[HASH_SIZE] = {NULL};

node* node_new(unsigned int key, int value){
	node* new = malloc(sizeof(node));
	new->key = key;
	new->value = value;
	new->next = NULL;
	new->previous = NULL;
	return new;
}

/**
* Hash function to map 2^25 address spaces to 2^10 buckets
*/
int hash(int address){
	return address % HASH_SIZE;
}

/**
* Insert at the beginning
*/
node* ll_insert(node* head, node* new){
	new->next = head;
	if(head != NULL){
		head->previous = new;
	}
	head = new;
	return head;
}

/**
* TODO Free Memory
*/
node* ll_delete(node* head, node* item){
	if(item->previous != NULL){
		item->previous->next = item->next;
	}
	if(item->next != NULL){
		item->next->previous = item->previous;
	}
	if(item == head){
		head = item->next;
	}
	return head ;
}

/**
* Search for an item, using the key
* Returns NULL if no match
*/
node* ll_search(node* head, unsigned int key){
	for(;head != NULL; head = head->next){
		if(head->key == key){
			return head;
		}
	}
	return NULL;
}

/**
* Insert into hash_table
*/
void ht_insert(node* item){
	unsigned int key = item->key;
	int index = hash(key);
	hash_table[index] = ll_insert(hash_table[index],item);
}

/**
* Delete from hash_table
*/
void ht_delete(node* item){
	int key= item->key;
	int index = hash(key);
	hash_table[index] = ll_delete(hash_table[index],item);
}

/**
* Search hash_table for an item, using the key
*/
node* ht_search(unsigned int key){
	int index = hash(key);
	return ll_search(hash_table[index],key);
}


void init(int psize, int winsize){
}

void put(unsigned int address, int value){
	node* new = node_new(address, value);
	ht_insert(new);
}


int get(unsigned int address){
	node* item = ht_search(address);
	return item->value;
}

void done(){

}

int main(){
	int i;
	for(i = 0; i < 10; ++i){
		put(i, i+1);
		printf("%d \n",get(i));
	}
	return 0;
}