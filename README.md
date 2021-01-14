# Party_Project


To Run: Download to a linux system: party.c, and compile

Run the following command ONLY once to turn our compile script into and executable.: 
chmod +x compile

To complile the program run the following command:  ./compile

To run the program, run the following command and verify the number of students is always a multiple of 4, and the number of taxis is 4x the number of students:
  ./party –s 12 –t 3 –m 10
  
  This program does the following correctly:
	Creates taxi and student threads.
	Allow student threads to randomly use the taxi mutex/resource. As in they can leave the party out of the order they entered in. 
	Allow taxi threads to randomly use the curb mutex/resource. As in they can drive to the curve out of the order they entered in.
	Taxi is able to pickup 4 students when they are ready to leave.
	Taxi's are able to access curb resources one at a time.
	

This programs runs with these exceptions:
	In the program an array is filled with the student thread id's that are waiting on the curb for the taxi. A bug is causing  that arry (taxiPickUpQueue[]) to fill incorrectly with the wrong student thread IDs. Because of this the taxi thread is is not able to correctly pickup students based on thier IDs.
	Another issue is if the taxi thread checks if there are students to be pick up, and there are none, the taxi thread will not check again.
