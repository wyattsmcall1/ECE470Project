#ifndef RHINO_H
#define RHINO_H
#include <boost/array.hpp>

/***** Constants *****/

// Commands to send the Rhino controller
#define FORWARD '+'
#define REVERSE '-'

// Number of send/receive cycles without motion to signal a stall
#define STALL 10
// Opening distance for the gripper.
#define CLOSE_A 50
// Number of joints on the arm (ignores the gripper and the "extra" motors 'G' and 'H')
#define JOINTS 5

//~ #define RHINO_DEBUG 0
//~ #define RHINO_MAIN 1
/***** Controller commands *****/

/**
 * Initializes internal variables.  Should be called at the start of a program.
 */
void rhino_init();

/**
 * Open communications with the Rhino controller.
 * return<0 indicates an error.
 */
int rhino_opencomm(int port);

/***** Information commands *****/

/**
 * Retrieve the "error register" from the controller.
 * return - <0 indicates an error, otherwise return the error count.
 */
int rhino_inquire(char motor);

bool rhino_inquires();
/***** Movement commands *****/

/**
 * Move the motor to the desired postion.
 *
 * motor - 'A' through 'H' (65 to 72)
 * destination - encoder count from the origin
 * return!=0 indicates an error
 */
int rhino_move(char motor, int destination);


/**
 * Move motors 'B' through 'F' all at once.
 * Think "multi-move()"
 */
int rhino_mmove(int destB, int destC, int destD, int destE, int destF);

/**
 * As above, but pass an array instead of individual positions.
 * Think "array multi-move()"
 */
//~ int rhino_ammove(int destinations[JOINTS]);
int rhino_ammove(boost::array<int,JOINTS> destination);

/**
 * Close communications with the Rhino controller.
 */
int rhino_closecomm();

/**
 * Open and close the Rhino's gripper.
 */
int rhino_grip();
void rhino_ungrip();

/**
 * Return the encoder position of the desired motor.
 * An invalid motor returns 0.
 */
int rhino_position(char motor);

boost::array<int,8> rhino_positions();
/***** Home commands *****/

/**
 * Return the motors to their saved home positions.
 * return<0 indicates an error.
 */
int rhino_softhome();
/**
 * Use the microswitches to find the home positions for all the motors.
 */
int rhino_hardhome();
/**
 * Find the home position for a single motor.
 */
int rhino_home(char motor);

/**
 * Reads the microswitch for each motor.
 * Returns 1 (TRUE) if it is closed, 0 (FALSE) if it is not.
 */
int rhino_switch(char motor);

#endif
