#include <string>
#include <iostream>
#include <cstdio>
#include <stdlib.h>

#include <unistd.h>

#include "serial/serial.h"
#include "rhino_ros/rhino.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::memcpy;

// Holds the encoder positions for each arm.
//~ int positions[8];
boost::array<int,8> positions;
bool ready;
// Holds the target encoder positions for each arm.
//~ int destinations[8];

// Which COM port to connect to the Rhino on.  (1-4)
const char* PortStrings[]={"ERROR", "/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3"};

serial::Serial my_serial("/dev/ttyS0",9600,serial::Timeout::simpleTimeout(1000));

#ifdef RHINO_DEBUG
FILE *logfile=NULL;
#endif

/**
 * initialize rhino positions
 */

void rhino_init()
{
	#ifdef RHINO_DEBUG
	if(logfile==NULL)
	{
		logfile=fopen("rhino.log", "w");
	}
	else
	{
		fprintf(logfile, "\nRhino: init()\n\n");
	}
	#endif

	// initialize encoder positions for each arm.
	int i;
	for(i=0; i<8; i++)
	{
		positions[i]=0;
	}
	ready=true;
}

/**
 * Open communications with the Rhino controller.
 * return<0 indicates an error.
 */
int rhino_opencomm(int port)
{
	if(1>port || port>4)
	{
		printf("Invalid port COM%d\n", port);
		return 0;
	}
	printf("Connecting to Rhino on COM%d\n", port);

	/* Data format:
		RS-232 COM port
		9600 baud
		7-bit data
		even parity
		2 stop bits
	*/

	// This "open comm" routine was copied from the MinGW-users mailing list.
	// I suspect it originated on MSDN...
	// try to open comm port
	//hcom = CreateFileA(
						//PortStrings[port],
						//GENERIC_READ | GENERIC_WRITE,
						//0,                               /* com devs must be opeend w/ exclusive-access */
						//NULL,                            /* no security attribs */
						//OPEN_EXISTING,                   /* necessary for comm ports */
						//0,                               /* no overlapped i/o */
						//NULL                             /* must be null for comm */
						//);
	unsigned long baud = 9600;
	serial::bytesize_t bytesize= (serial::bytesize_t)7;
	serial::parity_t parity= (serial::parity_t)2;
	serial::stopbits_t stopbits= (serial::stopbits_t)2;
	serial::Timeout time(0,500,100,500,100);
	
	//~ my_serial(PortStrings[port], baud, serial::Timeout::simpleTimeout(1000),bytesize,parity,stopbits);
	my_serial.setPort(PortStrings[port]);
	my_serial.setBaudrate(baud);
	my_serial.setTimeout(time);
	my_serial.setBytesize(bytesize);
	my_serial.setParity(parity);
	my_serial.setStopbits(stopbits);
	
	
	cout << "Is the serial port open?";
	if(my_serial.isOpen())
		cout << " Yes." << endl;
	else
	{
		cout << " No." << endl;
		return 0;
	}
    cout<<"connected to serial port: " <<my_serial.getPort() <<endl;
	
	/*
	  0 ReadIntervalTimeout;
	  100 ReadTotalTimeoutMultiplier;
	  500 ReadTotalTimeoutConstant;
	  100 WriteTotalTimeoutMultiplier;
	  500 WriteTotalTimeoutConstant;
	*/
    
    
    my_serial.setTimeout(time);

   	#ifdef RHINO_DEBUG
		serial::Timeout out_time;
		out_time=my_serial.getTimeout();
		cout<<"Serial port time out set to:"<<endl;
		cout<<"		inter byte timeout: "<<out_time.inter_byte_timeout<<endl;
		cout<<"		read timeout const: "<<out_time.read_timeout_constant<<endl;
		cout<<"		read timeout multiplier: "<<out_time.read_timeout_multiplier<<endl;
		cout<<"		write timeout constant: "<<out_time.write_timeout_constant<<endl;
		cout<<"		write timeout multiplier: "<<out_time.write_timeout_multiplier<<endl;
    #endif
    
    //reset the controller and the input buffer
    my_serial.flush();
    cout<<"in out buffer flushed"<<endl;
    
    // "warm up" connection
    rhino_inquire('A');
    rhino_inquire('B');
	if(rhino_inquire('C')<0)
	{
		printf("Connection not established.\n");
		return -1;
	}
	else
	{
		//cout<<rhino_inquire('C')<<endl;
		//rhino_home('A'); // Auto-synch the gripper commands
		printf("Connection established.\n");
	}
	return 0;
}

