#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include "semun.h"



static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(int sem_num);
static int semaphore_v(int sem_num);
static int playerTeamA(void);
static int playerTeamB(void);
static int sem_id;


int main(void) 
{

	int i;
	
	int n = 10; // Total players

	int pause_time;

	// Pipes
	int data_processed;
	int TeamA_pipe[2];
	int TeamB_pipe[2];
	const char some_data[] = "goal";
	char buffer[BUFSIZ + 1];

	pipe(TeamA_pipe);
	pipe(TeamB_pipe);

	memset(buffer, '\0', sizeof(buffer)); 

	// Timer
	time_t endmatch;
	int seconds = 300; //5 minutes


	int goalA; // Goals in goal A

	int goalB; // Goals in goal B

	printf("Start Match\n");

	// Create Semaphores

	srand((unsigned int)getpid());
	sem_id = semget((key_t)1234,3,0666|IPC_CREAT);

	if(!set_semvalue()) 
	{
		fprintf(stderr, "Failed to initialize semaphore\n");
		exit(EXIT_FAILURE);

	}
	else
	{
		goalA = 0;
		goalB = 0;
		printf("Semaphores created\n");
		sleep(2);
	}

	// Create Teams (forks)
	
	pid_t pids[10]; // Array of players
	
	for(i = 0; i < n; i++)
	{
		if((pids[i] = fork()) < 0) // if error in the forks 
		{
			perror("fork error");
			abort();
		}
		else if (pids[i] == 0) // if forks were created good
		{

			sleep(1);
			if(i % 2 == 0)
			{
				printf("Player ID: %d created; PPID: %d; team B\n",getpid(), getppid() );
				while(1)
				{
					if(playerTeamB())
					{
						//write pipe
						data_processed = write(TeamB_pipe[1], some_data, strlen(some_data));
					}
				}
			}
			else
			{
				printf("Player ID: %d created; PPID: %d; team A\n",getpid(), getppid() );
				while(1)
				{
					if(playerTeamA())
					{	
						//write pipe
						data_processed = write(TeamA_pipe[1], some_data, strlen(some_data));
					}
				}
			}
			//exit(0);
		}
	}

	endmatch = time(NULL) + seconds;
	
	printf("\nLet's start the game\n\n");	
	
	while(time(NULL) < endmatch)
	{
		//read pipe
		if(!fcntl( TeamA_pipe[0], F_SETFL, fcntl(TeamA_pipe[0], F_GETFL) | O_NONBLOCK))
		{
			if ((read(TeamA_pipe[0], buffer, BUFSIZ)) > 0)
			{
				goalB++;
				printf("\nPartial Score: Team A %d  --  %d Team B\n\n", goalB, goalA);
			}
			
		}
		
		if(!fcntl( TeamB_pipe[0], F_SETFL, fcntl(TeamB_pipe[0], F_GETFL) | O_NONBLOCK))
		{
			if ((read(TeamB_pipe[0], buffer, BUFSIZ)) > 0)
			{
				goalA++;
				printf("\nPartial Score: Team A %d  --  %d Team B\n\n", goalB, goalA);
			}
			
		}
		
	}

	// Kill all the childs 
	for(i = 0; i < 10; i++)
	{
		kill(pids[i], SIGKILL);
		printf("process %d terminated\n", pids[i]);	
	}
	
	//delete semaphores
	del_semvalue();
	printf("\nTime's up");	
	printf("\n%d - match finished\n", getpid());
	printf("\nFinal Score: Team A %d  --  %d Team B\n", goalB, goalA);

	exit(EXIT_SUCCESS);

	return 0;

}

/* Team A Players Code */

