; Auto-generated. Do not edit!


(cl:in-package rhino_ros-msg)


;//! \htmlinclude command.msg.html

(cl:defclass <command> (roslisp-msg-protocol:ros-message)
  ((destination
    :reader destination
    :initarg :destination
    :type (cl:vector cl:integer)
   :initform (cl:make-array 5 :element-type 'cl:integer :initial-element 0))
   (grip
    :reader grip
    :initarg :grip
    :type cl:integer
    :initform 0)
   (softhome
    :reader softhome
    :initarg :softhome
    :type cl:integer
    :initform 0))
)

(cl:defclass command (<command>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rhino_ros-msg:<command> is deprecated: use rhino_ros-msg:command instead.")))

(cl:ensure-generic-function 'destination-val :lambda-list '(m))
(cl:defmethod destination-val ((m <command>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-msg:destination-val is deprecated.  Use rhino_ros-msg:destination instead.")
  (destination m))

(cl:ensure-generic-function 'grip-val :lambda-list '(m))
(cl:defmethod grip-val ((m <command>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-msg:grip-val is deprecated.  Use rhino_ros-msg:grip instead.")
  (grip m))

(cl:ensure-generic-function 'softhome-val :lambda-list '(m))
(cl:defmethod softhome-val ((m <command>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rhino_ros-msg:softhome-val is deprecated.  Use rhino_ros-msg:softhome instead.")
  (softhome m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command>) ostream)
  "Serializes a message object of type '<command>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    ))
   (cl:slot-value msg 'destination))
  (cl:let* ((signed (cl:slot-value msg 'grip)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'softhome)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command>) istream)
  "Deserializes a message object of type '<command>"
  (cl:setf (cl:slot-value msg 'destination) (cl:make-array 5))
  (cl:let ((vals (cl:slot-value msg 'destination)))
    (cl:dotimes (i 5)
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
      (cl:setf (cl:slot-value msg 'softhome) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command>)))
  "Returns string type for a message object of type '<command>"
  "rhino_ros/command")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command)))
  "Returns string type for a message object of type 'command"
  "rhino_ros/command")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command>)))
  "Returns md5sum for a message object of type '<command>"
  "16d9779b648110c827c3280a76afbcab")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command)))
  "Returns md5sum for a message object of type 'command"
  "16d9779b648110c827c3280a76afbcab")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command>)))
  "Returns full string definition for message of type '<command>"
  (cl:format cl:nil "int32[5] destination~%int32 grip~%int32 softhome~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command)))
  "Returns full string definition for message of type 'command"
  (cl:format cl:nil "int32[5] destination~%int32 grip~%int32 softhome~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'destination) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command>))
  "Converts a ROS message object to a list"
  (cl:list 'command
    (cl:cons ':destination (destination msg))
    (cl:cons ':grip (grip msg))
    (cl:cons ':softhome (softhome msg))
))
