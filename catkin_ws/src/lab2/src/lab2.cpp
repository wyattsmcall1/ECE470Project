#include "ros/ros.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>

/** for custom msg and srv
 */
#include <rhino_ros/command.h>
#include <rhino_ros/positions.h>

/** basic setting for using something like cout instead of std::cout everytime.
 */
using std::cout;
using std::endl;
using std::cin;
using std::string;

/** uncomment the line below to enable cout for debug purposes.
 */
//#define DEBUG

boost::array<int,5> positions; 	//global position variable, used to transfer
				//msg from callback function to where ever it is
				//needed.

/**	void position_callback(const rhino_ros::positions::ConstPtr& msg):
 *  Callback function for subscriber subscribed to topic "rhino/position".
 * Every time a msg is published unber the topic "rhino/position", it will
 * be put into a buffer, when the ros::spinOnce() is involked, the msg will
 * be transfered to whoever is subscribed to that topic under the same msg type
 * <rhino_ros::positions>, and the callback funtion associate with each subscriber
 * will be invoked and be processed.
 */

int grip;
void position_callback(const rhino_ros::positions::ConstPtr& msg){
	// copy msg to a gloabal variable, more info on std::copy(), use google.
	std::copy(msg->position.begin(),msg->position.begin()+5,positions.begin());
	grip=msg->grip;
}

int D[7][3] = 
{
	{-600,-470,-290}, 	//b grab
	{-550,-430,-270}, 	//m grab
	{-490,-370,-230}, 	//t grab
	{0,0,0}, 	  	//raise
	{-620,-480,-310},	//b drop
	{-515,-390,-240},	//m drop
	{-445,-330,-200}  	//t drop
};

int E[7][3] = {
	{740,820,956},		//b grab
	{670,760,890},		//m grab
	{600,710,850},		//t grab
	{350,540,730},		//raise
	{730,810,930},		//b drop
	{640,740,870},		//m drop
	{570,680,840}		//t drop
};

int choice1, choice2, wait, source, dest, intermed, i=0;

rhino_ros::command move(int level,int dest,int grab)
{
	rhino_ros::command msg;
        msg.grip=grab;                             // Motor A Encoder Destination
        msg.destination[0]=0;                      // Motor B Encoder Destination
       	msg.destination[1]=0;                      // Motor C Encoder Destination
        msg.destination[2]=D[level-1][dest-1];     // Motor D Encoder Destination
        msg.destination[3]=E[level-1][dest-1];     // Motor E Encoder Destination
        msg.destination[4]=0;                      // Motor F Encoder Destination    

	return msg;
}