static int playerTeamA()
{
	int result; // 1 == score -- 0 == nothing
	
	int i;
	int pause_timeA;


//	while(1)
//	{

		/*Wait time*/
		
		//printf("Pause time A: %d\n", pause_timeA );
		
		pause_timeA = ((rand() % 16) + 5);
		sleep(pause_timeA);
		
		result = 0;

/*Attack MODE*/

		//printf("PID: %d trying to get the ball\n",getpid() );

		if (!semaphore_p(0))
		{
			/* ball is already taken */
			//printf("PID: %d unable to get the ball\n",getpid() );
			//exit(EXIT_FAILURE);
		}
		else
		{
			/* got the ball */
			printf("PID: %d got the ball\n", getpid());

			for (i = 0; i < 3; ++i)
			{
				/* Try to get the goal */
				//printf("PID: %d trying to get the goal\n",getpid() );

				if (!semaphore_p(2))
				{
					/* the goal is already taken */
					
					sleep(1); //sleep 1 second and try again
					
					//printf("PID: %d unable to get the goal\n",getpid() );
					//exit(EXIT_FAILURE);
				}
				else
				{
					/* got the goal and score*/
					
					result = 1;
					printf("\nGOOOOOLLLLL of PID: %d\nTeam A scores!!!!\n\n",getpid());
					i = 3;

					if(!semaphore_v(2))
					{
						/* release the goal */
						exit(EXIT_FAILURE);
					}
				}
			}
		

			if(!semaphore_v(0))
			{
				exit(EXIT_FAILURE);
			}
			else
			{
				/* release the ball */
				printf("PID: %d release the ball\n", getpid());
			}
			return result;
		}	
		
/*Defense MODE
	Get the own goal to prevent goals
*/
		
		if (!semaphore_p(1))
		{
			/*goal already taken*/
			//printf("PID: %d unable to get own goal\n",getpid() );
			//exit(EXIT_FAILURE);
		}
		else
		{	
			/*Defending the goal*/
			//printf("PID: %d is defending\n",getpid() );
			
			pause_timeA = (rand() % 11) + 5;
			//printf("Pause time A: %d\n", pause_timeA );
			sleep(pause_timeA);
			
			if(!semaphore_v(1))
			{
				exit(EXIT_FAILURE);
			}
		}
//	}
	return result;
}

/* Team B Players Code */

static int playerTeamB()
{	
	int result; // 1 == score -- 0 == nothing
	int i;
	int pause_timeB;

//	while(1)
//	{

	/*Wait time*/

		pause_timeB = ((rand() % 16) + 5);
		//printf("Pause time B: %d\n", pause_timeB );
		sleep(pause_timeB);
		result = 0;

/*Defense Mode*/
		
		if (!semaphore_p(2))
		{
			//printf("PID: %d unable to get the own goal\n",getpid() );
			//exit(EXIT_FAILURE);
		}
		else
		{
			//printf("PID: %d is defending\n",getpid() );
			pause_timeB = (rand() % 11) + 5;
			//printf("Pause time B: %d\n", pause_timeB );
			sleep(pause_timeB);

			if(!semaphore_v(2)){

				exit(EXIT_FAILURE);
			}
		}
		
		/*Attack MODE*/
		
		//printf("PID: %d trying to get the ball\n",getpid() );

		if (!semaphore_p(0))
		{
			//printf("PID: %d unable to get the ball\n",getpid() );
			//exit(EXIT_FAILURE);
		}
		else
		{
			printf("PID: %d got the ball\n", getpid());

			for (i = 0; i < 3; ++i)
			{
				//printf("PID: %d trying to get the goal\n",getpid() );

				if (!semaphore_p(1))
				{
					//pause_time = rand() % 1000;

					sleep(1);
					//printf("PID: %d unable to get the goal\n",getpid() );

					//exit(EXIT_FAILURE);
				}
				else
				{
					result = 1;
					printf("\nGOOOOOLLLLL of PID: %d\nTeam B scores!!!!\n\n", getpid());
					i = 3;

					if(!semaphore_v(1))
					{
						exit(EXIT_FAILURE);
					}
				}
			}

			if(!semaphore_v(0))
			{		
				exit(EXIT_FAILURE);
			}
			else
			{
				printf("PID: %d release the ball\n", getpid());
			}
		}

	return result;
	//}
}


/******************************************************************
*******************************************************************
Semaphore functions
*******************************************************************
******************************************************************/

static void del_semvalue(void)
{
	union semun sem_union;

	if(semctl(sem_id,0,IPC_RMID,sem_union) == -1)
	{
		fprintf(stderr, "Failed to delete semaphore\n");
	}
}

static int set_semvalue(void)
{
	union semun sem_union;
	sem_union.val = 1;
	
	if(semctl(sem_id,0,SETVAL,sem_union) == -1)
	{
		return(0);
	}
	if(semctl(sem_id,1,SETVAL,sem_union) == -1)
	{
		return(0);
	}
	if(semctl(sem_id,2,SETVAL,sem_union) == -1)
	{
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

	sem_b.sem_flg = SEM_UNDO | IPC_NOWAIT;

	if (semop(sem_id, &sem_b, 1) == -1)
	{

		//fprintf(stderr, "semaphore_p failed PID: %d\n", getpid());

		return(0);

	}

	return(1);
}



static int semaphore_v(int sem_num)
{
	struct sembuf sem_b;

	sem_b.sem_num = sem_num;

	sem_b.sem_op = 1; /*V()*/

	sem_b.sem_flg = SEM_UNDO | IPC_NOWAIT;

	if (semop(sem_id, &sem_b, 1) == -1)

	{

		//fprintf(stderr, "semaphore_v failed PID: %d\n", getpid());

		return(0);

	}

	return(1);
}



/******************************************************************
*******************************************************************

*******************************************************************
******************************************************************/

