int latch = 5;
int clck = 6;
int data = 7;

void setup() {
  pinMode(clck, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, INPUT);
  
  Serial.begin(9600);
}

byte buttons = 0;

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  
  digitalWrite(latch, LOW);
  for(int i = 0; i < 8; ++i) {
    bitWrite(buttons, i, !(digitalRead(data)));
    digitalWrite(clck, HIGH);
    digitalWrite(clck, LOW);
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(latch, HIGH);

  Serial.write(buttons);
}
