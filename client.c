
#include "net.h"
#define BUFFER_SIZE 1024

main(int argc, char *argv[]){
int sock;
char buffering[BUFFER_SIZE+1];
int byte_received = 0;
struct sockaddr_in serv_addr;
char namecreated[20];
char name_deleted[20];
static struct sigaction act;

void catchin(int);

act.sa_handler = catchin;
sigfillset(&(act.sa_mask));

sigaction(SIGINT, &act, (void *) 0);


if(argc <= 1){
printf("How to use : %s remoteIPaddress [example: ./client 127.0.0.1]\n", argv[0]); exit(1); }

bzero( (char*) &serv_addr, sizeof(serv_addr) );
serv_addr.sin_family = AF_INET ;
serv_addr.sin_port = htons (SERV_TCP_PORT);
inet_pton (AF_INET, argv[1], &serv_addr.sin_addr);

if( (sock = socket(AF_INET, SOCK_STREAM, 0) ) < 0) {
perror("Client: socket() error \n");
exit(1); }

if(connect (sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0 ) {
perror("Client: connect() error\n");
exit(1);}

/* Get the user name */
	char *buf;
	buf=(char *)malloc(10*sizeof(char));
	buf=getlogin();

	/* set the 'client_file' path */
	char star[30];
	strcpy(star, "/home/");
	strcat(star, buf);
	strcat(star, "/client_file/");

	/* Check the path exist or not, if not, create one */
	struct stat s;
	if(stat(star, &s) == -1){
	mkdir(star, 0700); }

do{
bzero( buffering, sizeof(buffering));
recv(sock, buffering, BUFFER_SIZE, 0);
printf("\n%s\n", buffering);
gets(buffering);
send(sock,buffering, BUFFER_SIZE, 0);


if(!strcmp(buffering, "1"))
{
	bzero( buffering, sizeof(buffering));
	recv(sock, buffering, BUFFER_SIZE, 0);
	printf("\n%s\n", buffering);
	gets(buffering);
	send(sock,buffering, BUFFER_SIZE, 0);

   	char file_name[30];
    	strcpy(file_name, "/home/");
    	strcat(file_name, buf);
    	strcat(file_name, "/client_file/");
    	strcat(file_name, buffering);

    	FILE *filepointer;
    	filepointer = fopen(file_name, "ab");

		if(NULL == filepointer)
    	{
        	printf("Error opening file");

    	}

    	bzero( buffering, sizeof(buffering));

    	byte_received = recv(sock, buffering, BUFFER_SIZE, 0);
    	fwrite(buffering,1,byte_received,filepointer);

}


else if(!strcmp(buffering, "2"))
{


	DIR *dir;
	struct dirent *ent;

	char directory_Name[30];
   	strcpy(directory_Name, "/home/");
    	strcat(directory_Name, buf);
    	strcat(directory_Name, "/client_file/");

	if ((dir = opendir (directory_Name)) != NULL) {

		printf("\n[File List in Client Directory]\n");
  		// print all the files and directories within directory
  		while ((ent = readdir (dir)) != NULL) {

		printf("%s\n", ent->d_name);    }

 		closedir (dir);
	}

	printf("\nSelect file_name from the list above with extension\n");

	bzero( buffering, sizeof(buffering));
	gets(buffering);
	send(sock,buffering, BUFFER_SIZE, 0);

	char file_name[30];
	strcpy(file_name, "/home/");
	strcat(file_name, buf);
	strcat(file_name, "/client_file/");

	strcat(file_name, buffering);

	FILE *filepointer;
    	filepointer = fopen(file_name, "r");

	bzero( buffering, sizeof(buffering));
	int nread = fread(buffering,1,256,filepointer);
	send(sock, buffering, nread, 0);
}


else if(!strcmp(buffering, "3"))
{
	printf("Input directory name to be created: ");
	scanf("%s", namecreated);

	/* set the path/name of the directory that want to create */
	char createDirectory[30];
	strcpy(createDirectory, "/home/");
	strcat(createDirectory, buf);
	strcat(createDirectory, "/");
	strcat(createDirectory, namecreated);

	/* Check the path exist or not, if not, create one */
	struct stat s;
	if(stat(createDirectory, &s) == -1){
	mkdir(createDirectory, 0700); }
}


else if(!strcmp(buffering, "4"))
{
	printf("Input directory name to be deleted: ");
	scanf("%s", name_deleted);

	/* set the path of the directory that want to delete */
	char deleteDirectory[30];
	strcpy(deleteDirectory, "/home/");
	strcat(deleteDirectory, buf);
	strcat(deleteDirectory, "/");
	strcat(deleteDirectory, name_deleted);

	/* select all the files inside the directory that want to delete */
	char selectSubDirectory[50];
	strcpy(selectSubDirectory, "exec rm -r ");
	strcat(selectSubDirectory, "/home/");
	strcat(selectSubDirectory, buf);
	strcat(selectSubDirectory, "/");
	strcat(selectSubDirectory, name_deleted);
	strcat(selectSubDirectory, "/*");

	/* Check if the path exist or not, if it exist, delete it */
	struct stat s;
	if(stat(deleteDirectory, &s) != -1){
	system(selectSubDirectory);
	rmdir(deleteDirectory); }
}

}while (strcmp(buffering, "Exit"));
close(sock);
}

void catchin(int signo){
	printf("\n[ Failed signal has been successfull ignored.]\n");
}
