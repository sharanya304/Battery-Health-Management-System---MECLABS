// ESP32 Battery Voltage & Temperature Monitoring


// Pin Definitions
const int batteryVoltagePin = 34;   // GPIO34 (ADC1)
const int thermistorPin     = 35;   // GPIO35 (ADC1)

// Battery Voltage Parameters
const float R1 = 10000.0;   // 10kΩ
const float R2 = 10000.0;   // 10kΩ

// Thermistor Parameters (NTC 5D-11)
const float thermistorNominal = 5000.0;   // 5kΩ at 25°C
const float temperatureNominal = 25.0;    // °C
const float bCoefficient = 3470.0;        // Beta value
const float seriesResistor = 10000.0;     // 10kΩ

void setup() {
  Serial.begin(115200);

  // ESP32 ADC resolution
  analogReadResolution(12);   // 0–4095
}

void loop() {

  float batteryVoltage = readBatteryVoltage();
  float temperature = readTemperature();

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.print(" V | ");

  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.println(" °C");

  delay(1000);
}

// -------- Battery Voltage Reading --------
float readBatteryVoltage() {
  int adcValue = analogRead(batteryVoltagePin);
  float voltage = (adcValue / 4095.0) * 3.3;

  // Voltage divider compensation
  voltage = voltage * (R1 + R2) / R2;

  return voltage;
}

// -------- Temperature Reading --------
float readTemperature() {
  int adcValue = analogRead(thermistorPin);
  if (adcValue == 0) return -100.0;

  float voltage = (adcValue / 4095.0) * 3.3;
  float resistance = seriesResistor * voltage / (3.3 - voltage);

  float temperatureK = 1.0 / (
    (log(resistance / thermistorNominal) / bCoefficient) +
    (1.0 / (temperatureNominal + 273.15))
  );

  return temperatureK - 273.15;
}