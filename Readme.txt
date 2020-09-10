How to Run :-

1. Run make file by

	$ make all

2. run server.cpp by

	$ ./server port_number

3. run client.cpp by 

	$ ./client server_address port_number


How It works :-

TCP PHASE :-
1. server will be waiting for a TCP connection from the client.
2. client will connect to the server using server’s TCP port already known to the client.
3. client sends a Request Message (Type 1 message) to the server via TCP port to request a UDP port from server for future communication.
4. server obtains a UDP port number and sends this port number back to the client as a Response Message (Type 2 Message) over the TCP connection.
5. TCP connection on both the server and client will be closed


UDP Phase (After recieving UDP port from Server) 

1. client transmits a short Data Message (Type 3 message) over the UDP port. 
2. The server will display the received Data Message and sends a Data Response (type 4 message) to indicate the successful reception. 
3. After this data transfer phase, both sides close their UDP sockets.
4. and server will wait for another connection


From - Sanil Upadhyay(194101043)
	   Rajat Shukla(194101039)