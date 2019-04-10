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
	// Sem_init(&caretakerWait, 1, 0);
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

		printf("Animal %d queued\n", b);
		// if animalsInTheYard > 0, then wait
		if(caretakersInTheYard > 0){
			Cond_wait(&animalWait, &mutexForAnimalWait);
		}

		Mutex_lock(&forAtomicity);
		animalsInTheYard++;
		Mutex_unlock(&forAtomicity);
		printf("Animal %d enters\n", b);
		usleep( random()%(long)2e6 );

		//Animal exits the yard;
		Mutex_lock(&forAtomicity);
		animalsInTheYard--;
		Mutex_unlock(&forAtomicity);
		printf("Animal %d leaves\n", b);
		usleep( random()%(long)2e6 );

		// if the yard is empty signal the caretaker
		if(animalsInTheYard == 0){
			Cond_signal(&caretakerWait);
		}

	}

}

void caretakerInstance(void* a){

	int b = (int)a;
	printf("caretaker created: %d\n", b);

	while(true){

		printf("Caretaker %d queued\n", b);
		// if animalsInTheYard > 0, then wait
		if(animalsInTheYard > 0){
			Cond_wait(&caretakerWait, &mutexForCaretakerWait);
		}

		Mutex_lock(&mutexForCaretakerInTheYard);
		Mutex_lock(&forAtomicity);
		caretakersInTheYard++;
		Mutex_unlock(&forAtomicity);
		printf("Caretaker %d enters\n", b);
		usleep( random()%(long)2e6 );

		Mutex_lock(&forAtomicity);
		caretakersInTheYard--;
		Mutex_unlock(&forAtomicity);
		printf("Caretaker %d leaves\n", b);
		usleep( random()%(long)2e6 );

		Mutex_unlock(&mutexForCaretakerInTheYard);

		// if the yard is empty signal the caretaker
		if(caretakersInTheYard == 0){
			Cond_signal(&animalWait);
		}

	}

}

int main(int argc, char** argv){

	printf("Welcome to the Animal Clinic!!\n\n");

	int numOfAnimals, numOfCaretakers;

	if(argc == 3){
		numOfAnimals = atoi(argv[1]);
		numOfCaretakers = atoi(argv[2]);
		printf("Number of animals: %d\n", numOfAnimals);
		printf("Number of care-takers: %d\n", numOfCaretakers);
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
