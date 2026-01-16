#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup (SDA/SCL Pins depend on your board, e.g., A4/A5 for Uno)
// Default address 0x27 or 0x3F. Check your module.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int irEntry = 14;    // IR sensor at entry point
const int irExit  = 27;    // IR sensor at exit point
const int buzzer  = 13;
const int ledPin  = 12;

// Capacity Definitions
const int totalSeats = 40;
const int standingLimit = 10;
const int totalCapacity = totalSeats + standingLimit; // 50 people total

// State Variables
int currentCount = 0;

// Timing and debounce
unsigned long lastEntryTriggerTime = 0;
unsigned long lastExitTriggerTime = 0;
const int debounceDelay = 300; // ms - Shortened for better response, adjust if needed

// --- Function Prototypes ---
void updateLCD();
void handleEntry();
void handleExit();
void checkCapacityAlarm();

void setup() {
  Serial.begin(115200);

  // Set pin modes
  // CHANGED: Using INPUT instead of INPUT_PULLUP.
  // This relies on the IR sensor module to drive the pin HIGH (not blocked) or LOW (blocked).
  pinMode(irEntry, INPUT); 
  pinMode(irExit, INPUT);  
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Smart Bus Init ");
  delay(1500);
  lcd.clear();

  updateLCD();
}

void loop() {
  // Read sensor states. Assuming IR sensors output LOW when the beam is broken (person detected).
  int entryState = digitalRead(irEntry);
  int exitState  = digitalRead(irExit);

  unsigned long currentTime = millis();

  // Handle Entry: Triggered when state is LOW (beam broken) and debounced
  if (entryState == LOW && (currentTime - lastEntryTriggerTime > debounceDelay)) {
    handleEntry();
    lastEntryTriggerTime = currentTime;
  }

  // Handle Exit: Triggered when state is LOW (beam broken) and debounced
  if (exitState == LOW && (currentTime - lastExitTriggerTime > debounceDelay)) {
    handleExit();
    lastExitTriggerTime = currentTime;
  }
  
  // Continuous check for capacity alarm
  checkCapacityAlarm();
}

// --- Entry/Exit Logic ---

// Handle entry logic
void handleEntry() {
  currentCount++; // Always increment first
  updateLCD();
  Serial.print("Passenger Entered. Count: ");
  Serial.println(currentCount);
  
  // Play a brief tone for entry
  tone(buzzer, 1000, 100); 
}

// Handle exit logic
void handleExit() {
  if (currentCount > 0) {
    currentCount--;
    updateLCD();
    Serial.print("Passenger Exited. Count: ");
    Serial.println(currentCount);
    // Play a brief tone for exit
    tone(buzzer, 800, 100);
  } else {
    // Prevent count from going negative
    currentCount = 0;
    updateLCD();
    Serial.println("Warning: Count already 0. Error in detection.");
    noTone(buzzer); // Ensure buzzer is off if no exit occurred
  }
}

// --- Capacity Alarm and LCD Update ---

// Check if bus is over capacity and trigger continuous alarm
void checkCapacityAlarm() {
  if (currentCount > totalCapacity) {
    // Bus is over-filled: LED ON and continuous buzzer
    digitalWrite(ledPin, HIGH);
    // Use a tone frequency that stands out
    tone(buzzer, 2500); // Continuous tone (no duration)
    
    // Safety check to prevent rapid tone calls
    if (currentCount == totalCapacity + 1) { // Only print once when limit is first exceeded
        Serial.println("!!! BUS OVER-CAPACITY ALARM !!!");
    }
    
  } else {
    // Bus is at or under capacity: LED OFF and buzzer OFF
    digitalWrite(ledPin, LOW);
    noTone(buzzer);
  }
}

// LCD display update
void updateLCD() {
  int availableSpace = totalCapacity - currentCount;
  
  // Determine available seats/standing spots
  int seatsAvailable = totalSeats - currentCount;
  int standingSpotsAvail = 0;
  
  // Adjust based on current count
  if (currentCount <= totalSeats) {
    // Only seats are being used
    seatsAvailable = totalSeats - currentCount;
    standingSpotsAvail = standingLimit;
  } else if (currentCount > totalSeats && currentCount <= totalCapacity) {
    // Seats are full, standing spots are being used
    seatsAvailable = 0;
    standingSpotsAvail = totalCapacity - currentCount;
  } else {
    // Bus is over capacity
    seatsAvailable = 0;
    standingSpotsAvail = 0;
  }
  
  lcd.clear();
  
  // Line 1: Occupancy Status
  lcd.setCursor(0, 0);
  if (availableSpace > 0) {
    lcd.print("Avail: ");
    lcd.print(availableSpace);
    lcd.print("/");
    lcd.print(totalCapacity);
  } else if (availableSpace == 0) {
    lcd.print("BUS FULL! ");
    lcd.print(totalCapacity);
    lcd.print(" Max");
  } else {
    lcd.print("!! OVER FILLED !!");
  }

  // Line 2: Detailed Count
  lcd.setCursor(0, 1);
  lcd.print("Ppl:");
  lcd.print(currentCount);
  lcd.print(" S:");
  lcd.print(seatsAvailable);
  lcd.print(" ST:");
  lcd.print(standingSpotsAvail);
}
