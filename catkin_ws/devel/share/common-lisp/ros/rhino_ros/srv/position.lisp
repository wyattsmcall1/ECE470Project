; Auto-generated. Do not edit!


(cl:in-package rhino_ros-srv)


;//! \htmlinclude position-request.msg.html

(cl:defclass <position-request> (roslisp-msg-protocol:ros-message)
  ((request
    :reader request
    :initarg :request
    :type cl:fixnum
    :initform 0))
)

(cl:defclass position-request (<position-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <position-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'position-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rhino_ros-srv:<position-request> is deprecated: use rhino_ros-srv:position-request instead.")))

(cl:ensure-generic-function 'request-val :lambda-list '(m))
(cl:defmethod request-val ((m <position-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-srv:request-val is deprecated.  Use rhino_ros-srv:request instead.")
  (request m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <position-request>) ostream)
  "Serializes a message object of type '<position-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'request)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <position-request>) istream)
  "Deserializes a message object of type '<position-request>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'request)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<position-request>)))
  "Returns string type for a service object of type '<position-request>"
  "rhino_ros/positionRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'position-request)))
  "Returns string type for a service object of type 'position-request"
  "rhino_ros/positionRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<position-request>)))
  "Returns md5sum for a message object of type '<position-request>"
  "5a6980b211ec1434829976b3c68e337c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'position-request)))
  "Returns md5sum for a message object of type 'position-request"
  "5a6980b211ec1434829976b3c68e337c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<position-request>)))
  "Returns full string definition for message of type '<position-request>"
  (cl:format cl:nil "uint8 request~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'position-request)))
  "Returns full string definition for message of type 'position-request"
  (cl:format cl:nil "uint8 request~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <position-request>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <position-request>))
  "Converts a ROS message object to a list"
  (cl:list 'position-request
    (cl:cons ':request (request msg))
))
;//! \htmlinclude position-response.msg.html

(cl:defclass <position-response> (roslisp-msg-protocol:ros-message)
  ((position
    :reader position
    :initarg :position
    :type (cl:vector cl:integer)
   :initform (cl:make-array 8 :element-type 'cl:integer :initial-element 0))
   (at_position
    :reader at_position
    :initarg :at_position
    :type cl:integer
    :initform 0))
)

(cl:defclass position-response (<position-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <position-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'position-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rhino_ros-srv:<position-response> is deprecated: use rhino_ros-srv:position-response instead.")))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <position-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-srv:position-val is deprecated.  Use rhino_ros-srv:position instead.")
  (position m))

(cl:ensure-generic-function 'at_position-val :lambda-list '(m))
(cl:defmethod at_position-val ((m <position-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-srv:at_position-val is deprecated.  Use rhino_ros-srv:at_position instead.")
  (at_position m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <position-response>) ostream)
  "Serializes a message object of type '<position-response>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    ))
   (cl:slot-value msg 'position))
  (cl:let* ((signed (cl:slot-value msg 'at_position)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <position-response>) istream)
  "Deserializes a message object of type '<position-response>"
  (cl:setf (cl:slot-value msg 'position) (cl:make-array 8))
  (cl:let ((vals (cl:slot-value msg 'position)))
    (cl:dotimes (i 8)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'at_position) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<position-response>)))
  "Returns string type for a service object of type '<position-response>"
  "rhino_ros/positionResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'position-response)))
  "Returns string type for a service object of type 'position-response"
  "rhino_ros/positionResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<position-response>)))
  "Returns md5sum for a message object of type '<position-response>"
  "5a6980b211ec1434829976b3c68e337c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'position-response)))
  "Returns md5sum for a message object of type 'position-response"
  "5a6980b211ec1434829976b3c68e337c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<position-response>)))
  "Returns full string definition for message of type '<position-response>"
  (cl:format cl:nil "int32[8] position~%int32 at_position~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'position-response)))
  "Returns full string definition for message of type 'position-response"
  (cl:format cl:nil "int32[8] position~%int32 at_position~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <position-response>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'position) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <position-response>))
  "Converts a ROS message object to a list"
  (cl:list 'position-response
    (cl:cons ':position (position msg))
    (cl:cons ':at_position (at_position msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'position)))
  'position-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'position)))
  'position-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'position)))
  "Returns string type for a service object of type '<position>"
  "rhino_ros/position")