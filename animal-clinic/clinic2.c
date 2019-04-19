#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "mythreads.h"

int currentAnimal;
int currentCaretaker;

int animalsInTheYard;
int caretakersInTheYard;

pthread_cond_t caretakerWait;
pthread_mutex_t mutexForCaretakerWait;

pthread_cond_t animalWait;
pthread_mutex_t mutexForAnimalWait;

pthread_mutex_t mutexForCaretakerInTheYard;
pthread_mutex_t forAtomicity;

void init(){

	int animalsInTheYard = 0;
	int caretakersInTheYard = 0;
	
	Cond_init(&caretakerWait);
	Mutex_init(&mutexForCaretakerWait);

	Cond_init(&animalWait);
	Mutex_init(&mutexForAnimalWait);

	Mutex_init(&mutexForCaretakerInTheYard);
	Mutex_init(&forAtomicity);

}

void animalInstance(void* a){

	int b = (int)a;
	printf("animal created: %d\n", b);

	while(true){

		// animal enters the yard
		Mutex_lock(&forAtomicity);
		animalsInTheYard++;
		
		// if caretakersInTheYard > 0, then wait
		if(caretakersInTheYard > 0){
			printf("Animal %d queued\n", b);
			Cond_wait(&animalWait, &mutexForAnimalWait);
		}

		printf("Animal %d enters (%d)\n", b, animalsInTheYard);
		Mutex_unlock(&forAtomicity);
		usleep( random()%(long)2e6 );

		// animal leaves the yard
		Mutex_lock(&forAtomicity);
		animalsInTheYard--;
		printf("Animal %d leaves (%d)\n", b, animalsInTheYard);
		
		// if the yard is empty signal the caretaker
		if(animalsInTheYard == 0){
			Cond_signal(&caretakerWait);
		}
		Mutex_unlock(&forAtomicity);
		usleep( random()%(long)2e6 );
		
	}

}

void caretakerInstance(void* a){

	int b = (int)a;
	printf("caretaker created: %d\n", b);

	while(true){

		// caretaker enters the yard
		Mutex_lock(&mutexForCaretakerInTheYard);
		
		// if animalsInTheYard > 0, then wait
		if(animalsInTheYard > 0){
			printf("Caretaker %d queued\n", b);
			Cond_wait(&caretakerWait, &mutexForCaretakerWait);
		}

		caretakersInTheYard++;
		printf("Caretaker %d enters (%d)\n", b, caretakersInTheYard);
		usleep( random()%(long)2e6 );

		// caretaker leaves the yard
		caretakersInTheYard--;
		printf("Caretaker %d leaves (%d)\n", b, caretakersInTheYard);
		usleep( random()%(long)2e6 );
		
		// if the yard is empty signal the animals
		if(caretakersInTheYard == 0){
			Cond_signal(&animalWait);
		}

		Mutex_unlock(&mutexForCaretakerInTheYard);

	}

}

int main(int argc, char** argv){

	printf("\nWelcome to the Animal Clinic!!\n\n");

	int numOfAnimals, numOfCaretakers;

	if(argc == 3){
		numOfAnimals = atoi(argv[1]);
		numOfCaretakers = atoi(argv[2]);
		printf("Number of animals: %d\n", numOfAnimals);
		printf("Number of care-takers: %d\n\n", numOfCaretakers);
	}
	else exit(0);

	init();

	pthread_t *animals = malloc(sizeof(pthread_t)*numOfAnimals);
	pthread_t *caretakers = malloc(sizeof(pthread_t)*numOfCaretakers);
	int i = 0;

	for(i = 0; i < numOfAnimals; i++){
		Pthread_create(&animals[i], NULL, (void*)&animalInstance, (void*)(size_t)i);
	}
	for(i = 0; i < numOfCaretakers; i++){
		Pthread_create(&caretakers[i], NULL, (void*)&caretakerInstance, (void*)(size_t)i);
	}

	for(i = 0; i < numOfAnimals; i++){
		Pthread_join(animals[i], NULL);
	}
	for(i = 0; i < numOfCaretakers; i++){
		Pthread_join(caretakers[i], NULL);
	}

	return 0;
}
