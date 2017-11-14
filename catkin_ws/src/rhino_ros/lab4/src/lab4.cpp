#include "ros/ros.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

/** for custom msg and srv
 */
#include <rhino_ros/command.h>
#include <rhino_ros/positions.h>


using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::atof;
#define DEBUG
int grip;
boost::array<int,5> positions; //global position variable.

void position_callback(const rhino_ros::positions::ConstPtr& msg){
	std::copy(msg->position.begin(),msg->position.begin()+5,positions.begin());
	grip=msg->grip;
}

boost::array<int,5> lab_angles(float theta1, float theta2, float theta3, float theta4, float theta5)
{
//=========================copy your lab_angles code from lab 3 to here=================
	boost::array<int,5> return_value;
	return return_value;
}
/** 
 * function that calculates numbers for each motor.
 * Function needs to modify for lab4
 */
boost::array<int,5> lab_movex(float xworld, float yworld, float zworld, float pitch, float roll)
// Computes of the the elbow-up inverse kinematics solutions
{
	double x,y,z,theta5,theta4,theta3,theta2,theta1,wx,wy,wz; 

	// First, convert xworld, yworld, zworld to coordinates in the base frame
	x= -999;
	y= -999;
	z= -999;

	// Now the wrist roll
	theta5 = -999;
	
	// the waist angle
	theta1 = -999;
	
	// Now the wrist center
	wx= -999;
	wy= -999;
	wz= -999;

	// The arm angles
	theta2= -999;
	theta3= -999;

	// Finally, the wrist angle
	theta4= -999;

	// View values
	//use cout
	printf("theta1 = %f degrees\n",theta1);
	printf("theta2 = %f degrees\n",theta2);
	printf("theta3 = %f degrees\n",theta3);
	printf("theta4 = %f degrees\n",theta4);
	printf("theta5 = %f degrees\n",theta5);

	// check that your values are good BEFORE sending commands to rhino
	return lab_angles((float)theta1,(float)theta2,(float)theta3,(float)theta4,(float)theta5);
}
 


int main(int argc, char **argv)
{
	//initialization & variable definition
	ros::init(argc, argv, "lab3");
	ros::NodeHandle nh;
	ros::Publisher lab_pub = nh.advertise<rhino_ros::command>("rhino/command",1000);
	ros::Subscriber lab_sub= nh.subscribe<rhino_ros::positions>("rhino/position",100,position_callback);
	rhino_ros::command msg;
	
	
	//latch on to the subscriber
	ros::Rate poll_rate(100);
	while(lab_pub.getNumSubscribers()==0)
		poll_rate.sleep();
	
	//soft home
	msg.softhome = 1;
	while(!ros::ok()){};
	lab_pub.publish(msg);
	ros::spinOnce();
	msg.softhome = 0;
	
	//command line parse:
	if(argc!=6){
		cout<<"invalid number of input"<<endl;
		cout<<"command should be rosrun lab# lab# xworld yworld zworld pitch roll"<<endl;
		return 0;
	}
	//float xworld, float yworld, float zworld, float pitch, float roll
	cout<<"xworld:"<<atof(argv[1])<<",yworld:"<<atof(argv[2])<<",zworld:"<<atof(argv[3])<<",pitch:"<<atof(argv[4])<<",roll:"<<atof(argv[5])<<endl;
	
	//run and print value from lab_movex
	lab_movex(atof(argv[1]),atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]));
	
	// check that your theta and encoder destination values are good BEFORE uncommenting two lines below
	// msg.destination=lab_movex(atof(argv[1]),atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]));
	// cout<<"msg destination: \n"<<msg<<endl;
	
	//========Main Loop========
	ros::Rate loop_rate(4);

	//latch on to the subscriber
	while(lab_pub.getNumSubscribers()==0)
		poll_rate.sleep();
	
	//publish the result.
	while((msg.destination!=positions ||msg.grip!=grip) && ros::ok()){
		lab_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
	//no softhome, therefore arm stays at the last destination.
	//to softhome, you can publish softhome through a new terminal:
	//rostopic pub /rhino/command rhino_ros/command "destination: [0, 0, 0, 0, 0] grip: 0 softhome: 1"
	return 0;
}
