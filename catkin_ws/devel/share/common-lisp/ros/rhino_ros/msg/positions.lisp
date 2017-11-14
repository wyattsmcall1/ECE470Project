; Auto-generated. Do not edit!


(cl:in-package rhino_ros-msg)


;//! \htmlinclude positions.msg.html

(cl:defclass <positions> (roslisp-msg-protocol:ros-message)
  ((position
    :reader position
    :initarg :position
    :type (cl:vector cl:integer)
   :initform (cl:make-array 8 :element-type 'cl:integer :initial-element 0))
   (grip
    :reader grip
    :initarg :grip
    :type cl:integer
    :initform 0)
   (at_position
    :reader at_position
    :initarg :at_position
    :type cl:integer
    :initform 0))
)

(cl:defclass positions (<positions>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <positions>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'positions)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rhino_ros-msg:<positions> is deprecated: use rhino_ros-msg:positions instead.")))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <positions>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-msg:position-val is deprecated.  Use rhino_ros-msg:position instead.")
  (position m))

(cl:ensure-generic-function 'grip-val :lambda-list '(m))
(cl:defmethod grip-val ((m <positions>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-msg:grip-val is deprecated.  Use rhino_ros-msg:grip instead.")
  (grip m))

(cl:ensure-generic-function 'at_position-val :lambda-list '(m))
(cl:defmethod at_position-val ((m <positions>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-msg:at_position-val is deprecated.  Use rhino_ros-msg:at_position instead.")
  (at_position m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <positions>) ostream)
  "Serializes a message object of type '<positions>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    ))
   (cl:slot-value msg 'position))
  (cl:let* ((signed (cl:slot-value msg 'grip)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'at_position)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <positions>) istream)
  "Deserializes a message object of type '<positions>"
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
      (cl:setf (cl:slot-value msg 'grip) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'at_position) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<positions>)))
  "Returns string type for a message object of type '<positions>"
  "rhino_ros/positions")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'positions)))
  "Returns string type for a message object of type 'positions"
  "rhino_ros/positions")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<positions>)))
  "Returns md5sum for a message object of type '<positions>"
  "effceaa6d21d3d376be41978245bcf8f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'positions)))
  "Returns md5sum for a message object of type 'positions"
  "effceaa6d21d3d376be41978245bcf8f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<positions>)))
  "Returns full string definition for message of type '<positions>"
  (cl:format cl:nil "int32[8] position~%int32 grip~%int32 at_position~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'positions)))
  "Returns full string definition for message of type 'positions"
  (cl:format cl:nil "int32[8] position~%int32 grip~%int32 at_position~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <positions>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'position) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <positions>))
  "Converts a ROS message object to a list"
  (cl:list 'positions
    (cl:cons ':position (position msg))
    (cl:cons ':grip (grip msg))
    (cl:cons ':at_position (at_position msg))
))
