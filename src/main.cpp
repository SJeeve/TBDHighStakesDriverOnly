
#include "vex.h"

using namespace vex;

competition Competition;
brain Brain;
controller Controller = controller();

motor intake = motor(PORT11, false);
//Initializing drivetrain motors
motor rightFront = motor(PORT12, true);
motor rightMiddle = motor(PORT13, true);
motor rightBack = motor(PORT14, true);
motor leftFront = motor(PORT15, false);
motor leftMiddle = motor(PORT16, false);
motor leftBack = motor(PORT17, false);
//Motor groups
motor_group leftDriveSmart = motor_group(leftFront, leftMiddle, leftBack);
motor_group rightDriveSmart = motor_group(rightFront, rightMiddle, rightBack);

//These values are in inches
float wheelTravel = 2.75 * M_PI;
float trackWidth = 18;
float wheelBase = 18;
bool FineControl = false;
//Gear ratio
//Forgot if it's driven to driver or the other way around
float externalGearRatio = 1;

drivetrain Drivetrain = drivetrain(leftDriveSmart, rightDriveSmart, wheelTravel, trackWidth, wheelBase, inches, externalGearRatio);

digital_out MobileGoalSolenoid = digital_out(Brain.ThreeWirePort.A);
//Change controls here
const vex::controller::button SpinIntakeForward = Controller.ButtonR1; 
const vex::controller::button SpinIntakeBackward = Controller.ButtonL1;
const vex::controller::button ActivateFineControl = Controller.ButtonX;
const vex::controller::button ActivateMobileGoalSolenoid = Controller.ButtonA;

bool MobileGoalSolenoidIsActive = false;
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

void autonomous(void) {

}

void usercontrol(void) {
  leftDriveSmart.spin(forward);
  rightDriveSmart.spin(forward);
  intake.spin(forward);
  while (1) {
    float leftDrive = Controller.Axis4.position() - Controller.Axis1.position();
    float rightDrive = Controller.Axis4.position() + Controller.Axis1.position();
    if(ActivateMobileGoalSolenoid.pressing())
    {
      if(MobileGoalSolenoidIsActive)
      {
        MobileGoalSolenoid.set(false);
      } else 
      {
        MobileGoalSolenoid.set(true);
      }
      MobileGoalSolenoidIsActive = !MobileGoalSolenoidIsActive;
    }
    //If we decide to keep this I would want an LED so it's easier to tell when it's on or off
    if(ActivateFineControl.pressing())
    {
      FineControl = !FineControl;
    }

    if(FineControl){
      leftDrive = pow(leftDrive, 3);
      rightDrive = pow(rightDrive, 3);
    }



    leftDriveSmart.spin(forward, leftDrive * 6, volt);
    rightDriveSmart.spin(forward, rightDrive * 6, volt);

    //kinda shitty code but it should work
    if(SpinIntakeForward.pressing())
    {
      intake.spin(forward, 12, volt);
    } else if(SpinIntakeBackward.pressing())
    {
      intake.spin(forward, -12, volt);
    } else {
      intake.stop();
    }
    wait(20, msec);  
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
