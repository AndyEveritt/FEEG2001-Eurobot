unsigned long DistanceFront(){
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonarFront.ping(); // Send ping, get ping time in microseconds (uS).
    unsigned long distance = uS/US_ROUNDTRIP_CM;
    if (distance == 0){distance = MAX_DISTANCE;}
//    Serial.print("Ping: ");
//    Serial.print(distance); // Convert ping time to distance in cm and print result (0 = outside set distance range)
//    Serial.println("cm");
    return(distance);
}


unsigned long DistanceBack(){
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonarBack.ping(); // Send ping, get ping time in microseconds (uS).
    unsigned long distance = uS/US_ROUNDTRIP_CM;
    if (distance == 0){distance = MAX_DISTANCE;}
//    Serial.print("Ping: ");
//    Serial.print(distance); // Convert ping time to distance in cm and print result (0 = outside set distance range)
//    Serial.println("cm");
    return(distance);
}


void DistanceCheck(){
  float e1 = encoder1();
  float e2 = encoder2();
  if (e1 > 0 && e2 > 0 && Sensors == true){
    if (DistanceFront() > MinDistanceF){AvoidMillis = millis();}
    
    if (millis() - AvoidMillis > 100){
      Serial.println("Stop");
      stopMotor();
  
      int i = 1;
      while (i <= 5 && DistanceFront() < MinDistanceF){
        Serial.println(i);
        delay(50);
        i += 1;
        if (i == 5 && DistanceFront() < MinDistanceF){
          Serial.println("Start Avoid");
          movement (10, 0, 0, DefaultAcceleration, false, false, true);
          Serial.println("End Avoid");
        }
      }
    }
  }
  else if (e1 < 0 && e2 < 0 && Sensors == true){
    if (DistanceBack() > MinDistanceB){AvoidMillis = millis();}
    
    if (millis() - AvoidMillis > 100){
      Serial.println("Stop");
      stopMotor();
  
      int i = 1;
      while (i <= 5 && DistanceBack() < MinDistanceB){
        Serial.println(i);
        delay(50);
        i += 1;
        if (i == 5 && DistanceBack() < MinDistanceB){
          Serial.println("Start Avoid");
          movement (10, 0, 0, DefaultAcceleration, false, false, true);
          Serial.println("End Avoid");
        }
      }
    }
  }
//  else if (Sensors == true){
//    if (DistanceFront() > MinDistanceF && DistanceBack() > MinDistanceB){AvoidMillis = millis();}
//    
//    if (millis() - AvoidMillis > 200){
//      Serial.println("Stop");
//      stopMotor();
//  
//      int i = 1;
//      while (i <= 5 && (DistanceFront() < MinDistanceF || DistanceBack() < MinDistanceB)){
//        Serial.println(i);
//        delay(50);
//        i += 1;
//        if (i == 5 && (DistanceFront() < MinDistanceF || DistanceBack() < MinDistanceB)){
//          Serial.println("Start Avoid");
//          movement (10, 0, 0, DefaultAcceleration, false, false, true);
//          Serial.println("End Avoid");
//        }
//      }
//    }
//  }
}

