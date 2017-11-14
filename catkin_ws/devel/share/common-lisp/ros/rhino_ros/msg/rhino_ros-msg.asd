
(cl:in-package :asdf)

(defsystem "rhino_ros-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "command" :depends-on ("_package_command"))
    (:file "_package_command" :depends-on ("_package"))
    (:file "positions" :depends-on ("_package_positions"))
    (:file "_package_positions" :depends-on ("_package"))
  ))