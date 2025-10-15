from machine import Pin, I2C
import ssd1306
import time
import espnow

# Configurer la communication I2C
i2c = I2C(0, sda=Pin(4), scl=Pin(3))

# Configurer l'écran OLED (128x64 pixels) sur le bus I2C
# SSD1306_I2C est une sous-classe de FrameBuffer. FrameBuffer fournit un support pour les primitives graphiques.
# http://docs.micropython.org/en/latest/pyboard/library/framebuf.html
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# Effacer l'écran en le remplissant de blanc, puis afficher la mise à jour
oled.fill(1)
oled.show()
time.sleep(0.1)  # Attendre 1 seconde

# Effacer l'écran à nouveau en le remplissant de noir
oled.fill(0)
oled.show()
time.sleep(1)  # Attendre encore une seconde

# Afficher du texte sur l'écran OLED
oled.text('Bonjour,', 0, 0)  # Afficher "Hello," à la position (0, 0)
oled.text('ANUMBY', 0, 16)  # Afficher "sunfounder.com" à la position (0, 16)

# La ligne suivante envoie ce qu'il faut afficher à l'écran
oled.show()


import network


sta=network.WLAN(network.STA_IF)
sta.active(True)
print(sta.config('mac'))

oled.text(sta.config('mac'), 0, 32) 

# La ligne suivante envoie ce qu'il faut afficher à l'écran
oled.show()


# A WLAN interface must be active to send()/recv()
sta = network.WLAN(network.WLAN.IF_STA)
sta.active(True)
sta.disconnect()   # Because ESP8266 auto-connects to last Access Point

e = espnow.ESPNow()
e.active(True)

while True:
    host, msg = e.recv()
    if msg:             # msg == None if timeout in recv()
        print(host, msg)
        oled.text(msg, 0, 48)
        oled.show()
        if msg == b'end':
            break