int main(int argc, char **argv)
{
    	//initialization & variable definition
	ros::init(argc, argv, "lab2");	//initialzation of ros required for each Node.
	ros::NodeHandle nh;				//handler for this node.
    	//define a publisher to publish command to topic "rhino/command" with msg type <rhino_ros::command>
	ros::Publisher lab_pub = nh.advertise<rhino_ros::command>("rhino/command",1000);
   	//define a subscriber to subscribe to topic "rhino/position" with msg type <rhino_ros::positioins>
	ros::Subscriber lab_sub= nh.subscribe<rhino_ros::positions>("rhino/position",100,position_callback);
    	//detail of topic can be obtained by entering "rostopic info /rhino/command"
   	//and detail of msg can be obtained by entering "rosmsg show rhino_ros/command" in terminal
	rhino_ros::command msg;
	
	/** lines below simply test if the any subscriber have 
	 * latch on to the publisher
	 */
	ros::Rate poll_rate(100);
	while(lab_pub.getNumSubscribers()==0)
		poll_rate.sleep();
	
	/** setting msg.softhome=1 commands rhino to soft home position
	 *  however this does not reset msg.destination[] to zero, if
	 *  they are non-zero, it should be set to zero manually.
	 */

	msg.softhome = 1;
	while(!ros::ok()){};	//check if ros is ready for operation
	lab_pub.publish(msg);	//push msg to publish buffer
	ros::spinOnce();	//let ros process all publish and subscribe once.
	msg.softhome = 0;
	
	//=========================Main Loop=============================
	//===== modify this code for your own functions and inputs=====
	//===============================================================
  	wait=true;
	while(wait)
	{
		cout<<"Enter a source number between 1 and 3. "<<endl;
		cin>>choice1;
		
		// validate the input
		if(1>choice1 || choice1>3)
		{
			cout<<choice1<<" is a bad number - try again.\n"<<endl;
			return 0;
		}

		else
		{
			// quit waiting for good input
			wait=false;
		}
	}
    
    	wait=true;
    	while(wait)
    	{
        	cout<<"Enter a destination number between 1 and 3. "<<endl;
       	 	cin>>choice2;
        
        	// validate the input
        	if(1>choice2 || choice2>3)
        	{
            		cout<<choice2<<" is a bad number - try again.\n"<<endl;
            		return 0;
        	}

        	else
        	{
           		// quit waiting for good input
            		wait=false;
        	}
   	 }

    	source=choice1;
    	dest=choice2;
    	if(source==1 && dest==2){
      	 	intermed=3;
    	}
		else if(source==1 && dest==3){
        	intermed=2;
    	}
    	else if(source==2 && dest==3){
        	intermed=1;
    	}
		else if(source==2 && dest==1){
        	intermed=3;
    	}
    	else if(source==3 && dest==1){
        	intermed=2;
    	}
    	else if(source==3 && dest==2){
        	intermed=3;
    	}
    	else{
			intermed = 0;
		}
		

	ros::Rate loop_rate(4); // high loop rate may result in undesired behaviors.
	while(ros::ok())
	{
		/*      
		move(3, source, 1, dest);
        	move(2, source, 1, intermed);
       
       	 	move(1, dest, 2, intermed);
       		move(1, source, 1, dest);
        
       	 	move(2, intermed, 1, source);
       	 	move(1, intermed, 2, dest);
        
      	  	move(1, source, 3, dest);
 		*/
		switch(i){
			case 0:
				msg=move(3, source, 1);
				break;
			case 1:
				msg=move(4, source, 1);
				break;
			case 2:
				msg=move(4, dest, 1);
				break;
			//step
			case 3:
				msg=move(2, dest, 1);
				break;
			case 4:
				msg=move(1, dest, 0);
				break;
			case 5:
				msg=move(4, dest, 0);
				break;
			case 6:
				msg=move(4, source, 0);
				break;	
			case 7:
				msg=move(2, source, 0);
				break;
			case 8:
				msg=move(2, source, 1);
				break;
			case 9:
				msg=move(4, source, 1);
				break;
			case 10:
				msg=move(4, intermed, 1);
				break;
			//step
			case 11:
				msg=move(2, intermed, 1);
				break;
			case 12:
				msg=move(1, intermed, 0);
				break;
			case 13:
				msg=move(4, intermed, 0);
				break;
			case 14:
				msg=move(4, dest, 0);
				break;
			case 15:
				msg=move(1, dest, 1);
				break;
			case 16:
				msg=move(4, dest, 1);
				break;
			case 17:
				msg=move(4, intermed, 1);
				break;
			case 18:
				msg=move(6, intermed, 0);
				break;
			case 19:
				msg=move(4, intermed, 0);
				break;
			case 20:
				msg=move(4, source, 0);
				break;
			//step
			case 21:
				msg=move(2, source, 0);
				break;
			case 22:
				msg=move(1, source, 1);
				break;
			case 23:
				msg=move(4, source, 1);
				break;
			case 24:
				msg=move(4, dest, 1);
				break;
			//step
			case 25:
				msg=move(3, dest, 1);
				break;
			case 26:
				msg=move(1, dest, 0);
				break;
			case 27:
				msg=move(4, dest, 0);
				break;
			case 28:
				msg=move(4, intermed, 0);
				break;
			case 29:
				msg=move(2, intermed, 1);
				break;
			case 30:
				msg=move(4, intermed, 1);
				break;
			case 31:
				msg=move(4, source, 1);
				break;
			//step
			case 32:
				msg=move(2, source, 1);
				break;
			case 33:
				msg=move(1, source, 0);
				break;
			case 34:
				msg=move(4, source, 0);
				break;
			case 35:
				msg=move(4, intermed, 0);
				break;
			//step
			case 36:
				msg=move(2, intermed, 0);
				break;
			case 37:
				msg=move(1, intermed, 1);
				break;
			case 38:
				msg=move(4, intermed, 1);
				break;
			case 39:
				msg=move(4, dest, 1);
				break;
			case 40:
				msg=move(6, dest, 0);
				break;
			case 41:
				msg=move(4, dest, 0);
				break;
			case 42:
				msg=move(4, source, 0);
				break;
			//step
			case 43:
				msg=move(2, source, 0);
				break;
			case 44:
				msg=move(1, source, 1);
				break;
			case 45:
				msg=move(4, source, 1);
				break;
			case 46:
				msg=move(4, dest, 1);
				break;
			case 47:
				msg=move(7, dest, 0);
				break;
			case 48:
				msg=move(4, dest, 0);
				break;
			case 49:
				msg.softhome = 1;
				break;
			default:
				break;
		}

		while(((msg.destination!=positions) || (msg.grip!=grip))&&ros::ok()){
			#ifdef DEBUG
				cout<<msg<<" destination"<<endl;
				cout<<"positions "<< positions[0]<<" "<<positions[1]<<" "<< positions[2]<<" "<< positions[3]<<" "<< positions[4]<<endl;
			#endif
			lab_pub.publish(msg);
			ros::spinOnce();
			loop_rate.sleep(); // "wait" so the program runs at rate of loop_rate, 4 Hz.
		}
		i++;

   	}
	//======================end of main loop==========================
	
	//test sub latches to pub.  Redundant but good practice
	while(lab_pub.getNumSubscribers()==0)
		poll_rate.sleep();
    
 
        //soft home
        msg.softhome = 1;
    
        //reset destination.
        msg.destination[0]=0;
        msg.destination[1]=0;
        msg.destination[2]=0;
        msg.destination[3]=0;
        msg.destination[4]=0;
        msg.grip=0;

        /**
        * check if the destination has been approached, if not, keep
        * sending msg until the destination has been reached.
        */
        while(msg.destination!=positions && ros::ok() && msg.grip==grip)
	{
        	lab_pub.publish(msg);
        	ros::spinOnce();
        	loop_rate.sleep();
       	}
		
        //reset msg.softhome, this is not necessary...
        msg.softhome = 0;
}

