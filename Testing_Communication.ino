const byte START_BYTE = 0x02; // Start byte (STX)
const byte END_BYTE = 0x03; // End byte (ETX)

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Set LED_BUILTIN as output
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  if (Serial.available() > 0) { // If there's data available on serial
    byte incomingByte = Serial.read(); // Read incoming byte
    if (incomingByte == "a") { // If incoming byte is the start byte
      while (Serial.available() == 0) {} // Wait until more data arrives
      byte nextByte = Serial.read(); // Read the next byte
      if (nextByte == 'S') { // Check if it's an 'S'
        while (Serial.available() == 0) {} // Wait until more data arrives
        nextByte = Serial.read(); // Read the next byte
        if (nextByte == 'T') { // Check if it's a 'T'
          while (Serial.available() == 0) {} // Wait until more data arrives
          nextByte = Serial.read(); // Read the next byte
          if (nextByte == 'A') { // Check if it's an 'A'
            while (Serial.available() == 0) {} // Wait until more data arrives
            nextByte = Serial.read(); // Read the next byte
            if (nextByte == 'R') { // Check if it's an 'R'
              while (Serial.available() == 0) {} // Wait until more data arrives
              nextByte = Serial.read(); // Read the next byte
              if (nextByte == 'T') { // Check if it's a 'T'
                while (Serial.available() == 0) {} // Wait until more data arrives
                nextByte = Serial.read(); // Read the next byte
                if (nextByte == "b") { // Check if it's the end byte
                  digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED
                }
              }
            }
          }
        }
      }
    }
  }
}
// the first byte is the start byte. If it is, it reads the next five bytes and checks if they spell out "START". If they do and the sixth byte is the end byte, it turns on the LED. Note that this code is blocking, meaning that it waits for each byte to arrive before proceeding, so it may not be suitable for all applications.





