
(cl:in-package :asdf)

(defsystem "rhino_ros-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "position" :depends-on ("_package_position"))
    (:file "_package_position" :depends-on ("_package"))
  ))