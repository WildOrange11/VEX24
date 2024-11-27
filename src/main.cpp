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
controller C2;

int fbpos = 0;
int lrpos = 0;
int armpos = 0;
int inpos = 0;
int f1 = 800;
int d2 = 6;

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

void move(int l = 0)
{
  LMotor1.spinFor(l, deg, false);
  LMotor2.spinFor(l, deg, false);
  LMotor3.spinFor(l, deg, false);
  RMotor1.spinFor(l, deg, false);
  RMotor2.spinFor(l, deg, false);
  RMotor3.spinFor(l, deg);
}

void go(directionType d = forward)
{
  LMotor1.spin(d);
  LMotor2.spin(d);
  LMotor3.spin(d);
  RMotor1.spin(d);
  RMotor2.spin(d);
  RMotor3.spin(d);
}

void pre_auton(void)
{
  chain.setVelocity(-100, percent);
  intake.spin();
}

void autonomous(void)
{
  d1.driveFor(4,sec);
  PickRing(); // Pick up the first ring
  move(100);
  wait(0.5, sec);
  move(500);
  wait(0.5, sec);
  PickGoal();
  move(100);
  PickRing();
  PickRing();
  move(-200);
  PlaceGoal();
}

void usercontrol(void)
{
  while (1)
  {
    fbpos = C1.Axis3.position();
    lrpos = C1.Axis1.position();
    if (fbpos > 10)
    {
      go();
    }
    else if (fbpos < -10)
    {
      go(reverse);
    }
    else
    {
      LMotor1.stop();
      LMotor2.stop();
      LMotor3.stop();
      RMotor1.stop();
      RMotor2.stop();
      RMotor3.stop();
    }

    if (lrpos > 0)
    {
      setSpeed(25, 75);
      go();
    }
    else if (lrpos < 0)
    {
      setSpeed(75, 25);
      go();
    }
    else
    {
      setSpeed(80, 80);
    }

    if (C1.ButtonL2.pressing())
    {
      chain.spin(forward);
      intake.spin(forward);
    }
    {
      chain.stop();
    }
    if (C1.ButtonR1.pressing())
    {
      chain.spin(forward);
      intake.spin(forward);
    }
    else
    {
      chain.stop();
    }
    if (C1.ButtonR2.pressing() || C2.ButtonR2.pressing())
    {
      goal1.set(false);
    }
    if (C1.ButtonL2.pressing() || C2.ButtonR2.pressing())
    {
      goal1.set(true);
    }
    wait(0.2, sec);
  }
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
