// Define analog input pins for piezo sensors
const int KickPin = A0;
const int SnarePin = A1;
const int CymbalPin = A2;
const int TomPin1 = A3;
const int TomPin2 = A4;
const int HatPin = A5;

// MIDI note numbers for each drum
const int KickNote   = 36;  // MIDI note for Kick drum
const int SnareNote  = 38;  // MIDI note for Snare drum
const int CymbalNote = 49;  // MIDI note for Cymbal
const int Tom1Note   = 45;  // MIDI note for Tom 1
const int Tom2Note   = 47;  // MIDI note for Tom 2
const int HatNote    = 42;  // MIDI note for Hi-Hat

// Velocity and threshold
const int threshold = 50;    // Minimum analog value to detect a hit
const int minVelocity = 1;   // Minimum MIDI velocity
const int maxVelocity = 127; // Maximum MIDI velocity

// Variables for hit detection
int piezoValues[6];
int peakValues[6];        // Peak values during hits
bool hitDetected[6];      // Flag to detect hits

void setup() {
  // No Serial setup needed, as we send MIDI
  Serial.begin(31250);
  // Initialize values
  for (int i = 0; i < 6; i++) {
    hitDetected[i] = false;
    peakValues[i] = 0;
  }
}

void loop() {
  // Check all piezo sensors
  for (int i = 0; i < 6; i++) {
    piezoValues[i] = analogRead(getAnalogPin(i));

    // Detect the start of a hit
    if (piezoValues[i] > threshold && !hitDetected[i]) {
      hitDetected[i] = true;          // Mark hit as detected
      peakValues[i] = piezoValues[i]; // Initialize peak value
    }

    // Track peak value during the hit
    if (hitDetected[i]) {
      if (piezoValues[i] > peakValues[i]) {
        peakValues[i] = piezoValues[i]; // Update peak value
      }

      // End hit when value drops below threshold
      if (piezoValues[i] < threshold) {
        int velocity = map(peakValues[i], threshold, 1023, minVelocity, maxVelocity);
        velocity = constrain(velocity, minVelocity, maxVelocity);

        // Send MIDI Note-On message
        sendMIDI(0x90, getNoteNumber(i), velocity);

        // Reset for next hit
        hitDetected[i] = false;
        peakValues[i] = 0;
      }
    }
  }

  delay(5); // Small delay for responsiveness
}

// Function to get the analog pin for each piezo sensor
int getAnalogPin(int index) {
  switch (index) {
    case 0: return KickPin;
    case 1: return SnarePin;
    case 2: return CymbalPin;
    case 3: return TomPin1;
    case 4: return TomPin2;
    case 5: return HatPin;
    default: return A0;
  }
}

// Function to get the MIDI note for each piezo sensor
int getNoteNumber(int index) {
  switch (index) {
    case 0: return KickNote;
    case 1: return SnareNote;
    case 2: return CymbalNote;
    case 3: return Tom1Note;
    case 4: return Tom2Note;
    case 5: return HatNote;
    default: return 36; // Default to Kick
  }
}

// Function to send MIDI Note-On messages
void sendMIDI(byte command, byte note, byte velocity) {
  Serial.write(command); // MIDI command (e.g., 0x90 for Note-On)
  Serial.write(note);    // MIDI note number
  Serial.write(velocity);// MIDI velocity
}
