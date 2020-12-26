void Launcher(int angle){
  ServoLauncher.write(angle);
  delay(15);
}


void LauncherReset(){
  ServoLauncher.write(100);
  delay(1000);
  ServoLauncher.write(30);
}

