//Omni Forward reciever code digital communication
//FR->m2 FL->m1

//CYTRON
const int m1_dir = 5;
const int m1_pwm = 6;

const int m2_dir = 7;
const int m2_pwm = 9;

//communication pins
int com_pin4 = 13;
int com_pin3 = 11;
int com_pin2 = 12;
int com_pin1 = 10;
int com_pin0 = 8;


int com_var = 0;

int target_slow_left = 45;
int target_slow_right = 45;

int target_normal_left = 85;
int target_normal_right = 85;

int target_fast_left = 130;
int target_fast_right = 130;

int target_stop_left = 0;
int target_stop_right = 0;

int current_value_left = 0;
int current_value_right = 0;

int step_accel_left = 1.0001;
int step_accel_right = 1.0001;

int step_decel_left = 2.2;
int step_decel_right = 2.2;


void setup() {
  Serial.begin(115200);

  //Cytron
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);

  //Communication pins
  pinMode(com_pin0, INPUT);
  pinMode(com_pin1, INPUT);
  pinMode(com_pin2, INPUT);
  pinMode(com_pin3, INPUT);
  pinMode(com_pin4, INPUT);

}

void loop() {

  com_var = digitalRead(com_pin4) * pow(2, 4) + digitalRead(com_pin3) * pow(2, 3) + digitalRead(com_pin2) * pow(2, 2) + digitalRead(com_pin1) * pow(2, 1) + digitalRead(com_pin0) * pow(2, 0);

  Serial.print("Pin4: ");
  Serial.print(digitalRead(com_pin4));

  Serial.print("Pin3: ");
  Serial.print(digitalRead(com_pin3));

  Serial.print(" Pin2: ");
  Serial.print(digitalRead(com_pin2));

  Serial.print(" Pin1: ");
  Serial.print(digitalRead(com_pin1));

  Serial.print(" Pin0: ");
  Serial.println(digitalRead(com_pin0));

  Serial.print("Com Variable: ");
  Serial.println(com_var);

  if (com_var == 1) {  // Slow Forward
    
    
    fwd();
    // analogWrite(m1_pwm, 45);
    // analogWrite(m2_pwm, 45);

    if (current_value_left < target_slow_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_slow_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_slow_left) {
      current_value_left = target_slow_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_slow_right) {
      current_value_right = target_slow_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }


  } else if (com_var == 2) {  // Slow Backward
    
    
    bkw();
    // analogWrite(m1_pwm, 45);
    // analogWrite(m2_pwm, 45);

    if (current_value_left < target_slow_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_slow_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_slow_left) {
      current_value_left = target_slow_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_slow_right) {
      current_value_right = target_slow_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 3) {  // Slow Left
    
    
    lt();
    // analogWrite(m1_pwm, 45);
    // analogWrite(m2_pwm, 45);

    if (current_value_left < target_slow_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_slow_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_slow_left) {
      current_value_left = target_slow_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_slow_right) {
      current_value_right = target_slow_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 4) {  // Slow Right
    
    
    rt();
    // analogWrite(m1_pwm, 45);
    // analogWrite(m2_pwm, 45);

    if (current_value_left < target_slow_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_slow_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_slow_left) {
      current_value_left = target_slow_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_slow_right) {
      current_value_right = target_slow_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 5) {  // Forward
    
    
    fwd();
    // analogWrite(m1_pwm, 95);
    // analogWrite(m2_pwm, 85);

    if (current_value_left < target_normal_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_normal_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_normal_left) {
      current_value_left = target_normal_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_normal_right) {
      current_value_right = target_normal_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }


  } else if (com_var == 6) {  // Backward
    
    
    bkw();
    // analogWrite(m1_pwm, 85);
    // analogWrite(m2_pwm, 85);

    if (current_value_left < target_normal_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_normal_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_normal_left) {
      current_value_left = target_normal_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_normal_right) {
      current_value_right = target_normal_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 7) {  // Left
    
    
    lt();
    // analogWrite(m1_pwm, 95);
    // analogWrite(m2_pwm, 85);

    if (current_value_left < target_normal_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_normal_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_normal_left) {
      current_value_left = target_normal_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_normal_right) {
      current_value_right = target_normal_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 8) {  // Right
    
    
    rt();
    // analogWrite(m1_pwm, 85);
    // analogWrite(m2_pwm, 85);

    if (current_value_left < target_normal_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_normal_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_normal_left) {
      current_value_left = target_normal_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_normal_right) {
      current_value_right = target_normal_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 9) {  // Fast Forward
    
    
    fwd();
    // analogWrite(m1_pwm, 130);
    // analogWrite(m2_pwm, 130);

    if (current_value_left < target_fast_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_fast_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_fast_left) {
      current_value_left = target_fast_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_fast_right) {
      current_value_right = target_fast_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 10) {  // Fast Backward
    
    
    bkw();
    // analogWrite(m1_pwm, 130);
    // analogWrite(m2_pwm, 130);

    if (current_value_left < target_fast_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_fast_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_fast_left) {
      current_value_left = target_fast_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_fast_right) {
      current_value_right = target_fast_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 11) {  // Fast Left
    
    
    lt();
    // analogWrite(m1_pwm, 130);
    // analogWrite(m2_pwm, 130);

    if (current_value_left < target_fast_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_fast_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_fast_left) {
      current_value_left = target_fast_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_fast_right) {
      current_value_right = target_fast_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 12) {  // Fast Right
    
    
    rt();
    // analogWrite(m1_pwm, 130);
    // analogWrite(m2_pwm, 130);

    if (current_value_left < target_fast_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_fast_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_fast_left) {
      current_value_left = target_fast_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_fast_right) {
      current_value_right = target_fast_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 13) {  // Slow Clock
    
    
    current_value_left = 18;
    current_value_right = 18;
    cw();
    analogWrite(m1_pwm, 18);
    analogWrite(m2_pwm, 18);
  } else if (com_var == 14) {  // Slow Anti Clock
    
    
    current_value_left = 18;
    current_value_right = 18;
    ccw();
    analogWrite(m1_pwm, 18);
    analogWrite(m2_pwm, 18);
  } else if (com_var == 15) {  // Clock
    
    
    current_value_left = 35;
    current_value_right = 35;
    cw();
    analogWrite(m1_pwm, 35);
    analogWrite(m2_pwm, 35);
  } else if (com_var == 16) {  // Anti Clock
    
    
    current_value_left = 35;
    current_value_right = 35;
    ccw();
    analogWrite(m1_pwm, 35);
    analogWrite(m2_pwm, 35);
  }

  else if (com_var == 17) {  // Forward wheels
    
    
    current_value_left = 130;
    current_value_right = 130;
    fwd();
    // analogWrite(m1_pwm, 100);
    // analogWrite(m2_pwm, 100);

    if (current_value_left < target_fast_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right < target_fast_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left > target_fast_left) {
      current_value_left = target_fast_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_fast_right) {
      current_value_right = target_fast_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

  } else if (com_var == 18) {  // Backward wheels
    
    
    current_value_left = 0;
    current_value_right = 0;
    fwd();
    analogWrite(m1_pwm, 0);
    analogWrite(m2_pwm, 0);
  } else if (com_var == 19) {  // Diagonal FL && RR
    
    
    fwd();
    // analogWrite(m1_pwm, 100);
    // analogWrite(m2_pwm, 0);

    if (current_value_left < target_fast_left) {
      current_value_left += step_accel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
      analogWrite(m2_pwm, 0);
    }

    if (current_value_left > target_fast_left) {
      current_value_left = target_fast_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
      analogWrite(m2_pwm, 0);
    }

  }

  else if (com_var == 20) {  // Diagonal FR && RL
    
    
    fwd();
    // analogWrite(m1_pwm, 0);
    // analogWrite(m2_pwm, 100);

    if (current_value_right < target_fast_right) {
      current_value_right += step_accel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_right > target_fast_right) {
      current_value_right = target_fast_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, current_value_right);
    }
  }

  else if (com_var == 0) {  //  Stop
    
    
    stp();

    if (current_value_left > target_stop_left) {
      current_value_left -= step_decel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_stop_right) {
      current_value_right -= step_decel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left <= target_stop_left) {
      current_value_left = target_stop_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right <= target_stop_right) {
      current_value_right = target_stop_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }
  }

  else if (com_var == 31) {  //  Stop
    
    
    stp();

    if (current_value_left > target_stop_left) {
      current_value_left -= step_decel_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right > target_stop_right) {
      current_value_right -= step_decel_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }

    if (current_value_left <= target_stop_left) {
      current_value_left = target_stop_left;
      Serial.print("Current Value Left");
      Serial.println(current_value_left);
      analogWrite(m1_pwm, current_value_left);
    }

    if (current_value_right <= target_stop_right) {
      current_value_right = target_stop_right;
      Serial.print("Current Value Right");
      Serial.println(current_value_right);
      analogWrite(m2_pwm, current_value_right);
    }
  }
  // delay(100);
}

//HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)
//FR->m2  FL->m1
void fwd() {
  // Serial.println("Bot moving forward");
  m1_cwMotor();
  m2_cwMotor();
}
void bkw() {
  // Serial.println("Bot moving backwards");
  m1_ccwMotor();
  m2_ccwMotor();
}
void rt() {
  // Serial.println("Bot moving right");
  m1_cwMotor();
  m2_ccwMotor();
}
void lt() {
  // Serial.println("Bot moving left");
  m1_ccwMotor();
  m2_cwMotor();
}
void ccw() {
  // Serial.println("Bot moving counter-clockwise");
  m1_ccwMotor();
  m2_cwMotor();
}
void cw() {
  // Serial.println("Bot moving clockwise");
  m1_cwMotor();
  m2_ccwMotor();
}
void stp() {
  // Serial.println("Bot stop");
  m1_stopMotor();
  m2_stopMotor();
}

//LOW LEVEL MOTOR FUNCTIONS

void m1_cwMotor() {
  // Serial.println("Motor A clockwise");
  digitalWrite(m1_dir, HIGH);
  //analogWrite(m1_pwm, pwr1);
}

void m1_ccwMotor() {
  // Serial.println("Motor A counter-clockwise");
  digitalWrite(m1_dir, LOW);
  //analogWrite(m1_pwm, pwr1);
}

void m2_cwMotor() {
  // Serial.println("Motor B clockwise");
  digitalWrite(m2_dir, HIGH);
  //analogWrite(m2_pwm, pwr2);
}

void m2_ccwMotor() {
  // Serial.println("Motor B counter-clockwise");
  digitalWrite(m2_dir, LOW);
  //analogWrite(m2_pwm, pwr2);
}

void m1_stopMotor() {
  // Serial.println("Motor A stop");
  // analogWrite(m1_pwm, 0);
  // analogWrite(m1_pwm, pwr1);
}

void m2_stopMotor() {
  // Serial.println("Motor B stop");
  // analogWrite(m2_pwm, 0);
  // analogWrite(m1_pwm, pwr2);
}