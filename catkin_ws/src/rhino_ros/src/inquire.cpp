#include "rhino_ros/rhino.h"
#include "rhino_ros/position.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <ros/ros.h>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "inquire_server");
	ros::NodeHandle nh;
	ros::ServiceClient client = nh.serviceClient<rhino_ros::position>("rhino/position_srv");
	rhino_ros::position srv;
	srv.request.request=1;
	ros::Rate loop_rate(10);
	while(ros::ok()){
		client.call(srv);
		cout<<"at position? "<<!srv.response.at_position<<endl;
		loop_rate.sleep();
	}
}
