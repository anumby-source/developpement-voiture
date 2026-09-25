from machine import Pin, I2C
from ms5837 import MS5837
import ssd1306
import time

# Configurer la communication I2C
i2c = I2C(0, sda=Pin(21), scl=Pin(20))

# Configurer l'écran OLED (128x64 pixels) sur le bus I2C
# SSD1306_I2C est une sous-classe de FrameBuffer. FrameBuffer fournit un support pour les primitives graphiques.
# http://docs.micropython.org/en/latest/pyboard/library/framebuf.html
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

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
        oled.fill(0)
        oled.text("MS5837", 0, 0)
        oled.text("P: %.2f mbar" % pression, 0, 20)
        oled.text("T: %.1f C" % temperature, 0, 40)
        oled.show()
        print("Pression : {:.1f} mbar | Température : {:.2f} °C".format(pression, temperature))
        
    except Exception as e:
        print("Erreur de lecture :", e)
        
    time.sleep(1)


