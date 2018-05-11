/*forks*/



#include <stdio.h>

#include <stdlib.h>

#include <sys/sem.h>

#include <unistd.h>

#include <signal.h>



#include "semun.h"



static int set_semvalue(void);

static void del_semvalue(void);

static int semaphore_p(int sem_num);

static int semaphore_v(int sem_num);

static void playerTeamA(int goalB);
static void playerTeamB(int goalA);



static int sem_id;



int main(void) {



	int i;
	
	int n = 4; // Total players

	int pause_time;

	int goalA;

	int goalB;

	printf("Start Match\n");


	srand((unsigned int)getpid());

	sem_id = semget((key_t)1234,3,0666|IPC_CREAT);



	if(1)

	{

		if(!set_semvalue()) {

			fprintf(stderr, "Failed to initialize semaphore\n");

			exit(EXIT_FAILURE);

		}

		goalA = 0;
		goalB = 0;
		printf("Semaphores created\n");
		sleep(2);

	}


	/*child*/

	/*Team 1*/
	
	pid_t pids[10];
	
	for(i = 0; i < n; i++)
	{
		if((pids[i] = fork()) < 0)
		{
			perror("fork error");
			abort();
		}
		else if (pids[i] == 0) 
		{
			
			if(i % 2 == 0)
			{
				printf("Player ID: %d created; team B\n",getpid() );
				playerTeamB(goalA);
			}
			else
			{
				printf("Player ID: %d created; team A\n",getpid() );
				playerTeamA(goalB);
			}
			exit(0);
		}
	}

	sleep(300);
	for(i = 0; i < 10; i++)
	{
		kill(pids[i], SIGKILL);
		printf("process %d terminated\n", pids[i]);	
	}
	
	
	printf("\n%d - match finished\n", getpid());

	del_semvalue();
	
	printf("\nFinal Score: Team A %d  --  %d Team B\n", goalB, goalA);

	exit(EXIT_SUCCESS);


	return 0;

}


static void playerTeamA(int goalB)
{
	int i;

	int pause_timeA;


	while(1)
	{

		/*Wait time*/

		pause_timeA = ((rand() % 15) + 5);
		printf("Pause time A: %d\n", pause_timeA );
		sleep(pause_timeA);

		printf("PID: %d trying to get the ball\n",getpid() );

		if (!semaphore_p(0))

		{
			printf("PID: %d unable to get the ball\n",getpid() );
			exit(EXIT_FAILURE);

		}
		else
		{

			printf("PID: %d got the ball\n", getpid());

			for (i = 0; i < 3; ++i)
			{
				printf("PID: %d trying to get the goal\n",getpid() );


				if (!semaphore_p(2))

				{
					//pause_time = rand() % 1000;

					sleep(1);
					printf("PID: %d unable to get the goal\n",getpid() );

					exit(EXIT_FAILURE);

				}

				else

				{

					printf("GOOOOOLLLLL of PID: %d\n",getpid() );
					goalB += 1;
					printf("Team A scores!!!! %d goals\n", goalB);
					i = 3;
				}


				if(!semaphore_v(2))
				{
					exit(EXIT_FAILURE);
				}

			}
		}

		if(!semaphore_v(0))
		{
			exit(EXIT_FAILURE);
		}

			
		if (!semaphore_p(1))
		{

			exit(EXIT_FAILURE);

		}

		else

		{

			printf("PID: %d is defending\n",getpid() );

			pause_timeA = rand() % 2;
			printf("Pause time A: %d\n", pause_timeA );
			sleep(2);

		}

		if(!semaphore_v(1)){

			exit(EXIT_FAILURE);
		}
	}
}


static void playerTeamB(int goalA)
{	

	int i;

	int pause_timeB;

	while(1)
	{

	/*Wait time*/

		pause_timeB = ((rand() % 15) + 5);
		printf("Pause time B: %d\n", pause_timeB );
		sleep(pause_timeB);

		printf("PID: %d trying to get the ball\n",getpid() );

		if (!semaphore_p(0))
		{
			printf("PID: %d unable to get the ball\n",getpid() );
			exit(EXIT_FAILURE);

		}
		else
		{

			printf("PID: %d got the ball\n", getpid());

			for (i = 0; i < 3; ++i)
			{
				printf("PID: %d trying to get the goal\n",getpid() );


				if (!semaphore_p(1))

				{
					//pause_time = rand() % 1000;

					sleep(1);
					printf("PID: %d unable to get the goal\n",getpid() );

					exit(EXIT_FAILURE);

				}

				else

				{
					printf("GOOOOOLLLLL of PID: %d\n",getpid() );
					goalA += 1;
					printf("Team B scores!!!! %d goals\n", goalA);
					i = 3;
				}


				if(!semaphore_v(1)){

					exit(EXIT_FAILURE);
				}

			}
		}

		if(!semaphore_v(0)){

			exit(EXIT_FAILURE);

		}

		if (!semaphore_p(2))

		{

			exit(EXIT_FAILURE);

		}

		else

		{

			printf("PID: %d is defending\n",getpid() );

			pause_timeB = rand() % 2;
			printf("Pause time B: %d\n", pause_timeB );
			sleep(2);

		}

		if(!semaphore_v(2)){

			exit(EXIT_FAILURE);
		}
	}
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
	if(semctl(sem_id,1,SETVAL,sem_union) == -1){

		return(0);

	}
	if(semctl(sem_id,2,SETVAL,sem_union) == -1){

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

