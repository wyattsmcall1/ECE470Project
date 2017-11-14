# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "rhino_ros: 2 messages, 1 services")

set(MSG_I_FLAGS "-Irhino_ros:/home/youbot/catkin_ws/src/rhino_ros/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(rhino_ros_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv" NAME_WE)
add_custom_target(_rhino_ros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rhino_ros" "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv" ""
)

get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg" NAME_WE)
add_custom_target(_rhino_ros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rhino_ros" "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg" ""
)

get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg" NAME_WE)
add_custom_target(_rhino_ros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rhino_ros" "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rhino_ros
)
_generate_msg_cpp(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rhino_ros
)

### Generating Services
_generate_srv_cpp(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rhino_ros
)

### Generating Module File
_generate_module_cpp(rhino_ros
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rhino_ros
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(rhino_ros_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(rhino_ros_generate_messages rhino_ros_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv" NAME_WE)
add_dependencies(rhino_ros_generate_messages_cpp _rhino_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg" NAME_WE)
add_dependencies(rhino_ros_generate_messages_cpp _rhino_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg" NAME_WE)
add_dependencies(rhino_ros_generate_messages_cpp _rhino_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rhino_ros_gencpp)
add_dependencies(rhino_ros_gencpp rhino_ros_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rhino_ros_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rhino_ros
)
_generate_msg_lisp(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rhino_ros
)

### Generating Services
_generate_srv_lisp(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rhino_ros
)

### Generating Module File
_generate_module_lisp(rhino_ros
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rhino_ros
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(rhino_ros_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(rhino_ros_generate_messages rhino_ros_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv" NAME_WE)
add_dependencies(rhino_ros_generate_messages_lisp _rhino_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg" NAME_WE)
add_dependencies(rhino_ros_generate_messages_lisp _rhino_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg" NAME_WE)
add_dependencies(rhino_ros_generate_messages_lisp _rhino_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rhino_ros_genlisp)
add_dependencies(rhino_ros_genlisp rhino_ros_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rhino_ros_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rhino_ros
)
_generate_msg_py(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rhino_ros
)

### Generating Services
_generate_srv_py(rhino_ros
  "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rhino_ros
)

### Generating Module File
_generate_module_py(rhino_ros
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rhino_ros
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(rhino_ros_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(rhino_ros_generate_messages rhino_ros_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/srv/position.srv" NAME_WE)
add_dependencies(rhino_ros_generate_messages_py _rhino_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/command.msg" NAME_WE)
add_dependencies(rhino_ros_generate_messages_py _rhino_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/catkin_ws/src/rhino_ros/msg/positions.msg" NAME_WE)
add_dependencies(rhino_ros_generate_messages_py _rhino_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rhino_ros_genpy)
add_dependencies(rhino_ros_genpy rhino_ros_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rhino_ros_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rhino_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rhino_ros
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(rhino_ros_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rhino_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rhino_ros
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(rhino_ros_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rhino_ros)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rhino_ros\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rhino_ros
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(rhino_ros_generate_messages_py std_msgs_generate_messages_py)