/**
 * Retrieve the "error register" from the controller.
 * return - <0 indicates an error, otherwise return the error count.
 */
int rhino_inquire(char motor)
{
	if('A'>motor || motor>'H')
	{
		printf("Illegal motor: %c\n", motor);
		return -1;
	}

	int error;
	error=-1; // default to having an error occur.

	// Send the request to the motor
	uint8_t buf[10]; // avoid buffer overflows - much bigger than it needs to be.
	
	
	int sendsize = snprintf((char *)buf,10, "%c?\r", motor);

	
	size_t written=	my_serial.write(buf,sendsize);
	

	if(written)
	{
		if(written<3)
		{
			printf("Timeout before writing data.\n");
			return -1;
		}
	}
	else
	{
		printf("Error writing to the Rhino.\n");
		return -1;
	}

	//get the error register for the motor.
	// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/base/readfile.asp
	try{
		size_t read=my_serial.read(buf,1);
	}
	catch(...){
		printf("Error reading the motor position.\n");
		return -1;
	}
	if(read==0){
			printf("Timeout before reading data.\n");
			return -1;
	}
	else{
		error=((int)buf[0])-32; // Fix the communications offset
	}
	return error;
}

int rhino_closecomm()
{
	my_serial.close();
	return 0;
}


/**
 * Move the motor to the desired postion.
 * This function blocks until the move is complete.
 *
 * motor - 'A' through 'H'
 * position - encoder count from the origin
 * return!=0 indicates an error
 */
