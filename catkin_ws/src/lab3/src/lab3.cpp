#include "ros/ros.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <eigen3/Eigen/Dense> //sudo apt-get install libeigen3-dev
#include <cmath>
/** for custom msg and srv
 */
#include <rhino_ros/command.h>
#include <rhino_ros/positions.h>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::atof;
using Eigen::Matrix4f;
//#define DEBUG

boost::array<int,5> positions; //global position variable.

float theta1, theta2, theta3, theta4, theta5;

//callback function, detail see lab2.cpp
void position_callback(const rhino_ros::positions::ConstPtr& msg){
	std::copy(msg->position.begin(),msg->position.begin()+5,positions.begin());
}

/** translate DH parameters to Homogeneous Transformation Matrix
 *	Matrix4f is a type of 4x4 float numbered matrix.
 *  angles are in radian.
 *  distance are in cm.
 */
Matrix4f DH2HT(float a, float alpha, float d, float theta)
{
	Matrix4f HT;
	//you will need to write the HT matrix given a, alpha, d and theta
	HT<<    cos(theta),-sin(theta)*cos(alpha), sin(theta)*sin(alpha), a*cos(theta),
		sin(theta),cos(theta)*cos(alpha),-cos(theta)*sin(alpha),a*sin(theta),
		0,         sin(alpha),           cos(alpha),            d,
		0,         0,                     0,                    1;
	//print out a, alpha, d, theta and HT matrix.
	#ifdef DEBUG
		cout<< "a: "<<a<<"; alpha: "<<alpha<<"; d: "<<d<<"; theta: "<<"; "<<endl;
		cout<<"HT matrix: "<<HT<<endl;
	#endif
	return HT; 
}


/** 
 * function that calculates encoder numbers for each motor.
 */
boost::array<int,5> lab_angles(float theta1, float theta2, float theta3, float theta4, float theta5)
{
	boost::array<int,5> return_value;
	float enc_B, enc_C, enc_D, enc_E, enc_F;      // desired encoder values
	/* c programming note:
    	* If the first argument in a division is an INTEGER, c will perform
	* INTEGER division, eleminating any fractional part of the division.
	*
	*      float a;
	*      a = 5/3;  // assigns the integer 1 to a
	*
	* To avoid this problem, type cast the first integer as a float in
	* one of the two following equivalent ways.
	*
	*      float a;
	*      a = 5.0/3;  	// assigns the value 1.66666666 to a
	*	a = (float)5/3	// also assigns the value 1.6666666 to a
	*
	*/

	//===========Implement joint angle to encoder expressions here.====================
	// These are incorrect but will be a linear combination of thetas 
	// not necessarily all thetas

	enc_B=3.22*theta5;  
	enc_C=8.67*(theta4 +(theta3+133+(theta2-137))) ;
	enc_D=8.33*(theta3+133+(theta2-137));
	enc_E=-8.56*(theta2-137);
	enc_F=4.33*theta1;

	printf("Computed encoder values:\nB:%f \nC:%f \nD:%f \nE:%f \nF:%f\n\n",enc_B,enc_C,enc_D,enc_E,enc_F);
	
	return_value[0]=enc_B;
	return_value[1]=enc_C;
	return_value[2]=enc_D;
	return_value[3]=enc_E;
	return_value[4]=enc_F;
	return return_value;
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
	
	/**command line parse:
	 * for detail, google "main command line arguments c"
	/* command need to be lenght of 6 : lab3 theta1 theta2 theta3 theta4 theta5"
	 */
	if(argc!=6){
		cout<<"invalid number of input"<<endl;
		cout<<"command should be rosrun lab3 lab3 theta1 theta2 theta3 theta4 theta5"<<endl;
		return 0;
	}
	cout<<"theta1:"<<atof(argv[1])<<",theta2:"<<atof(argv[2])<<",theta3:"<<atof(argv[3])<<",theta4:"<<atof(argv[4])<<",theta5:"<<atof(argv[5])<<endl;
	theta1=atof(argv[1]);
	theta2=atof(argv[2]);
	theta3=atof(argv[3]);
	theta4=atof(argv[4]);
	theta5=atof(argv[5]);

	//~ float f = atof(sf); Function to turn string to float number.
	//assign return value from lab_angles to destination
	msg.destination=lab_angles(atof(argv[1]),atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]));
	cout<<"msg destination: \n"<<msg<<endl;
	//calculate and print forward kinematics.
	//Make sure to print text explaining all the parts of the Transformation Matrix
	cout<<"Forward Kinematics calculated :"<<endl;
	cout<<	DH2HT(0,M_PI/2.0,27,theta1)*
		DH2HT(23,0,0,theta2+(137*M_PI/180))*
		DH2HT(23,0,0,theta3+(-133*M_PI/180))*
		DH2HT(1.5,M_PI/2.0,0,theta4)*
		DH2HT(0,0,16,theta5)<<endl;

	//Main Loop
	ros::Rate loop_rate(4);
	
	//latch on to the subscriber
	while(lab_pub.getNumSubscribers()==0)
		poll_rate.sleep();
	
	//publish the result.
	while(msg.destination!=positions && ros::ok()){
		lab_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
	//no softhome, therefore arm stays at the last destination.
	//to softhome, you can publish softhome through a new terminal:
	//rostopic pub /rhino/command rhino_ros/command "destination: [0, 0, 0, 0, 0] grip: 0 softhome: 1"
	return 0;
}
