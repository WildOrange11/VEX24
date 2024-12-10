#include "vex.h"

using namespace vex;

// define your global instances of motors and other devices here
brain B;

motor chain = motor(PORT1, ratio18_1);
motor LMotor1 = motor(PORT2, ratio6_1);
motor LMotor2 = motor(PORT3, ratio6_1);
motor LMotor3 = motor(PORT4, ratio6_1);
motor RMotor1 = motor(PORT5, ratio6_1);
motor RMotor2 = motor(PORT6, ratio6_1);
motor RMotor3 = motor(PORT7, ratio6_1);
motor intake = motor(PORT9, ratio18_1);

motor_group L1 = motor_group(LMotor1, LMotor2, LMotor3);
motor_group R1 = motor_group(RMotor1, RMotor2, RMotor3);

drivetrain d1 = drivetrain(L1, R1);

digital_out goal1 = digital_out(B.ThreeWirePort.A);

controller C1;

int fbpos = 0;
int lrpos = 0;
int armpos = 0;
int f1 = 800;
int f2 = 6;

competition Competition;

void PickRing()
{
  chain.spin(forward);
  intake.spin(forward);
}

void PickGoal()
{
  goal1.set(false);
}

void DropRing()
{
  goal1.set(false);
  intake.spinFor(1000, deg);
  chain.spinFor(-1000, deg);
}

void PlaceGoal()
{
  goal1.set(true);
}

void pre_auton(void)
{
  chain.setVelocity(100, percent);
  intake.setVelocity(100, percent);
  L1.setVelocity(100,percent);
  R1.setVelocity(100,percent);
  intake.spin(forward);
}

void autonomous(void)
{
  //There are 4 possible starting positions of the robot. The robot will always be placed so that the center of the robot is on the starting line...
  
  //Code for red alliance, right side
  chain.spinFor(5,sec);
  d1.driveFor(18,inches);
  d1.turn(90,deg);


}
void usercontrol(void)
{
  while (1)
  {
    fbpos = C1.Axis3.position();
    lrpos = C1.Axis1.position();
    if (fbpos > 10 and -10 < lrpos < 10)
    {
      d1.drive(forward);
    }
    else if (fbpos < -10)
    {
      d1.drive(reverse);
    }
    else if (-10 < lrpos < 10)
    {
      d1.stop(); //Stop only if the left-right axis is not pressed
    }
    if (lrpos > 10)
    {
      d1.turnFor(left, 2, deg);
    }
    else if (lrpos < -10)
    {
      d1.turnFor(right, 2, deg);
    }
  }

  if (C1.ButtonL2.pressing())
  {
    chain.spin(forward);
    intake.spin(forward);
  }
  else
  {
    chain.stop();
  }
  if (C1.ButtonR2.pressing())
  {
    goal1.set(false);
  }
  else
  {
    goal1.set(true);
  }
  wait(0.2, sec);
}

int main()
{
  // Run the pre-autonomous function.
  pre_auton();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  while (true)
  {
    wait(0.5, sec);
  }
}
