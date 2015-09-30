# Tcp-ip-project-
Team Name: NT-2D

Student Name: JEYAGANESAN BELLBEM - 1092701059 
-developing server side

Student Name: VISHYLEAN RAJENDRAN - 1112702160
-developing client side

Development of File Repository System using Client-Server TCP/IP Model

File repository system is used for user file storage. User (Client) will connect with the file server using socket connection. The server should be able to handle multiple client connection requests at any particular time. Client can send file to the server, and server will automatically store the file in specific user’s directory. Client could also perform other tasks, e.g. create, copy, and delete directory. Client could also request for files to be downloaded from the server. During data transmission, the program should be clear of any interruption.

User manual:

Compile client.c and server.c (gcc filename.c -o filename)
Run both files on different terminal/pc (server.c must be run before client.c)
For example:

Terminal 1: ./server

Terminal 2: ./client server ip address
