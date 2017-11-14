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
int grip;
/**	void position_callback(const rhino_ros::positions::ConstPtr& msg):
 *  Callback function for subscriber subscribed to topic "rhino/position".
 * Every time a msg is published unber the topic "rhino/position", it will
 * be put into a buffer, when the ros::spinOnce() is involked, the msg will
 * be transfered to whoever is subscribed to that topic under the same msg type
 * <rhino_ros::positions>, and the callback funtion associate with each subscriber
 * will be invoked and be processed.
 */
void position_callback(const rhino_ros::positions::ConstPtr& msg){
	// copy msg to a gloabal variable, more info on std::copy(), use google.
	std::copy(msg->position.begin(),msg->position.begin()+5,positions.begin());
	grip=msg->grip;
	
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
	ros::spinOnce();		//let ros process all publish and subscribe once.
	msg.softhome = 0;
	
	//=========================Main Loop=============================
	//===== modify these codes for your own functions and inputs=====
	//===============================================================	
	// user input
	string mystr;
	// Read in some input - a simple user interface.
	int choice, wait=true;
	while(wait)
	{
		cout<<"Enter a number between 1 and 3. "<<endl;
		cin>>choice;
		
		// validate the input
		if(1>choice || choice>3)
		{
			cout<<choice<<" is a bad number - try again.\n"<<endl;
			return 0;
		}
		else
		{
			// quit waiting for good input
			wait=false;
		}
	}


	ros::Rate loop_rate(4); // high loop rate may result in undesired behaviors.
	int sign=1;
	
	// Move the waist back and forth "choice" times.
	while(ros::ok()&&choice>0)
	{
		msg.destination[0]=0;  // Motor B Encoder Destination
		msg.destination[1]=0;  // Motor C Encoder Destination
		msg.destination[2]=0;  // Motor D Encoder Destination
		msg.destination[3]=0;  // Motor E Encoder Destination
		msg.destination[4]=sign*200;  // Motor F Encoder Destination
		
		sign*=-1;
		/**
		 * check if the destination has been approached, if not, keep
		 * sending msg until the destination has been reached.
		 */
		while(msg.destination!=positions && ros::ok()){
			#ifdef DEBUG
				cout<<msg<<" destination"<<endl;
				cout<<"positions "<< positions[0]<<" "<<positions[1]<<" "<< positions[2]<<" "<< positions[3]<<" "<< positions[4]<<endl;
			#endif
			lab_pub.publish(msg);
			ros::spinOnce();
			loop_rate.sleep(); // "wait" so the program runs at rate of loop_rate, 4 Hz.
		}
		choice--;
	}
	//======================end of main loop==========================
	
	//test sub latches to pub.  Redundant but good practice
	while(lab_pub.getNumSubscribers()==0)
		poll_rate.sleep();
		
	//soft home
	msg.softhome = 1;
	msg.destination[4]=0; //reset destination.
	/**
	 * check if the destination has been approached, if not, keep
	 * sending msg until the destination has been reached.
	 */
	while(msg.destination!=positions && ros::ok()){
		lab_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
	//reset msg.softhome, this is not necessary....
	msg.softhome = 0;

	return 0;
}