int rhino_move(char motor, int destination)
{
	// QUICK FIX - until I can sort things out...
	// If we can, use rhino_mmove() to avoid rhino_move() vs rhino_mmove() errors
	if('A'<motor && motor<'G')
	{
		int i;
		boost::array<int,JOINTS> destinations;
		for(i=0; i<JOINTS; i++)
		{
			destinations[i]=positions[i+1];
		}
		destinations[motor-'B']=destination;

		return rhino_ammove(destinations);
	}
	ready=false;
	// The polarity of this move
	int sign;
	// The move command to send to the Rhino
	char command;
	// How many encoder clicks we need to move
	int toMove;
	// Count how many cycles occur without any motion
	int noMove;
	// Flag when the motor has completely stalled
//	int stalled;
	// Command buffer
	uint8_t buffer[10];
	// How many bytes are actually written to the Rhino
	size_t written;
	// The latest status of the Rhino error register
	int error;
	// What the error register should be after a move command and before motion has occurred.
	int oldError;
	// How far to move this iteration
	int increment;
	// How far the motor moved since the last update
	int moved;
	int sendsize;

	// validate the motor
	if('A'>motor || motor>'H')
	{
		printf("Illegal motor: %c\n", motor);
		ready=true;
		return -1;
	}

	// Normalize the direction
	toMove=destination-positions[motor-'A'];
	if(toMove<0)
	{
		sign=-1;
		command=REVERSE;
		toMove=-toMove;
	}
	else
	{
		sign=1;
		command=FORWARD;
	}

	#ifdef RHINO_DEBUG
	printf("move(%c, %d), position: %d, toMove: %d, sign: %d, command: %c\n",
		motor, destination, positions[motor-'A'], toMove, sign, command);
	#endif

	error=rhino_inquire(motor);
	oldError=error;
	moved=0;
	noMove=0;
	while(destination!=positions[motor-'A'] && noMove<STALL)
	{
		if(error<0)
		{
			#ifdef RHINO_DEBUG
			printf("Wait; error=%d < 0\n", error);
			#endif

			noMove++; // An invalid value is generally caused by a stall...

			// Read the error register
			error=rhino_inquire(motor);
			continue; // try again
		}

		// move towards the destination
		increment=95-error;

		if(increment>toMove)
		{
			increment=toMove;
		}

		toMove-=increment;

		// send the move instruction
		sendsize = snprintf((char *)buffer,10,  "%c%c%d\r", motor, command, increment);
		written=my_serial.write(buffer,sendsize);
		if(written)
		{
			if(written<2)
			{
				printf("Timeout before writing data.\n");
				ready=true;
				return -1;
			}
		}
		else
		{
			printf("Error writing to the Rhino.\n");
			ready=true;
			return -1;
		}

		// update the position
		moved=oldError-error;
		positions[motor-'A']+=sign*moved;
		oldError=error+increment;

		// Stall detection
		if(moved==0)
		{
			noMove++;
		}
		else
		{
			noMove=0;
		}
		#ifdef RHINO_DEBUG
		printf("            - position: %d, toMove: %d, oldError: %d, error: %d, moved: %d, increment: %d\n",
			positions[motor-'A'], toMove, oldError, error, moved, increment);
		#endif

		// Read the error register
		error=rhino_inquire(motor);
	}

	// Stall correction
	if(noMove>=STALL)
	{
		#ifdef RHINO_DEBUG
		printf( "Clearing a stall.\n");
		#endif

		// back off to clear the stall
		// reverse the direction of motion
		if(sign>0)
		{
			sign=-1;
			command=REVERSE;
		}
		else
		{
			sign=1;
			command=FORWARD;
		}

		// use the last known good error value
		increment=oldError;
		/*
		// Find out how far to go.
		increment=rhino_inquire(motor);
		if(increment<0)
		{
			// use the last known good error value
			increment=oldError;
		}
		*/

		#ifdef RHINO_DEBUG
		printf("stall reversal increment: %d\n", increment);
		#endif

		// send the move instruction
		sendsize = snprintf((char *)buffer,10,  "%c%c%d\r", motor, command, increment);
		written=my_serial.write(buffer,sendsize);
		if(written)
		{
			if(written<2)
			{
				printf("Timeout before writing data.\n");
				ready=true;
				return -1;
			}
		}
		else
		{
			printf("Error writing to the Rhino.\n");
			ready=true;
			return -1;
		}

		// Assume the move succeeds.
		//positions[motor-'A']+=sign*increment;
		// Wait for the move to succeed.
		noMove=0;
		int errors, temp;
		errors=1;
		while(noMove<STALL && error)
		{
			temp=rhino_inquire(motor);
			if(error && error==temp)
			{
				noMove++;
			}
			else
			{
				error=temp;
				noMove=0;
			}

			if(noMove>=STALL)
			{
				printf("Could not clear the stall on motor %c.\n", motor);
			}
		}
	}

	// return the error - will be 0 if no stall occurs
	ready=true;
	return destination-positions[motor-'A'];
}

/**
 * Move all the arm joints at the same time.
 * Parameters: destination position for each joint
 * Return!=0 if an error occurs.
 */
int rhino_mmove(int destB, int destC, int destD, int destE, int destF)
{
	//~ int destination[JOINTS];
	boost::array<int,JOINTS> destination;

	destination[0]=destB;
	destination[1]=destC;
	destination[2]=destD;
	destination[3]=destE;
	destination[4]=destF;

	return rhino_ammove(destination);
}


