/* p3helper.c
 * Gabriel Barrera
 * Program 3
 * CS570
 * Professor John Carroll
 * SDSU
 * 10/30/2018
 *
 * This is the only file you may change. (In fact, the other files should
 * be symbolic links to:
 *   ~cs570/Three/p3main.c
 *   ~cs570/Three/p3.h
 *   ~cs570/Three/Makefile
 *   ~cs570/Three/CHK.h    )
 */
#include "p3.h"

/* You may put any semaphore (or other global) declarations/definitions here: */
sem_t door;     //Control access to gym
sem_t jog;      //Control jogger flow   
sem_t shoot;    //Control shooter flow
int njog;       //Counter for joggers
int nshoot;     //Counter for shooters

/* General documentation for the following functions is in p3.h
 * Here you supply the code:
 */
void initstudentstuff(void) {
    sem_init(&door,0,1);
    sem_init(&jog,0,1);
    sem_init(&shoot,0,1);
    njog = 0;
    nshoot = 0;
}

//Lock gym for a specific kind
void prolog(int kind) {
    //If current person is a jogger    
    if (kind == JOGGER){
        //Lock gym for just jog
        sem_wait(&jog);
        if (njog == 0)
            //Lock gym for just joggers
            sem_wait(&door);         
        njog++;
        //When done, open door
        sem_post(&jog);            
    } 
    //If current person is a shooter
    else if (kind == SHOOTER){
        //Lock gym for a shooter
        sem_wait(&shoot);
        if (nshoot == 0)
            //Lock gym for just shooters
            sem_wait(&door);           
        nshoot++;
        //When done, open door
        sem_post(&shoot); 
    }   
}

//Regulate kind count and reopen gym as necessary
void epilog(int kind) {
    //If jogger is done, remove from count
    if (kind == JOGGER)
        njog--;
    //If shooter is done, remove from count
    else if (kind == SHOOTER)
        nshoot--;
    //If the gym is empty, open the door up for the next kind
    if (njog == 0 && nshoot == 0)
        sem_post(&door);
}  

