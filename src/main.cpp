
#include "vex.h"
using namespace vex;

vex::competition Competition;
brain Brain;
vex::controller Controller = vex::controller();

vex::motor conveyorBelt = vex::motor(PORT4, false);
vex::motor intake = vex::motor(PORT3, false);
//Initializing drivetrain vex::motors
vex::motor rightFront = vex::motor(PORT12, true);
vex::motor rightMiddle = vex::motor(PORT13, true);
vex::motor rightBack = vex::motor(PORT14, true);
vex::motor leftFront = vex::motor(PORT15, false);
vex::motor leftMiddle = vex::motor(PORT16, false);
vex::motor leftBack = vex::motor(PORT17, false);
//vex::motor groups
vex::motor_group leftDriveSmart = vex::motor_group(leftFront, leftMiddle, leftBack);
vex::motor_group rightDriveSmart = vex::motor_group(rightFront, rightMiddle, rightBack);

//These values are in inches
float wheelTravel = 2.75 * M_PI;
float trackWidth = 18;
float wheelBase = 18;
bool FineControl = false;
//Gear ratio
//Forgot if it's driven to driver or the other way around
float externalGearRatio = 1;

vex::drivetrain Drivetrain = vex::drivetrain(leftDriveSmart, rightDriveSmart, wheelTravel, trackWidth, wheelBase, inches, externalGearRatio);

vex::digital_out MobileGoalSolenoid = vex::digital_out(Brain.ThreeWirePort.A);
vex::digital_out FineControlLED = vex::digital_out(Brain.ThreeWirePort.B);
//Change controls here

const vex::controller::button SpinIntakeForward = Controller.ButtonR1; 
const vex::controller::button SpinIntakeBackward = Controller.ButtonL1;
const vex::controller::button ActivateFineControl = Controller.ButtonX;
const vex::controller::button ActivateMobileGoalSolenoid = Controller.ButtonA;

bool MobileGoalSolenoidIsActive = false;

void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {

}

void usercontrol(void) {
  leftDriveSmart.spin(forward);
  rightDriveSmart.spin(forward);
  intake.spin(forward);
  while (1) {
    float leftDrive = -1 * Controller.Axis3.position() - Controller.Axis1.position();
    float rightDrive = -1 * Controller.Axis3.position() + Controller.Axis1.position();
    if(ActivateMobileGoalSolenoid.pressing())
    {
      MobileGoalSolenoidIsActive = !MobileGoalSolenoidIsActive;
      MobileGoalSolenoid.set(MobileGoalSolenoidIsActive);
    }
    //If we decide to keep this I would want an LED so it's easier to tell when it's on or off
    if(ActivateFineControl.pressing())
    {
      FineControl = !FineControl;
      FineControlLED.set(FineControl);
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
      conveyorBelt.spin(forward, -12, volt);
    } else if(SpinIntakeBackward.pressing())
    {
      intake.spin(forward, -12, volt);
      conveyorBelt.spin(forward, 12, volt);
    } else {
      intake.stop();
      conveyorBelt.stop();
    }
    while(ActivateMobileGoalSolenoid.pressing())
      vex::wait(20, msec);
    vex::wait(20, msec);  
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
