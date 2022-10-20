#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <conio.h>
#include <pthread.h>
#include <windows.h>
#include <winuser.h>

using namespace std;

#define TRUE 1
#define FALSE 0
#define CHAIRS 3 //Sillas Disponibles
#define CANT_CUST 5 //Cantidad de Clientes
#define T_CUST 0 //Tiempo de llegada de Clientes
#define T_CORTE 3 //Tiempo de corte de pelo

typedef int semaphore;

//Prototipo de funciones
void *customer (void *);
void *barber (void *);
void up (semaphore *);
void down (semaphore *);

semaphore sem_barber=0, sem_customer=0,sem_mutex=1;
int waiting=0;

//Main function
int main (void) {
    pthread_t barb_t,cust_t[CANT_CUST];
    int i;
    int exit = 0;
    while(!GetAsyncKeyState(VK_ESCAPE)){
        cout<<"prueba"<<endl;
    }
    // pthread_create(&barb_t,NULL,barber,NULL);
    // for (i=0;i<CANT_CUST;i++){
    //     sleep(T_CUST);
    //     pthread_create(&cust_t[i],NULL,customer,NULL);
    // }
    // pthread_join(barb_t,NULL);
    return(0);
}

void *customer (void *n) {
printf ("Customer:entrando hay %d esperando\n",waiting);
down (&sem_mutex);
if (waiting < CHAIRS) {
waiting++;
up (&sem_customer);
up (&sem_mutex);
down (&sem_barber);
printf ("Customer:Me estan cortando el pelo.\n");
}
else {
up (&sem_mutex);
printf ("Customer:Me fui no hay lugar.\n");
} 
}

void *barber (void *j) {
printf ("Barber:Empiezo a trabajar\n"); 
while (TRUE) {
down (&sem_customer);
down (&sem_mutex);
waiting--;
up (&sem_barber);
up (&sem_mutex);
printf ("Barber:Comienzo el corte de pelo de un cliente quedan %d esperando.\n",waiting);
sleep (T_CORTE);
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