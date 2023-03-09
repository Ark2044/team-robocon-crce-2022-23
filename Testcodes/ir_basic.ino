int ir1 = 3;
int ir2 = 100;

int led_ctr1 = 0;
int irStatus1 = 0;
int led_ctr2 = 0;
int irStatus2 = 0;
long long current_millis = 0;
const int DIR1 = 5;
const int PWM = 11;

void setup() {
  Serial.begin(9600);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(PWM, OUTPUT);
  pinMode(DIR1, OUTPUT);
}

void loop() {
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM, 127);
  int interval = 1000;
  if (millis() - current_millis == interval) {
    Serial.print("Motor1 ");
    Serial.println(led_ctr1);
    Serial.print("Motor2 ");
    Serial.println(led_ctr2);
    current_millis = millis();
    led_ctr1 = 0;
    led_ctr2 = 0;
  }

  if (digitalRead(ir1) == 1 && irStatus1 == 0) {
    led_ctr1++;
    // Serial.print("Motor1 ");
    //Serial.println(led_ctr1);
  }
  irStatus1 = digitalRead(ir1);

  if (digitalRead(ir2) == 1 && irStatus2 == 0) {
    led_ctr2++;
    //Serial.print("Motor2 ");
    //Serial.println(led_ctr2);
  }
  irStatus2 = digitalRead(ir2);
}
