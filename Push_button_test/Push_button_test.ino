void setup() {
  Serial.begin(115200);
pinMode(4,INPUT);

}

void loop() {
int pushbutton_read = digitalRead(4);
Serial.println(pushbutton_read);

}
