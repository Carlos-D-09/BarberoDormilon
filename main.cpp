#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <conio.h>
#include <pthread.h>
#include <windows.h>
#include <winuser.h>

using namespace std;

#define SILLAS 3 //Sillas Disponibles
int tiempoCorte = 0;//Tiempo de corte de pelo

typedef int semaphore;

//Prototipo de funciones
void *customer (void *);
void *barber (void *);
void up (semaphore *);
void down (semaphore *);

semaphore semaforoBarbero=0; //Semaforo para identificar si el barbero esta dormido
semaphore semaforoCliente=0; //Semaforo para el conteo de cliente
semaphore semaforoSilla=1; //Semafora para la exclusión mutua de la silla

int esperando=0; //Numeros de clientes esperando

//Main function
int main (void) {
    pthread_t barb_t;
    pthread_create(&barb_t,NULL,barber,NULL);
    while(!GetAsyncKeyState(VK_ESCAPE)){
        pthread_t new cust_t;
        pthread_create(&cust_t,NULL,customer,NULL);
    }
    pthread_join(barb_t,NULL);
    return(0);
}

void *customer (void *n) {
    printf ("Customer:entrando hay %d esperando\n",waiting);
    down (&semaforoSilla);
    if (waiting < SILLAS) {
        waiting++;
        up (&semaforoCliente);
        up (&semaforoSilla);
        down (&semaforoBarbero);
        printf ("Customer:Me estan cortando el pelo.\n");
    }
    else {
        up (&semaforoSilla);
        printf ("Customer:Me fui no hay lugar.\n");
    } 
}

void *barber (void *j) {
    printf ("Barber:Empiezo a trabajar\n"); 
    while (TRUE) {
        down (&semaforoCliente);
        down (&semaforoSilla);
        up (&semaforoBarbero);
        up (&semaforoSilla);
        printf ("Barber:Comienzo el corte de pelo de un cliente quedan %d esperando.\n",waiting);
        sleep (tiempoCorte);
        printf ("Barber:Termine de cortar el pelo de un cliente quedan %d esperando.\n",waiting);
    }
}

void up (semaphore *sem) {
    *sem+=1;
}

void down (semaphore *sem) {
    while (*sem<=0){};
    *sem-=1;
} 