int rhino_ammove(boost::array<int,JOINTS> destination)
{ 	ready=false;
	// Remember that positions[] by 'A' while destination[] is offset by 'B'...

	// The polarity of this move
	int sign[JOINTS];
	// The move command to send to the Rhino
	char command[JOINTS];
	// How many encoder clicks we need to move
	int toMove[JOINTS];
	// Count how many cycles occur without any motion
	int noMove[JOINTS];
	// Flag when the motor has completely stalled
	int stalled;
	// Command buffer
	uint8_t buffer[10];
	// How many bytes are actually written to the Rhino
	size_t written;
	// The latest status of the Rhino error register
	int error[JOINTS];
	// What the error register should be after a move command and before motion has occurred.
	int oldError[JOINTS];
	// How far to move this iteration
	int increment;
	// How far the motor moved since the last update
	int moved[JOINTS];

	int motor;
	
	int sendsize;
	// Normalize the direction
	for(motor=0; motor<JOINTS; motor++)
	{
		toMove[motor]=destination[motor]-positions[motor+1];
		if(toMove[motor]<0)
		{
			sign[motor]=-1;
			command[motor]=REVERSE;
			toMove[motor]=-toMove[motor];
		}
		else
		{
			sign[motor]=1;
			command[motor]=FORWARD;
		}
	}

	// Initialize the variables.
	for(motor=0; motor<JOINTS; motor++)
	{
		error[motor]=rhino_inquire((char)(motor+'B'));
		oldError[motor]=error[motor];
		moved[motor]=0;
		noMove[motor]=0;
	}
	stalled=false;

	#ifdef RHINO_DEBUG
		cout<<"\nrhino_mmove()\ndestinations - B: "<<destination[0]<< "C: "<<destination[1]<< "D: "<<destination[2]<< "E: "<<destination[3]<< "F: "<<destination[4]<<endl;
	#endif
	
	// Drive the motors to their positions.
	while(!stalled &&
		(   destination['B'-'B']!=positions['B'-'A'] || destination['C'-'B']!=positions['C'-'A']
		 || destination['D'-'B']!=positions['D'-'A'] || destination['E'-'B']!=positions['E'-'A']
		 || destination['F'-'B']!=positions['F'-'A'])
		)
	{
		#ifdef RHINO_DEBUG
			cout<<" positions - - B: "<<positions['B'-'A']<< "C: "<<positions['C'-'A']<< "D: "<<positions['D'-'A']<< "E: "<<positions['E'-'A']<< "F: "<<positions['F'-'A']<<endl;
		#endif

		for(motor=0; motor<JOINTS; motor++)
		{
			if(noMove[motor]>STALL)
			{
				// ignore the stall if we are essentially there
				if(error[motor]<10 && abs(destination[motor]-positions[motor+1])<10)
				{
					// move the destination to match the position
					destination[motor]=positions[motor+1];
				}
				else
				{
					printf("Error: motor %c stalled.\n", (char)(motor+'B'));
					stalled=true;
				}
			}

			if(error[motor]<0)
			{
				#ifdef RHINO_DEBUG
				cout<< "Wait; error= "<< error[motor]<<"<0 on"<<(char)(motor+'B')<< endl;
				#endif

				noMove[motor]+=6; // An invalid value is generally caused by a stall...

				// Read the error register
				error[motor]=rhino_inquire((char)(motor+'B'));
				continue; // try again
			}

			// move towards the destination
			increment=95-error[motor];

			if(increment>toMove[motor])
			{
				increment=toMove[motor];
			}

			toMove[motor]-=increment;

			// send the move instruction
			//~ sprintf_s(buffer,sizeof(buffer), "%c%c%d\r", (char)(motor+'B'), command[motor], increment);

			sendsize = snprintf((char *)buffer,10,  "%c%c%d\r", (char)(motor+'B'), command[motor], increment);
			//~ cout<<(char *)buffer<<endl;
			written=my_serial.write(buffer,sendsize);
			if(written)
			{
				if(written<2)
				{
					cout<<written<<endl;
					printf("Timeout before writing data.\n");
					ready=true;
					return -1;
				}
			}
			else
			{
				printf("Error writing to the Rhino.\n");
				ready=true;
				return -1;
			}

			// update the position
			moved[motor]=oldError[motor]-error[motor];
			positions[motor+1]+=sign[motor]*moved[motor];
			oldError[motor]=error[motor]+increment;

			// Stall detection - catch both no motion and negative motion.
			if(moved[motor]<=0 && destination[motor]!=positions[motor+1])
			{
				#ifdef RHINO_DEBUG
				//~ fprintf(logfile, "noMove: '%c', position: %d, toMove: %d, sign: %d, command: %c, ",
					//~ (char)(motor+'B'), positions[motor], toMove[motor], sign[motor], command[motor]);
				cout<<"noMove: "<<(char)(motor+'B')<< ", position: "<<positions[motor]<< ", toMove: "<<toMove[motor] << ", sign: "<<sign[motor]<<", command: "<<command[motor]<<endl;
				//~ fprintf(logfile, "error: %d, oldError: %d, increment: %d\n",
					//~ error[motor], oldError[motor], increment);
				cout<<"error: "<<error[motor]<<", oldError: "<<oldError[motor]<<", increment: "<<increment<<endl;
				#endif
				noMove[motor]++;
			}
			else
			{
				noMove[motor]=0;
			}

			// Read the error register
			error[motor]=rhino_inquire((char)(motor+'B'));
		}
	}

	// Clean up any "small epsilons" that weren't accounted for
	if(!stalled)
	{
		for(motor=0; motor<JOINTS; motor++)
		{
			positions[motor+1]+=sign[motor]*error[motor];
		}
		ready=true;
		return 0;
	}

	// Stall correction

	// Return the motors to the last pre-stall position.
	if(stalled)
	{
		printf("Clearing the stall.\n");

		for(motor=0; motor<JOINTS; motor++)
		{
			// back off to clear the stall
			// reverse the direction of motion
			if(sign[motor]>0)
			{
				sign[motor]=-1;
				command[motor]=REVERSE;
			}
			else
			{
				sign[motor]=1;
				command[motor]=FORWARD;
			}

			// use the last known good error value
			increment=oldError[motor];

			#ifdef RHINO_DEBUG
			cout<<"stall reversal increment: "<<increment<<endl;
			#endif

			// send the move instruction
			sendsize = snprintf((char *)buffer,10,  "%c%c%d\r", (char)(motor+'B'), command[motor], increment);
			written=my_serial.write(buffer,sendsize);
			if(written)
			{
				if(written<5)
				{
					printf("Timeout before writing data.\n");
					ready=true;
					return -1;
				}
			}
			else
			{
				printf("Error writing to the Rhino.\n");
				ready=true;
				return -1;
			}

			// reset the stall counter
			noMove[motor]=0;
		}

		// Wait for the move to succeed.
		stalled=false;
		int errors, temp;
		errors=1;
		while(!stalled && errors)
		{
			errors=0;
			for(motor=0; motor<JOINTS; motor++)
			{
				temp=rhino_inquire((char)(motor+'B'));
				if(temp<0)
				{
					noMove[motor]+=6;
				}
				else if(error[motor] && error[motor]==temp)
				{
					noMove[motor]++;
				}
				else
				{
					error[motor]=temp;
					noMove[motor]=0;
				}

				if(noMove[motor]>STALL)
				{
					printf("Could not clear the stall on motor %c.\n", (char)(motor+'B'));
					stalled=true;
				}

				errors+=error[motor];
			}
		}
	}

	// return the error - will be 0 if no stall occurs
	ready=true;
	return -1*stalled;
}

