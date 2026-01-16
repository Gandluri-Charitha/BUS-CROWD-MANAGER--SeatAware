# BUS-CROWD-MANAGER--SeatAware
Using this monitor real-time seat and passenger availability, automate door operations based on occupancy. This project uses sensors and a microcontroller to count the number of passengers boarding and exiting the bus. Based on this data, the system dynamically updates seat availability on a display and controls door access to prevent overcrowding.

# Hardware Requirements 
   ESP32 Development Board – Main controller
   IR Sensor Pairs (2 Nos.) – For counting entry & exit
   16×2 LCD with I2C Backpack  – For displaying counts
   5V Power Supply / Phone Charger – Powering the system
   Breadboard & Jumper Wires – Circuit connections
   Passive Buzzer – For over-limit alert
   LED Indicator – Visual alert on full capacity
   Mounting Accessories – Tape, brackets
# Software Requirements
     Arduino IDE
    ESP32 Board Support Package
    Github
    VS code 
# METHODOLOGY
 Real-Time Seat Availability & Automated Door Control: 
 
Step 1: Sensor Integration for Passenger Counting Infrared (IR) or Ultrasonic sensors are    installed at the entrance and exit doors of the bus. These sensors detect  SeatAware when a person enters or exits the bus.A microcontroller (e.g., Arduino, ESP32, or Raspberry Pi) processes the sensor inputs.

Step 2: Real-Time Seat & Standing Capacity Monitoring:The system maintains a live count of the number of passengers on board. It compares this count with the total seating and standing capacity.If the maximum allowed limit is reached, the system:Prevents doors from opening. Displays "Bus Full“ message on the screen. 

Step 3: Automated Door Operation Based on the availability of space:If space is available, the microcontroller activates a relay or motor to open the bus door. If not, the door remains closed to ensure passenger safety and avoid overloading. 
 
Step 4: Real-Time Display System:A display unit (LED or LCD) at the front of the bus shows:Number of available seats.Number of allowed standing spots left. "Full" or "Available" status.





