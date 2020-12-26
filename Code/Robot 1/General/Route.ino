void Route(){
/*
  // Test route
  movement(S, 0, -80);
  movement(TS, -30);
  movement(S, 0, -50);
  movement(TS, 75);
  movement(S, 0, 135);
  movement(S, 0, -135);
  movement(TS, -75);
  movement(S,0,50);
  movement(TS,30);
  movement(S, 0, 80);
  exit(0);
*/

/*
Demonstration Route (large crater)
  */
//  movement(20, 0, -40);
//  movement(20, 0, -30);
//  movement(TS, -30);
//  movement(S, 0, -70);
//  movement(TS, 103.58);
//  movement(S, 0, 20);
//  movement(TS, -35);
//  movement(S, 0, 120);
//  SpinnerAngle(30);
//  Spinner(true);
//  movement(S, 0, 20);
//  movement(S,0,-20);
//  Spinner(false);
//  movement(TS, 6);
//  SpinnerAngle(160, 5);
//  LiftUp();
//  movement(S, 0, -85.636);
//  movement(TS, -130);
//  movement(S, 0, 59.025);
//  movement(TS, 55.42);
//  movement(S, 0, 20.142);
//  movement(TS, 30);
//  movement(S, 0, 70);
//  LiftTiltDown();
//  while (millis() > 0){TimeCheck();}


/*
 * Route 2 Small Crater
 */
//  movement(20,0,-50);
//  movement(20,0,-25);
//  movement(TS,-30);
//  movement(S,0,-60);
//  movement(TS,90);
//  movement(S,0,60);
//  LiftTiltDown();
//  movement(TS,-105);
//  SpinnerAngle(0);
//  movement(S,0,42);
//  Spinner(true);
//  movement(20,0,5);
//  movement(10,-30);
//  movement(10,60);
//  movement(10,-30);
//  movement(20,0,-5);
//  Spinner(false);
//  LiftUp();
//  SpinnerAngle(160, 5);
//  movement(S,0,-42);
//  movement(TS,105);
//  movement(S,0,-60);
//  LiftTiltDown();
//  movement(TS,-90);
//  movement(S,0,60);
//  movement(TS,30);
//  movement(20,0,40);
//  movement(20,0,35);
//  while (millis() > 0){TimeCheck();}



/*
 * Route 3 - Small Crater fast
  */
//  movement(20,0,-40);
//  movement(20,0,-30);
//  movement(TS,-30);
//  movement(S,0,-50);
//  movement(TS,40);
//  movement(S,0,35);
//  movement(30,0,5);
//  SpinnerAngle(30);
//  Spinner(true);
//  movement(20,0,20);
//  movement(S,0,-20);
//  Spinner(false);
//  LiftUp();
//  SpinnerAngle(160, 5);
//  movement(S,0,-40);
//  movement(TS,-40);
//  movement(S,0,44);
//  movement(TS,30);
//  movement(20,0,40);
//  movement(20,0,30);
//  LiftTiltDown();
//  End();



/*
 * Route 4 - Large Crater fast
  */
//  SpinnerAngle(30);
//  Sensors = false;
//  movement(20,0,-34,1,false);
//  movement(20,0,-30,1,false);
//  movement(TS,-30);
//  Sensors = true;
//  movement(S,0,-50);
//  movement(TS,75);
//  movement(S,0,120);
//  movement(TS,23);
//  SpinnerAngle(30);
//  Spinner(true);
//  movement(20,0,35);
//  movement(10,20);
//  movement(10,-20);
//  movement(S,0,-35);
//  Spinner(false);
//  LiftUp();
//  SpinnerAngle(160, 5);
//  movement(S,0,-65);
//  movement(TS,-90);
////  movement(TS,-28);
////  movement(S,0,-120);
////  movement(TS,-70);
////  movement(30,0,50);
////  Sensors = false;
////  movement(0.5*TS,32,0,1,false);
////  movement(15,0,40,1,false);
////  movement(15,0,40,1,false);
//  LiftTiltDown();
//  End();

  /*
 * Route 5 - Large Crater fast
  */
//  Sensors = false;
////  movement(20,0,34,1,false);
////  movement(20,0,30,1,false);
//  movement(20,0,66.7,1,false);
//  movement(TS,-30);
//  Sensors = true;
//  movement(S,0,50);
//  movement(TS,-82);
//  movement(S,0,120);
////  movement(TS,23);
//  SpinnerAngle(30);
//  Spinner(true);
//  movement(20,0,38);
////  movement(10,20);
////  movement(10,-20);
//  movement(S,0,-38);
//  Spinner(false);
//  LiftUp();
//  SpinnerAngle(160, 5);
//  movement(TS,40);
//  Sensors = false;
//  movement(S,0,-80);
//  movement(TS,-100);
////  movement(TS,-28);
////  movement(S,0,-120);
////  movement(TS,-70);
////  movement(30,0,50);
////  Sensors = false;
////  movement(0.5*TS,32,0,1,false);
////  movement(15,0,40,1,false);
////  movement(15,0,40,1,false);
//  LiftTiltDown();
//  End();

  /*
   * Route 6 - Large Crater around robot 2
   */
   Sensors = false;
   movement(10,0,67,1,false);
   movement(TS,-30);
//   Sensors = true;
   movement(S,0,45);
   movement(TS,-95);
   movement(S,0,61);
   movement(TS,-52);
   movement(S,0,53);
   movement(TS,90);
   movement(S,0,50);
   SpinnerAngle(30);
   Spinner(true);
   movement(15,0,10,1,false);
   movement(15,-20);
   movement(15,20);
   movement(20,0,-10,1,false);
   Spinner(false);
   LiftUp();
   SpinnerAngle(160,5);
   movement(S,0,-80);
   movement(TS,90);
   movement(20,0,-10);
   LiftTiltDown();
   TimeDelay(20);
   SpinnerAngle(30);
   Spinner(true);
   movement(20,0,10,1,false);
   Spinner(false);
   LiftUp();
   SpinnerAngle(160,5);
   movement(S,0,-20);
   LiftTiltDown();
   
}  
