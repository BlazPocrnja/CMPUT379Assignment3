/**
* Bit array code provided by...
* Source: http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
* Date Accessed : 12/3/2016
*/

#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 1024			//2^10
#define MEMORY_SIZE 33554432	//2^25

typedef struct node{
	unsigned int key;
	int value;
	struct node *next;
	struct node *previous;
} node;

node *hash_table[HASH_SIZE] = {NULL};

int page_size = 0;				//in words(32 bit)
int window_size = 0;	
unsigned int mem_refs = 0; 

int *bit_array;

unsigned int *history = NULL;
unsigned int history_size = 0;


/**
* Allocates memory and assigns values to a new node struct
*/
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
* Remove node from linked list and free memory
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
	free(item);
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


/**
* Set bit at kth position
*/
void set_bit(int *A, unsigned int k ){
	A[k/32] |= 1 << (k%32);  // Set the bit at the k-th position in A[i]
}

/**
* Clear bit at kth position
*/
void clear_bit(int *A, unsigned int k){ 
	A[k/32] &= ~(1 << (k%32));  
}

/**
* Test bit at kth position
* Returns 1 if set
* 0 otherwise
*/
int test_bit(int *A, unsigned int k ){
	if ( (A[k/32] & (1 << (k%32) )) != 0 ){       
		return 1;
	}
	return 0;
}

/**
* Sets all bits to 0
*/
void reset_bits(){
	unsigned int n = MEMORY_SIZE/page_size;
	unsigned int i;
	for(i = 0; i < n; ++i){
		clear_bit(bit_array, i);
	}
}

/**
* Updates history of working set size
*/
void update_history(){
	unsigned int sum = 0;
	//n pages/bits to check
	unsigned int n = MEMORY_SIZE/page_size;
	unsigned int i;
	for(i = 0; i < n; ++i){
		sum += test_bit(bit_array, i);
	}

	if(history == NULL){
		history_size = 1;
		history = malloc(history_size*sizeof(unsigned int));
	}
	else{
		++history_size;
		history = (int*)realloc(history, history_size*sizeof(unsigned int));
	}
	history[history_size - 1] = sum;

}

/**
* Sets corresponding page bit with given address
* Increments mem_refs counter
* If counter is >= window_size update history and reset bits
*/
void reference_page(unsigned int address){
	//Take the floor of address/page_size (int cast does this)
	unsigned int page = address/page_size;

	if(!test_bit(bit_array,page)){
		set_bit(bit_array,page);
	}

	++mem_refs;	

	if(mem_refs >= window_size){
		update_history();
		reset_bits();
		mem_refs = 0;
	}
}

/**
* Prints history of working set sums
*/
void print_history(){
	printf("\n%d Complete Working Set(s)\n",history_size);
	printf("---------------------\n");
	unsigned int i;
	for(i = 0; i < history_size; ++i){
		printf("Working set %d had %d unique page accesses!\n", i + 1, history[i]);
	}
	printf("\n");
}

float avg_history(){
	unsigned int sum = 0;
	unsigned int i;
	for(i = 0; i < history_size; ++i){
		sum += history[i];
	}
	float avg = (float)sum/history_size;
	return avg;
}

/**
* Initializes page_size, window_size, mem_refs and the bit_array
*/
void init(int psize, int winsize){
	page_size = psize;
	window_size = winsize;
	mem_refs = 0;

	unsigned int n = MEMORY_SIZE/page_size;

	//We're using a bit array so we need to determine how many ints to allocate
	//n pages = n bits
	//(n bits/ x ints) = (32 bits / 1 int)
	//Take the ceiling of x (add 1, cast to int)
	unsigned int x = (n/32) + 1;

	bit_array = (int*)calloc(x , sizeof(int));
}

/**
* Insert a value into the given virtual address space
*/
void put(unsigned int address, int value){
	reference_page(address);
	node* new = node_new(address, value);

	//If a value exists at that location overwrite the old one
	node* item = ht_search(address);
	if(item != NULL){
		ht_delete(item);
	}
	ht_insert(new);
}

/**
* Get the value from a given virtual address space
*/
int get(unsigned int address){
	reference_page(address);
	node* item = ht_search(address);
	if(item == NULL){
		return 0;
	}
	return item->value;
}

/**
* 
*/
void done(){
	print_history();
	float avg = avg_history();
	printf("Average over all working set(s): %.2f\n\n" , avg);

	//Free bit_array
	free(bit_array);

	//Free history
	free(history);

	//Free hash_table memory
	unsigned int i;
	node* item;
	for(i = 0; i < MEMORY_SIZE; ++i){
		item = ht_search(i);
		if(item != NULL){
			ht_delete(item);
		}
	}
}

int main(int argc, char* argv[]){

	init(atoi(argv[1]),atoi(argv[2]));

	put(0,17);
	put(MEMORY_SIZE-5, 17);
	put(1024, 100000);
	put(MEMORY_SIZE-5, 17);
	put(1,17);
	put(MEMORY_SIZE-6, 17);
	put(MEMORY_SIZE-5, 17);
	put(MEMORY_SIZE-5, 22);

	printf("\n%d\n",get(1));
	printf("\n%d\n",get(MEMORY_SIZE-5));

	printf("\n%d\n",get(MEMORY_SIZE-6));
	printf("\n%d\n",get(2024));


	done();

	return 0;
}