from machine import Pin, I2C
from ms5837 import MS5837
import time

# Initialisation I2C (Exemple pour ESP32, ajustez les broches selon votre carte)
i2c = I2C(0, scl=Pin(20), sda=Pin(21), freq=400000)

# Initialisation du capteur
sensor = MS5837(i2c)

print("Capteur MS5837 prêt.")

while True:
    try:
        # Met à jour les données du capteur
        sensor.read()
        
        # Récupération des valeurs
        pression = sensor.get_pressure()
        temperature = sensor.get_temperature()
        
        print("Pression : {:.1f} mbar | Température : {:.2f} °C".format(pression, temperature))
        
    except Exception as e:
        print("Erreur de lecture :", e)
        
    time.sleep(1)
