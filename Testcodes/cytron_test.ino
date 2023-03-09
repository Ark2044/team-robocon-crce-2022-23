int motor_dir1 = 26;
int motor_pwm1 = 12;

int motor_dir2 = 24;
int motor_pwm2 = 13;

// int motor_dir3 = 49;
// int motor_pwm3 = 11;

// int motor_dir4 = 50;
// int motor_pwm4 = 12;

void setup()
{
    pinMode(motor_dir1, OUTPUT);
    pinMode(motor_pwm1, OUTPUT);

    pinMode(motor_dir2, OUTPUT);
    pinMode(motor_pwm2, OUTPUT);

    // pinMode(motor_dir3, OUTPUT);
    // pinMode(motor_pwm3, OUTPUT);

    // pinMode(motor_dir4, OUTPUT);
    // pinMode(motor_pwm4, OUTPUT);

    digitalWrite(motor_dir1, HIGH);
    digitalWrite(motor_dir2, LOW);
    // digitalWrite(motor_dir3, LOW);
    // digitalWrite(motor_dir4, HIGH);
}

void loop()
{
    analogWrite(motor_pwm1, 0);
    analogWrite(motor_pwm2, 0);
    // analogWrite(motor_pwm3, 0);
    // analogWrite(motor_pwm4, 0);
    delay(2000);

    analogWrite(motor_pwm1, 100);
    analogWrite(motor_pwm2, 100);
    // analogWrite(motor_pwm3, 100);
    // analogWrite(motor_pwm4, 100);
    delay(2000);
}