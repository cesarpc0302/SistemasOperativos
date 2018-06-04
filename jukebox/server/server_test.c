#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


struct sockaddr_in c_addr;
char fname[100]; //file name

void* SendFileToClient(int *arg)
{

      int connfd=(int)*arg;
      
      printf("Connection accepted and id: %d\n",connfd);
      printf("Connected to Client: %s:%d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));

	int n;
  	char buffer[256];
	char path[] = "songs/";
      
   	bzero(buffer,256);
   	n = read(connfd,buffer,256);
   	if (n < 0) error("ERROR reading from socket");
	strcpy(fname,buffer);	


	if (strncmp(fname, "songs.txt", strlen("songs.txt")) == 0)
	{
	    printf("Creating songs txt\n");
	    system("ls songs/ > songs.txt");
	    sleep(1);
	}
	else
	{
	    strcat(path, fname);
	    strcpy(fname, path);
	}
	printf("fname: %s\n",fname);
        
	FILE *fp;
	fp = fopen(fname,"rb");

        if(fp == NULL)
        {
            printf("File open error");
            return 1;   
        }   
	if(fp) {
  		fseek(fp, 0 , SEEK_END);
		unsigned long lval = ftell(fp);
      		char buf [50];
      		sprintf (buf, "%lu" , lval );
		printf("file size: %s\n",buf);
  		fseek(fp, 0 , SEEK_SET);// needed for next read from beginning of file
sleep(1);
		write(connfd, buf, strlen(buf));

		read(connfd,buffer,256);

		

	}

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 1024 bytes */
            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
            //printf("Bytes read %d \n", nread);        

            /* If read was success, send data. */
            if(nread > 0)
            {
                //printf("Sending...");
                write(connfd, buff, nread);
            }
            if (nread < 1024)
            {
                if (feof(fp))
		{
                    printf("End of file\n");
		    printf("File transfer completed for id: %d\n",connfd);
		}
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
        }
printf("Closing Connection for id: %d\n",connfd);
close(connfd);
shutdown(connfd,SHUT_WR);
sleep(2);
}

int main(int argc, char *argv[])
{
    int connfd = 0,err;
    pthread_t tid; 
    struct sockaddr_in serv_addr;
    int listenfd = 0,ret;
    char sendBuff[1025];
    int numrv;
    size_t clen=0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0)
	{
	  printf("Error in socket creation\n");
	  exit(2);
	}

    printf("Socket retrieve success\n");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    ret=bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(ret<0)
    {
      printf("Error in bind\n");
      exit(2);
    }

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    while(1)
    {
        clen=sizeof(c_addr);
        printf("Waiting...\n");
        connfd = accept(listenfd, (struct sockaddr*)&c_addr,&clen);
        if(connfd<0)
        {
	  printf("Error in accept\n");
	  continue;	
	}
        err = pthread_create(&tid, NULL, &SendFileToClient, &connfd);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
   }
    close(connfd);
    return 0;
}
