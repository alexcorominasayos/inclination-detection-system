// Inclination Detection System
// Arduino UNO implementation based on the project's technical report.
// The report specifies a threshold of 0.8 g, corresponding to approximately
// 3.58 V or 733 ADC counts at a nominal 5 V reference.

const int pinSensor = A0;   // Conditioned accelerometer signal
const int ledMayor = 8;     // Green indicator: reading at/above threshold
const int ledMenor = 9;     // Red indicator: reading below threshold
const int pinBoton = 2;     // Toggle push button (INPUT_PULLUP)
const int pinMotor = 6;     // PWM output for the DC motor
const int pinPot = A1;      // Potentiometer for motor speed
const int umbralADC = 733;  // Approx. 0.8 g: 3.58 V / 733 ADC counts (per report)

bool estadoMotor = false;
bool ultimoEstadoBoton = HIGH;

void setup() {
  pinMode(ledMayor, OUTPUT);
  pinMode(ledMenor, OUTPUT);
  pinMode(pinBoton, INPUT_PULLUP);
  pinMode(pinMotor, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Read the conditioned accelerometer signal.
  const int lectura = analogRead(pinSensor);

  // Visual indication of whether the signal is above or below the threshold.
  if (lectura >= umbralADC) {
    digitalWrite(ledMayor, HIGH);
    digitalWrite(ledMenor, LOW);
  } else {
    digitalWrite(ledMayor, LOW);
    digitalWrite(ledMenor, HIGH);
  }

  // Toggle motor enable state when the push button is pressed.
  const bool lecturaBoton = digitalRead(pinBoton);
  if (lecturaBoton == LOW && ultimoEstadoBoton == HIGH) {
    estadoMotor = !estadoMotor;
    delay(200);  // Simple button debounce
  }
  ultimoEstadoBoton = lecturaBoton;

  // Convert the potentiometer reading into a PWM duty-cycle value.
  const int valorPot = analogRead(pinPot);
  const int intensidad = map(valorPot, 0, 1023, 0, 255);

  // The motor can run only when the sensor signal is below the threshold
  // and the user has enabled it with the push button.
  if (lectura >= umbralADC || !estadoMotor) {
    analogWrite(pinMotor, 0);
  } else {
    analogWrite(pinMotor, intensidad);
  }

  // Send the raw ADC reading over the serial connection.
  Serial.println(lectura);
}
