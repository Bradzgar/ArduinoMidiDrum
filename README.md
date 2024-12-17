# ArduinoMidiDrum
Arduino Uno R3 Board with MocoLufa Midi Firmware (https://github.com/kuwatay/mocolufa) to allow midi to usb using serialwrite. 
Code generated using chatgpt.

# Important Notes

Midi device should show as mocolufa or something similar in the DAW.

In the DrumMidi.ino, adjust 'const int threshold = 500; ' to tune piezo hit detection.

To be able to upload the sketch swap between the MocoLufa firmware and the Arduino Serial firmware:

  MocoLufa -> Arduino Serial:
    -A jumper between PIN 4(MOSI PB2) and PIN6 (grand) on ICSP connector.
     Reset is required to swicth the firmware mode.

  Arduino Serial -> MocoLufa:
    -Remove jumper and short the first two pins closest to the reset button.
    Unplug cable and plug back in.
