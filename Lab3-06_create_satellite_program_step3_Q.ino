#include "src/HeptaSat.h"

HeptaCdh cdh;
HeptaEps eps;

// Two separate thresholds prevent chattering when voltage hovers near the boundary
const float VOLTAGE_TURN_OFF = 3.7;
const float VOLTAGE_TURN_ON  = 3.9;

const float temperature = 25.0; // Temperature in degrees Celsius
bool sw3V3_is_on = true;

bool is_cmd_received(void);
char get_cmd(void);

void setup() {
  cdh.begin();
  eps.init();
  cdh.wait_for_serial();
  cdh.wait_for_sd();
}

void loop() {
  float battery_voltage = eps.get_battery_voltage();
  cdh.println("------------------------------");
  cdh.printf("Satellite Time: %.2f seconds\n", millis() / 1000.0); // Print time in seconds
  cdh.printf("Battery Voltage: %.2f V\n", battery_voltage);
  cdh.printf("Temperature: %.2f °C\n", temperature);
  cdh.println("------------------------------");

  if (sw3V3_is_on && battery_voltage < VOLTAGE_TURN_OFF) {
    cdh.println("Battery voltage is low! Switching off 3.3V SW to save power.");
    eps.switch_3V3_off();
    sw3V3_is_on = false;
  } else if (!sw3V3_is_on && battery_voltage > VOLTAGE_TURN_ON) {
    cdh.println("Battery voltage recovered. Switching on 3.3V SW.");
    eps.switch_3V3_on();
    sw3V3_is_on = true;
  } else {
    cdh.printf("3.3V SW is %s.\n", sw3V3_is_on ? "on" : "off");
  }

  if (is_cmd_received()) {
    char cmd = get_cmd();

    if (cmd != '\0') {
      cdh.printf("Received command: %c\n", cmd);

      // Process the command and respond accordingly
      switch (cmd) {
        case 'a': {
          for(uint8_t i = 0; i < 10; i++) {
            cdh.println("Hello HEPTA-SAT");
            delay(1000);
          }
          break;
        }

        case 'b': {
          // Write some code to measure the battery voltage and save it to a file on the SD card
          // You can refer to Lab3-04_save_battery_voltage_to_microsd





        }

        default:
          cdh.println("Unknown command");
          break;
      }
    }
  }
  delay(1000);
}

bool is_cmd_received(void) {
  // Check if there is any data available in the serial buffer
  return Serial.available() > 0;
}

char get_cmd(void) {
  // Read one command character and ignore line endings from Serial Monitor.
  while (is_cmd_received()) {
    char received_char = Serial.read();
    if (received_char != '\r' && received_char != '\n') {
      return received_char;
    }
  }
  return '\0'; // Return null character if no data is available
}
