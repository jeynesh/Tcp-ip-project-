
#include "net.h"
#include <stdbool.h>
#define BUFFER_SIZE 1024

main(){
	int sock;
        int nsock;
        int clilen;
	char buffering[BUFFER_SIZE+1];
	struct sockaddr_in serv_addr, cli_addr;
	int byte_received = 0;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	  perror("Server: socket() error\n");
	  exit(1);
	}

	printf("\n** File Repository Server **\n");

	/* Get Server's IP address */
	FILE *filepointer;
	char return_Data[64];
	char *token;
	char a[1000];
	filepointer = popen("/sbin/ifconfig eth0", "r");
	while (fgets(return_Data, 64, filepointer) != NULL)
	{
		/* store the string in return_Data into variable a */
		strcat(a, return_Data);
	}

	/* get the first token and walk through other tokens
	   until the IP address token */
	token = strtok(a, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " addr:");

	/* print the IP address token */
	printf("Server IP address: %s\n", token);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(token);
	serv_addr.sin_port = htons(SERV_TCP_PORT);

	pclose(filepointer);

	if(bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	perror("Server: bind() error\n");
	printf("\nWaiting for the connection............. [bind]\n");

	/* Get the user name */
	char *buff;
	buff=(char *)malloc(10*sizeof(char));
	buff=getlogin();

	/* set the 'server_file' path */
	char star[30];
	strcpy(star, "/home/");
	strcat(star, buff);
	strcat(star, "/server_file/");

	/* Check the path exist or not, if not, create one */
	struct stat s;
	if(stat(star, &s) == -1){
	mkdir(star, 0700); }

	listen(sock, 5);
	int p[2];
	if(pipe(p) == -1)
	{
		perror("Fail to create the pipe");
		exit(1);
	}

	for(;;){
	 clilen = sizeof(cli_addr);
	 nsock = accept(sock, (struct sockaddr *)&cli_addr, &clilen);


	char *clientaddress = inet_ntoa(cli_addr.sin_addr);
	char *individual_address;
	write(p[1], &clientaddress, sizeof(clientaddress));



	if(fork() == 0){
	  close(sock);
	  strcpy(buffering,"***** Welcome to File Repository server ***** \n**\nPress\n1.Download File\n2.upload File\n3.Create Directory\n4.Delete Directory**\n[type Exit to quit the server] : ");
	  send(nsock,  buffering, BUFFER_SIZE, 0);


	if(nsock > 0)

	  read(p[0],&individual_address,sizeof(individual_address));
	  printf("\nClient %s connected now.\n", individual_address);


	do{

	  recv(nsock, buffering, BUFFER_SIZE, 0);

	  if(!strcmp(buffering, "1"))
	{
		bzero( buffering, sizeof(buffering));
		strcat(buffering,"[Files List in Server Directory]\nIn order to carry out the Download process pls select a file from the Server Directory:\n\n");

		DIR *dir;
		struct dirent *ent;

		char directory_Name[30];
   		strcpy(directory_Name, "/home/");
    		strcat(directory_Name, buff);
    		strcat(directory_Name, "/server_file/");

		if ((dir = opendir (directory_Name)) != NULL) {

			//print all the files and directories within directory
 			while ((ent = readdir (dir)) != NULL) {

			strcat(buffering, ent->d_name);
			strcat(buffering, "\n");
 			}
 		 closedir (dir);
		send(nsock,  buffering, BUFFER_SIZE, 0);

		}else {

  		// could not open directory
 		 perror ("Directory does not exist.");
  		return EXIT_FAILURE;
		}

	bool exist = true;

	do{


	bzero(  buffering, sizeof(buffering));
	recv(nsock,  buffering, BUFFER_SIZE, 0);

	char file_name[30];
    	strcpy(file_name, "/home/");
    	strcat(file_name, buff);
    	strcat(file_name, "/server_file/");
    	strcat(file_name, buffering);


	FILE *filepointer = fopen(file_name, "r");
	if(filepointer==NULL)
        {
            strcpy(buffering,"No such file exist in the server.Please input correct file_name with extension.");
	    send(nsock,  buffering, BUFFER_SIZE, 0);
	    exist = false;

        }

	if(exist == true )
	{
	bzero(  buffering, sizeof(buffering));
	int nread = fread(buffering,1,256,filepointer);
	send(nsock, buffering, nread, 0);
	}

	bzero(  buffering, sizeof(buffering));
	strcpy(buffering,"Sucessfully Downloaded. [Exit to quit]");

	}while(exist == false);
	}


	 if(!strcmp(buffering, "2"))
	{

		bzero( buffering, sizeof(buffering));
		recv(nsock,  buffering, BUFFER_SIZE, 0);

		char file_name[30];
		strcpy(file_name, "/home/");
		strcat(file_name, buff);
		strcat(file_name, "/server_file/");
		strcat(file_name,  buffering);


		FILE *filepointer;
   	 	filepointer = fopen(file_name, "ab");
		bzero(  buffering, sizeof(buffering));
		byte_received = recv(nsock,  buffering, BUFFER_SIZE, 0);
		fwrite(buffering,1,byte_received,filepointer);
		fclose(filepointer);

		bzero(  buffering, sizeof(buffering));
		strcat(buffering,"Sucessfully uploaded. [Exit to quit]");
	}


	if(!strcmp(buffering, "3"))
	{
		bzero( buffering, sizeof(buffering));
		strcat(buffering,"Sucessfully Created file Directory. [Exit to quit]");
	}

	if(!strcmp(buffering, "4"))
	{
		bzero( buffering, sizeof(buffering));
		strcat(buffering,"Sucessfully Deleted file Directory. [Exit to quit]");
	}
	 send(nsock,  buffering, BUFFER_SIZE, 0);
	}while(strcmp(buffering, "Exit"));

	printf("\nClient %s disconnected now.\n", individual_address);
	exit(0);
	}
	close(nsock);
	}
	close(sock);
}
