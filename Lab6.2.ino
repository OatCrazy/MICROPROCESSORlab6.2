#include <driver/ledc.h> // Include the LEDC library

// Define the pin for the buzzer
#define BUZZER_PIN 25

// Define the frequencies for each note
const float NOTES[] = {1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976};

// Define the song (sequence of notes)
const int SONG[] = {4, 2, 0, -1, 2, 4, 4, 4, -1, 2, 2, 2, -1, 4, 7, 7, -1, 4, 2, 0, -1, 2, 4, 4, 4, -1, 2, 2, 4, -1, 2, 0, -1}; // Example song with a blank note

volatile int speedIndex = 2; // Default speed index (normal speed)
volatile int buttonPin = 34; // Pin for the button
volatile bool buttonPressed = false; // Flag to indicate button press

// ISR for button interrupt
void IRAM_ATTR buttonISR() {
  buttonPressed = !buttonPressed; // Set flag to indicate button press
}

// Function to play a note
void playNote(int note, int duration, int pauseDuration) {
  if (note != -1) {
    ledcWriteTone(0, NOTES[note]); // Set the tone frequency
    delay(duration * 5); // Play the note for the specified duration
    ledcWriteTone(0, 0); // Silence the buzzer after the note duration
  } else {
    ledcWriteTone(0, 0); // Silence the buzzer during the pause
    delay(duration); // Silent period for the duration of the note
  }
  delay(pauseDuration * 5); // Add a pause duration between notes
}

// Function to play the song with current speed
void playSong() {
  int noteDuration = 200 / (speedIndex + 1); // Adjust duration based on speed
  int pauseDuration = noteDuration / 5; // Adjust the pause duration (1/5 of note duration)
  for (int i = 0; i < sizeof(SONG) / sizeof(SONG[0]); i++) {
    playNote(SONG[i], noteDuration, pauseDuration); // Adjust duration and pause between notes
  }
}

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(buttonPin, INPUT); // Enable internal pull-up resistor for the button pin
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, RISING ); // Attach ISR to the button pin with RISING edge trigger
  
  // Initialize LEDC
  ledcSetup(0, 5000, 8); // LEDC channel 0, 5 kHz frequency, 8-bit resolution
  ledcAttachPin(BUZZER_PIN, 0); // Attach BUZZER_PIN to LEDC channel 0
}

void loop() {
  Serial.println(buttonPressed);
  if (buttonPressed) {
    // Handle button press
    speedIndex = (speedIndex + 1) % 5; // Cycle through speed levels
    Serial.print("Current speed: ");
    switch (speedIndex) {
      case 0:
        Serial.println("Very Slow");
        break;
      case 1:
        Serial.println("Slow");
        break;
      case 2:
        Serial.println("Normal");
        break;
      case 3:
        Serial.println("Fast");
        break;
      case 4:
        Serial.println("Very Fast");
        break;
      default:
        Serial.println("Unknown");
    }
    buttonPressed = false; // Reset the flag
  }

  playSong();
  //Serial.println("Loop"+ buttonPressed);
}