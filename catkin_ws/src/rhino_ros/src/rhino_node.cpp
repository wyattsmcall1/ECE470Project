#include "rhino_ros/rhino_node.h"
#include "rhino_ros/rhino.h"
#include "ros/ros.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>

#define RHINO_NODE_MAIN 1
#define TIMER_TIMEOUT (0.01F) //in sec

using std::cout;
using std::endl;

rhino_comm::rhino_comm(){
	rhino_init();
	rhino_opencomm(1);
	ctrl_sub=nh.subscribe("rhino/command",500,&rhino_comm::ctrl_sub_callback,this);
	position_srv=nh.advertiseService("rhino/position_srv",&rhino_comm::position_srv_callback,this);
	pos_pub=nh.advertise<rhino_ros::positions>("rhino/position",100);
	rhino_timer=nh.createTimer(ros::Duration(TIMER_TIMEOUT), boost::bind(&rhino_comm::onTimer,this));
}
rhino_comm::~rhino_comm(){
	rhino_closecomm();
}

void rhino_comm::ctrl_sub_callback(const rhino_ros::command::ConstPtr& msg){
	cout<<(*msg)<<endl;
	if(msg->softhome){
		rhino_softhome();
	}
	else{
		rhino_ammove(msg->destination);
		if(msg->grip){
			rhino_grip();
		}
		else{
			rhino_ungrip();
		}
	}
}

bool rhino_comm::position_srv_callback(	rhino_ros::position::Request &req,
										rhino_ros::position::Response &res){
	res.position=rhino_positions();
	res.at_position=rhino_inquires();
	return true;
}

void rhino_comm::onTimer(){
	boost::array<int,8> pos=rhino_positions();
	std::copy(pos.begin()+1,pos.begin()+6,pos_msg.position.begin());
	if(pos[0]>0)
		pos_msg.grip=1;
	else
		pos_msg.grip=0;
	pos_msg.at_position=rhino_inquires();
	pos_pub.publish(pos_msg);
}

#ifdef RHINO_NODE_MAIN
int main(int argc, char **argv)
{
	ros::init(argc, argv, "rhino_node");
	rhino_comm rhino_start;
	ros::spin();
	return 0;
}
#endif
