// Generated by gencpp from file rhino_ros/position.msg
// DO NOT EDIT!


#ifndef RHINO_ROS_MESSAGE_POSITION_H
#define RHINO_ROS_MESSAGE_POSITION_H

#include <ros/service_traits.h>


#include <rhino_ros/positionRequest.h>
#include <rhino_ros/positionResponse.h>


namespace rhino_ros
{

struct position
{

typedef positionRequest Request;
typedef positionResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct position
} // namespace rhino_ros


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::rhino_ros::position > {
  static const char* value()
  {
    return "5a6980b211ec1434829976b3c68e337c";
  }

  static const char* value(const ::rhino_ros::position&) { return value(); }
};

template<>
struct DataType< ::rhino_ros::position > {
  static const char* value()
  {
    return "rhino_ros/position";
  }

  static const char* value(const ::rhino_ros::position&) { return value(); }
};


// service_traits::MD5Sum< ::rhino_ros::positionRequest> should match 
// service_traits::MD5Sum< ::rhino_ros::position > 
template<>
struct MD5Sum< ::rhino_ros::positionRequest>
{
  static const char* value()
  {
    return MD5Sum< ::rhino_ros::position >::value();
  }
  static const char* value(const ::rhino_ros::positionRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::rhino_ros::positionRequest> should match 
// service_traits::DataType< ::rhino_ros::position > 
template<>
struct DataType< ::rhino_ros::positionRequest>
{
  static const char* value()
  {
    return DataType< ::rhino_ros::position >::value();
  }
  static const char* value(const ::rhino_ros::positionRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::rhino_ros::positionResponse> should match 
// service_traits::MD5Sum< ::rhino_ros::position > 
template<>
struct MD5Sum< ::rhino_ros::positionResponse>
{
  static const char* value()
  {
    return MD5Sum< ::rhino_ros::position >::value();
  }
  static const char* value(const ::rhino_ros::positionResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::rhino_ros::positionResponse> should match 
// service_traits::DataType< ::rhino_ros::position > 
template<>
struct DataType< ::rhino_ros::positionResponse>
{
  static const char* value()
  {
    return DataType< ::rhino_ros::position >::value();
  }
  static const char* value(const ::rhino_ros::positionResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // RHINO_ROS_MESSAGE_POSITION_H