/**
 * Close the gripper
 * return the width of the object in clicks
 */
int rhino_grip()
{	ready=false;
	rhino_move('A', CLOSE_A); // the "closed" position
	ready=true;
	return CLOSE_A-positions[0];
}

/**
 * Open the gripper
 */
void rhino_ungrip()
{
	ready=false;
	rhino_move('A', 0);
	ready=true;
}

/**
 * Return the encoder position of the desired motor.
 * An invalid motor returns 0.
 */
int rhino_position(char motor)
{
	if('A'>motor || motor>'H')
	{
		printf("Illegal motor: %c\n", motor);
		return 0;
	}

	return positions[motor-'A'];
}
boost::array<int,8> rhino_positions(){
	return positions;
}
/* DOES NOT HAVE A WORKING rhino_home(char motor)
 * Do a software "hard home" routine.
 * Resets the counters to 0.
 * return<0 indicates an error.
 */
int rhino_hardhome()
{
	// Don't drag things around with us.
	rhino_ungrip();

	// The sequence used by the Rhino people
	rhino_home('E');
	rhino_home('D');
	rhino_home('B');
	rhino_home('C');
	rhino_home('F');
	rhino_home('A');

	rhino_init(); // reset the position counters. - don't reset the com line.
	return 0;
}

int rhino_softhome()
{
	int i;
	boost::array<int,JOINTS> destinations;

	for(i=0; i<JOINTS; i++)
	{
		destinations[i]=0;
	}

	rhino_ammove(destinations);

	return 0;
}

/* NOT FULLY IMPLEMENTED
 * Run a "hard home" routine for the specified motor only.
 */
