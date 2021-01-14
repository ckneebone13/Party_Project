/*
		Author: Caraline Kneebone & Shane Combs
		Assignment Number: 3
		Date of Submission: 11/11/2020
		Name of this file: main.c
		Description of the program: taxi student party problem 
                            using threads and semaphors
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>//for atoi
#include <stdlib.h>//for atoi

#define SEATS 4

int numOfStudents;
int numOfTaxis;
int maximumNumOfSecs;
unsigned int randNum =1;

sem_t taxi_mutex;//shared taxi resource for students
sem_t curb_mutex;//taxi curb shared resource for taxi
int taxiPickUpQueue[];//max num of seats
int taxiSeatedQueue[];//seated
int numOfStudentsInQueue;//need to seat queue
int seatedStudents;

/*
 * Function Name: small_num_generator
 * Input to function: void
 * Output (return value): int
 * Description: Return a random number from 0 to maximumNumOfSecs input.
 */
int small_num_generator(){
    int rand_num = 0;
    int max_size = maximumNumOfSecs;
    time_t t;

    srand((unsigned) time(&t));//creates seed for random number generator function
    rand_num = (rand() % max_size);//random number generator
    return(rand_num);
}

/*
 * Function Name: studentThreadTask
 * Input to function: int
 * Output (return value): void
 * Description: Task that is ran for each student thread created. Once a student thread is done sleeping, it will
 * alter the queue (line) for the students to get picked up by the taxi by their student id.
 */
void studentThreadTask(int i) {
    //sleep(randNum);
    printf("Student %i: I am at the party. It is way more fun than what I expected...\n", i);
    sem_wait(&taxi_mutex);//grab taxi mutex to add students to pickup queue
    if (numOfStudentsInQueue < numOfStudents) {
        taxiPickUpQueue[numOfStudentsInQueue] = i;//add student ID to pickup queue
        printf("Student %d: I am done partying. I better get back to that calculus homework that is due tomorrow…\n", i);
        numOfStudentsInQueue++;//increase counter for number of students in queue
    }
    sem_post(&taxi_mutex);
    pthread_exit(0);
}

/*
 * Function Name: taxiThreadTask
 * Input to function: int
 * Output (return value): void
 * Description: Task that is ran for each taxi thread created. Taxi will take its place in the curb and wait and remove students
 * from the taxiPickupQueue and add it to the taxi.
 */
void taxiThreadTask(int j) {
    //sleep(randNum);
    sem_wait(&curb_mutex);//mutex to 0
    if (numOfStudentsInQueue < 1){//if no students are ready to be picked up
        //printf("\tif number of students in queue %d\n", numOfStudentsInQueue);
        printf("Taxi %d: I arrived at the curb.. There is no one that wants to go home… " 
                "I might as well take a nap..\n", j);
    }
    else{//if there are students to be picked up
        for (int i = 0; i <= SEATS; i++){//pickup students until seats are full
            //printf("\t\ttaxipickupqueue at i(%d): %d\n", i, taxiPickUpQueue[i]);
            taxiSeatedQueue[i] = taxiPickUpQueue[i];//grab student id 
            seatedStudents++//increase seat counter
            printf("\tTaxi %d: I have another student. The student %d, … When will I find more passengers? Sigh... The "
                "students seem to have too much fun these days.\n", j, taxiSeatedQueue[i]); 
            
            if (seatedStudents == SEATS){//when seats are full
                printf("\tTaxi %d: I have all four... %i %i %i %i Time to drive... BYE\n", j, 
                    taxiSeatedQueue[seatedStudents-3], taxiSeatedQueue[seatedStudents-2], 
                    taxiSeatedQueue[seatedStudents-1], taxiSeatedQueue[seatedStudents]);
                seatedStudents = 0;
            }
        }
    }
    sem_post(&curb_mutex);//mutex back to 1
    //for(int u = 0; u < numOfStudentsInQueue; u++){ printf("%i ", taxiPickUpQueue[u]); }
    //printf("\n");
    pthread_exit(0);
}

/*
 * Function Name: main
 * Input to function: int, char
 * Output (return value): void
 * Description: main function of the task. Takes in input of the command and then the creation process of the taxis
 * student threads which starts their tasks respectively.
 */
void main(int argc, char *argv[]) {

    sem_init(&taxi_mutex, 0, 1);
    sem_init(&curb_mutex, 0, 1);
    int option;
    //gather options from user
    while((option = getopt(argc, argv, "s:t:m:")) != -1) {
        switch (option) {
            case 'm':
                maximumNumOfSecs = atoi(optarg);
                continue;
            case 't':
                numOfTaxis = atoi(optarg);
                continue;
            case 's':
                numOfStudents = atoi(optarg);
                taxiPickUpQueue[numOfStudents];
                continue;
        }
    }
    
    randNum = small_num_generator();// (in seconds)
    
    //thread creation
    pthread_t thread_id_student[numOfStudents];
    pthread_t thread_id_taxi[numOfTaxis];

    //printf("thread count %i \n", numOfStudents + numOfTaxis);

    for (int i = 0; i < numOfStudents; i++) {//create threads for user input number of students
        //sleep(randNum);
        pthread_create(&thread_id_student[i], NULL, studentThreadTask, (void *)i);
    }

    for (int j = 0; j < numOfTaxis; j++) {//create threads for user input number of taxis
       // sleep(randNum);
        pthread_create(&thread_id_taxi[j], NULL, taxiThreadTask, (void *)j); 
    }
    //join threads for user input number of students
    for (int k = 0; k < numOfStudents; k++) {
        pthread_join(thread_id_student[k], NULL);
        //printf("student pthread join\n");
    }
    //join threads for user input number of taxis
    for (int n = 0; n < numOfTaxis; n++) {
        pthread_join(thread_id_taxi[n], NULL);
        //printf("taxi pthread join\n");
    }
    //destroy mutex's when finished
    sem_destroy(&taxi_mutex);
    sem_destroy(&curb_mutex);
}
