//R2R SYNTH TEST BED
// Collin Cunningham
// 4-27-08


//set ints, etc here
int potval;

void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}


  void loop()

  {
    potval = analogRead(0); // Read pot value to determine pitch
    
    // Turn each pin high, then low in series ascending
    digitalWrite(0, HIGH); // Least signifigant bit
    delayMicroseconds(potval);
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    delayMicroseconds(potval);
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
    delayMicroseconds(potval);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    delayMicroseconds(potval);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    delayMicroseconds(potval);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delayMicroseconds(potval);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    delayMicroseconds(potval);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH); // Most signifigant bit
    delayMicroseconds(potval);
    digitalWrite(7, LOW);
    
    // Turn each pin high, then low in series descending
    digitalWrite(6, HIGH);
    delayMicroseconds(potval);
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    delayMicroseconds(potval);
    digitalWrite(5, LOW);
    digitalWrite(4, HIGH);
    delayMicroseconds(potval);
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
    delayMicroseconds(potval);
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
    delayMicroseconds(potval);
    digitalWrite(2, LOW);
    digitalWrite(1, HIGH);
    delayMicroseconds(potval);
    digitalWrite(1, LOW);
    digitalWrite(0, HIGH);
    delayMicroseconds(potval);
    digitalWrite(0, LOW);
}
