void Route(){
  // Route 1 - Near Corner Module
  //  movement(S, 0, 64.36);
  //  movement(TS, -90);
  //  movement(S, 0, 57.65);
  //  movement(TS, -60.51);
  //  Claw(true);
  //  movement(S, 0, 20);
  //  Claw(false);
  //  movement(TS, -180);
  //  movement(S, 0, 20);
  //  movement(TS, 15.51);
  //  movement(S, 0, 78.2);
  //  Claw(true);
  //  movement(S, 0, -10);
  //  Knocker(true);
  //  movement(2*S, 0, 35.8);
  //  Knocker(false);
  
  
    // Route 2 - multi, far
//    Sensors = false;
//    movement(S,0,75);
//    Sensors = true;
//    movement(TS,-45);
//    movement(S,0,36.125);
//    movement(TS,99);
//    Knocker(true);
//    movement(60,0,21);
//    Knocker(false);
//    delay(8000);
//    movement(S,0,-20);
//    movement(TS,-55);
//    movement(S,0,29);
//    movement(TS,35);
//    Claw(160);
//    movement(S,0,25);
//    movement(20,0,10);
//    movement(TS,-180);
//    Claw(90);
//    movement(S,0,33);
//    movement(TS,-35);
//    movement(S,0,29);
//    movement(TS,-128);
//    movement(S,0,20);
//    movement(30,0,10);
//    movement(30,0,-10);
//    Claw(160);
//    movement(30,0,-35);
//    Knocker(true);
//    movement(80,0,35);

      //Route 3 - Near then far
    Claw(160);
    Sensors = false;
    movement(S,0,33);
    Sensors = true;
    movement(0.8*TS,-40.89);
    Claw(90);
    movement(S,0,78);
    movement(TS,80.22);
    Knocker(true);
    movement(80,0,16);
    Knocker(false);
    movement(TS,-9);
    movement(30,0,10);
    movement(30,0,-10);
    Claw(160);
    movement(S,0,-16);
    Claw(30);
    Knocker(true);
    movement(80,0,16);
    Knocker(false);
//    movement(30,0,10);
//    movement(30,0,-10);
//    movement(S,0,-15);
//    movement(TS,-52.73);
//    movement(S,0,22.73);
//    movement(TS,34);
    movement(S,0,-6);
    movement(TS,-45);
    movement(S,0,35.26);
    movement(TS,22);
    Claw(160);
    movement(20,0,14);
//    movement(S,0,25);
//    movement(20,0,10);
    movement(0.6*TS,-180);
    Claw(90);
    movement(0.7*S,0,14);
    movement(TS,-22);
    movement(S,0,35.26);
    movement(TS,-135);
    movement(S,0,6);
    movement(30,0,10);
    movement(30,0,-10);
    Claw(160);
    movement(30,0,-20);
    Claw(30);
    Knocker(true);
    movement(80,0,20);
    
    
    
}
