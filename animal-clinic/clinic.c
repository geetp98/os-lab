#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mythreads.h"

int cur_animal;
int cur_caretaker;

void animal(void){
	printf("animal\n");
}

void caretaker(void){
	printf("caretaker\n");
}

int main(int argc, char** argv){

	printf("Welcome to the Animal Clinic!!\n\n");

	int num_of_animals, num_of_caretakers;

	if(argc == 3){
		num_of_animals = atoi(argv[1]);
		num_of_caretakers = atoi(argv[2]);
		printf("Number of animals: %d\n", num_of_animals);
		printf("Number of care-takers: %d\n", num_of_caretakers);
	}
	else exit(0);

	pthread_t *animals = malloc(sizeof(pthread_t)*num_of_animals);
	pthread_t *caretakers = malloc(sizeof(pthread_t)*num_of_caretakers);
	int i = 0;

	for(i = 0; i < num_of_animals; i++){
		Pthread_create(&animals[i], NULL, (void*)&animal, NULL);
	}
	for(i = 0; i < num_of_caretakers; i++){
		Pthread_create(&caretakers[i], NULL, (void*)&caretaker, NULL);
	}

	for(i = 0; i < num_of_animals; i++){
		Pthread_join(animals[i], NULL);
	}
	for(i = 0; i < num_of_caretakers; i++){
		Pthread_join(caretakers[i], NULL);
	}

	return 0;
}
