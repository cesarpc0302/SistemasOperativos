/*forks*/



#include <stdio.h>

#include <stdlib.h>

#include <sys/sem.h>

#include <unistd.h>



#include "semun.h"



static int set_semvalue(void);

static void del_semvalue(void);

static int semaphore_p(int sem_num);

static int semaphore_v(int sem_num);





static int sem_id;



int main(int argc, char *argv[]) {



	int i;

	int pause_time;

	//char op_char = 'O';

	int goalA = 0;

	int goalB = 0;

	printf("Start Match\n");



	srand((unsigned int)getpid());

	sem_id = semget((key_t)1234,3,0666|IPC_CREAT);



	if(argc > 1)

	{

		if(!set_semvalue()) {

			fprintf(stderr, "Failed to initialize semaphore\n");

			exit(EXIT_FAILURE);

		}

		//op_char = 'X';

		sleep(2);

	}


	pid_t pid;



	/*child*/

	/*Team 1*/

	pid = fork();
	pid = fork();
	pid = fork();




	if (pid < 0) /*error*/

	{

		fprintf(stderr, "fork failed");

		return 1;

	}

	else if (pid == 0) /*child process*/

	{

		printf("process created PID: %d\n",getpid() );

		/*Wait time*/

		pause_time = ((rand() % 15) + 5);

		sleep(pause_time);

		printf("trying to get the ball PID: %d\n",getpid() );

		if (!semaphore_p(0))

		{
			printf("unable to get the ball PID: %d\n",getpid() );

			exit(EXIT_FAILURE);

		}

		printf("PID1: %d - got the ball\n", getpid());

			//pause_time = rand() % 1000;

			//sleep(pause_time);

			////printf("%c\n", op_char);

		for (i = 0; i < 3; ++i)

		{
			printf("trying to get the goal PID: %d\n",getpid() );

			if (!semaphore_p(2))

			{

				//pause_time = rand() % 1000;

				sleep(1);

				exit(EXIT_FAILURE);

			}

			else

			{

				printf("GOOOOOLLLLL PID: %d\n",getpid() );
				goalB += 1;

			}

			if(!semaphore_v(2)){

				exit(EXIT_FAILURE);

			}

		}



		if(!semaphore_v(0)){

			exit(EXIT_FAILURE);

		}

		

		if (!semaphore_p(1))

			{

				exit(EXIT_FAILURE);

			}

			else

			{

				printf("PID: %d is defending\n",getpid() );

				pause_time = rand() % 2;

				sleep(pause_time);

			}

			if(!semaphore_v(1)){

				exit(EXIT_FAILURE);

			}



/*		printf("\n%d - finished\n", getpid());

		if (argc > 1)

		{

			pause_time = rand() % 15 + 5;

			sleep(pause_time);

			del_semvalue();

		}

		exit(EXIT_SUCCESS);*/

	}

	else /*parent process*/

	{

		/*print score*/

		wait(NULL);

		printf("Child complete");

		printf("\n%d - match finished\n", getpid());

		if (argc > 1)

		{

			del_semvalue();

		}

		exit(EXIT_SUCCESS);

	}



	return 0;

}



static void del_semvalue(void)

{

	union semun sem_union;

	if(semctl(sem_id,0,IPC_RMID,sem_union) == -1){

		fprintf(stderr, "Failed to delete semaphore\n");

	}

}

static int set_semvalue(void)

{
	union semun sem_union;
	sem_union.val = 1;
	
	if(semctl(sem_id,0,SETVAL,sem_union) == -1){

		return(0);

	}
	return(1);

}


/*

ball = 0

goal team A = 1

goal team B = 2

*/



static int semaphore_p(int sem_num)

{

	struct sembuf sem_b;



	sem_b.sem_num = sem_num;

	sem_b.sem_op = -1; /*P()*/

	sem_b.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sem_b, 1) == -1)

	{

		fprintf(stderr, "semaphore_p failed\n");

		return(0);

	}

	return(1);

}



static int semaphore_v(int sem_num)

{

	struct sembuf sem_b;



	sem_b.sem_num = sem_num;

	sem_b.sem_op = 1; /*V()*/

	sem_b.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sem_b, 1) == -1)

	{

		fprintf(stderr, "semaphore_v failed\n");

		return(0);

	}

	return(1);

}

