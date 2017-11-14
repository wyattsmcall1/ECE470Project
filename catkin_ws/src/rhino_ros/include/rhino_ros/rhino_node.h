#ifndef RHINO_NODE_H
#define RHINO_NODE_H
#include "ros/ros.h"
#include <stdlib.h>
#include <rhino_ros/command.h>
#include <rhino_ros/position.h>
#include <rhino_ros/positions.h>

/**
 * Class contains:
 * 	subscriber--wait for control command and controls rhino to desired position 
 * 	service/publisher-- sends current position. (service: on demand)(publisher: with const freq)
 * 	Rhino initialization-- rhino init, comm port init
 */   
class rhino_comm {
		/** variables:
		 *  publisher, subscriber, service, Timer, publisher msg
		 *  NOTE: service is too slow, therefore not used. Leave it be.
		 *  timer is used to trigger a callback function at a frequency. (100hz)
		 */
		ros::NodeHandle nh;
		ros::Subscriber ctrl_sub;
		ros::Publisher pos_pub;
		ros::ServiceServer position_srv;
		ros::Timer rhino_timer;
		rhino_ros::positions pos_msg;
		
		/** callback functions
		 */
		void ctrl_sub_callback(const rhino_ros::command::ConstPtr& msg);
		bool position_srv_callback(rhino_ros::position::Request &req,
									rhino_ros::position::Response &res);
		void onTimer();
	public:
		rhino_comm();
		~rhino_comm();
};

/**
 * COMMAND LINE PUBLISHER CONTROL:(SOFTHOME HAVE HIGHEST PRIORITY)
 * 	rostopic pub /rhino/command rhino_ros/command "{destination:[0,40,0,0,20], grip: 0, softhome: 0 }"
 * COMMAND LINE SERVICE REQUEST:
 * 	rosservice call /rhino/position_srv "request: 0"
 */

#endif
