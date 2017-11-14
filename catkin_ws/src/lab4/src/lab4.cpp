#include "ros/ros.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <geometry_msgs/Point.h>

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
float xw=0;
float yw=0;
float zw;

int grip;
boost::array<int,5> positions; //global position variable.

void position_callback(const rhino_ros::positions::ConstPtr& msg){
	std::copy(msg->position.begin(),msg->position.begin()+5,positions.begin());
	grip=msg->grip;
}

void position_callback2(const geometry_msgs::Point::ConstPtr& msg){
	xw=msg->x;
	yw=msg->y;
	zw=msg->z;
}

boost::array<int,5> lab_angles(float theta1, float theta2, float theta3, float theta4, float theta5)
{
	boost::array<int,5> return_value;
	float enc_B, enc_C, enc_D, enc_E, enc_F;      // desired encoder values
	
	enc_B=theta5*(3.24);
	enc_C=(theta4+(theta3-125.0)+(theta2+125.0))*(8.8);
	enc_D=((theta3-125.0)+(theta2+125.0))*(8.7);
	enc_E=(theta2-125.0)*(-8.7);
	enc_F=theta1*(4.34);

	printf("Computed encoder values:\nB:%f \nC:%f \nD:%f \nE:%f \nF:%f\n\n",enc_B,enc_C,enc_D,enc_E,enc_F);
	
	return_value[0]=enc_B;
	return_value[1]=enc_C;
	return_value[2]=enc_D;
	return_value[3]=enc_E;
	return_value[4]=enc_F;
	return return_value;
}

/** 
 * function that calculates numbers for each motor.
 * Function needs to be modified for lab4
 */
boost::array<int,5> lab_movex(float xworld, float yworld, float zworld, float pitch, float roll)
// Computes of the the elbow-up inverse kinematics solutions
{
  double x,y,z,theta5,theta4,theta3,theta2,theta1; //,wx,wy,wz; 

	// First, convert xworld, yworld, zworld to coordinates in the base frame
	x= xworld;
	y= yworld;
	z= zworld;

	// Now the wrist roll
	theta5 = roll;
	
	// the waist angle
	theta1 = atan2(y,x)*(180/M_PI);
	
	double l1, l2, l3, l4, radP, Rw, Zw;

	radP=pitch*(M_PI/180);
	l1=16.0*cos(radP);
	l2=16.0*sin(radP);
	l3=1.0*cos(radP);
	l4=1.0*sin(radP);
	
	Rw=sqrt((x*x)+(y*y))-l2-l3;
	Zw=z+l1-l4;
	
	double L, b1, b2;
	
	L=sqrt((Rw*Rw)+((Zw-26.0)*(Zw-26.0)));
	b1=acos(L/46.0);
	b2= atan2(Zw-26.0,Rw); //in radians
	
	// The arm angles
	theta2= (b1+b2)*180/M_PI;
	theta3= -2*b1*180/M_PI;

	// Finally, the wrist angle
	theta4= (b1-b2)*180/M_PI+pitch;

	printf("theta1 = %f degrees\n",theta1);
	printf("theta2 = %f degrees\n",theta2);
	printf("theta3 = %f degrees\n",theta3);
	printf("theta4 = %f degrees\n",theta4);
	printf("theta5 = %f degrees\n",theta5);

	return lab_angles((float)theta1,(float)theta2,(float)theta3,(float)theta4,(float)theta5);
}

int main(int argc, char **argv)
{
//initialization & variable definition
	
	ros::init(argc, argv, "pickupandplace");
	ros::NodeHandle nh2;
	ros::Subscriber lab_sub2= nh2.subscribe<geometry_msgs::Point>("/rhino/target_position",100,position_callback2);

	ros::init(argc, argv, "lab3");
	ros::NodeHandle nh;
	ros::Publisher lab_pub = nh.advertise<rhino_ros::command>("rhino/command",1000);
	ros::Subscriber lab_sub= nh.subscribe<rhino_ros::positions>("rhino/position",100,position_callback);
	rhino_ros::command msg;

	//========Main Loop========
	ros::Rate loop_rate(4);

	//latch on to the subscriber
	ros::Rate poll_rate(100);
	while(lab_pub.getNumSubscribers()==0){
		poll_rate.sleep();
	}
	
	while(xw==0){
		ros::spinOnce();
	}
	// check that your theta and encoder destination values are good BEFORE uncommenting two lines below
	msg.destination=lab_movex(xw+1.5,yw,2.5,0,0);

	//publish the result.
	while((msg.destination!=positions || msg.grip!=grip) && ros::ok()){
		if(xw != 0 && yw != 0){
		lab_pub.publish(msg);}
		ros::spinOnce();
		loop_rate.sleep();
	}
	msg.grip=zw;

	while((msg.destination!=positions ||msg.grip!=grip) && ros::ok()){
		lab_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}

	msg.softhome=1;
	lab_pub.publish(msg);
	ros::spinOnce();
	loop_rate.sleep();
	
	msg.softhome=0;
	xw=0;
	yw=0;

	while(lab_pub.getNumSubscribers()==0){
		poll_rate.sleep();
	}

	while((xw == 0) && (yw == 0)){
		ros::spinOnce();
	}

	// check that your theta and encoder destination values are good BEFORE uncommenting two lines below
	msg.destination=lab_movex(xw,yw,4,0,0);

	while((msg.destination!=positions ||msg.grip!=grip) && ros::ok()){
		if((xw != 0) && (yw != 0)) 
		{
			lab_pub.publish(msg);
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	msg.grip=zw;

	while((msg.destination!=positions ||msg.grip!=grip) && ros::ok()){
		lab_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}

	msg.softhome=1;
	lab_pub.publish(msg);
	ros::spinOnce();
	loop_rate.sleep();

	//no softhome, therefore arm stays at the last destination.
	//to softhome, you can publish softhome through a new terminal:
	//rostopic pub /rhino/command rhino_ros/command "destination: [0, 0, 0, 0, 0] grip: 0 softhome: 1"
	return 0;
}