int rhino_home(char motor)
{
	// Try moving the specified motor to the soft home position first (to improve switch finding)
	rhino_move(motor, 0);

	int found, delta;
	switch(motor)
	{
		case 'A':
			// Close the gripper until it stalls; then open it 50 clicks.
			rhino_move('A', 200); // The range of travel is only 70 or 80 clicks; this has to stall.
			positions[0]=0;
			rhino_move('A', -CLOSE_A); // open the gripper its default distance
			positions[0]=0;
			break;
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			// Rotate one way until the switch is found, or a stall occurs.
			found=false;
			delta=20;
			if(rhino_switch(motor)>0)
			{
				found=true;
			}
			while(!found)
			{
				// move strategy
				// go + for a long time or until a stall occurs.
				// then reverse direction.
				if(rhino_move(motor, delta))
				{
					delta=-delta;
				}
				positions[motor-'A']=0; // reset the position tracker

				if(rhino_switch(motor)>0)
				{
					found=true;
				}
			}

			// Center the switch

			// Reset the software position
			positions[motor-'A']=0;
			break;

		// In our setup, G and H don't have position sensors.
		case 'G':
		case 'H':
			break;

		// Must be invalid
		default:
			printf("Invalid motor: %c\n", motor);
			return -1;
	}

	return 0;
}

/**
 * Reads the microswitch for each motor.
 * Returns 1 (TRUE) if it is closed, 0 (FALSE) if it is not.
 * -1 indicates an error occurred.
 */
int rhino_switch(char motor)
{
	int sendsize=0;
	
	if('A'>motor || motor>'H')
	{
		printf("Illegal motor: %c\n", motor);
		return -1;
	}

	int error;
	error=-1; // default to having an error occur.

	uint8_t buf[10];
	
	if(motor<'C') // A and B
	{
		sendsize=snprintf((char *)buf,10, "J\r");
	}
	else // C,D,E,F,G,H
	{
		sendsize=snprintf((char *)buf,10, "I\r");
	}

	// Send the request
	size_t written=	my_serial.write(buf,sendsize);
	if(written)
	{
		if(written<2)
		{
			printf("Timeout before writing data in switch.\n");
			return -1;
		}
	}
	else
	{
		printf("Error writing to the Rhino in switch.\n");
		return -1;
	}

	//get the error register for the motor.
	// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/base/readfile.asp
	size_t read=my_serial.read(buf,1);
	if(read)
	{
		if(read==0)
		{
			printf("Timeout before reading data.\n");
			return -1;
		}
		error=((int)buf[0])-32; // Fix the communications offset
	}
	else
	{
		printf("Error reading the motor position.\n");
		return -1;
	}

	// Decode the switch information
	// An open switch is a '1'.
	if(motor=='A')
	{
		return !(error&0x10);
	}
	else if(motor=='B')
	{
		return !(error&0x20);
	}
	else if(motor=='C')
	{
		return !(error&0x01);
	}
	else if(motor=='D')
	{
		return !(error&0x02);
	}
	else if(motor=='E')
	{
		return !(error&0x04);
	}
	else if(motor=='F')
	{
		return !(error&0x08);
	}
	else if(motor=='G')
	{
		return !(error&0x10);
	}
	else if(motor=='H')
	{
		return !(error&0x20);
	}

	return error;
}

bool rhino_inquires(){
	//~ int motor;
	//~ boost::array<int,5> error;
	//~ int ready=1;
	//~ for(motor=0; motor<JOINTS; motor++)
	//~ {
		//~ error[motor]=rhino_inquire((char)(motor+'B'));
		//~ if(error[motor]!=0){
			//~ ready=0;
		//~ }
		//~ cout<<(char)(motor+'B')<<": "<<error[motor]<<" ; ";
	//~ }
	//~ cout<<endl;
	
	return ready;
}

#ifdef RHINO_MAIN
int main(int argc, char **argv) {
	rhino_init();
	rhino_opencomm(1);
	//~ int des[5]={30,30,30,30,30};
	//~ rhino_ammove(des);
	//~ rhino_move('B', 30);
	//~ rhino_hardhome(); // does not work, and not yet fully implemented....

}
#endif

