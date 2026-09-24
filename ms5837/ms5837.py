import time
from machine import I2C

class MS5837:
    def __init__(self, i2c, address=0x76):
        self.i2c = i2c
        self.address = address
        self.C = [0] * 7
        self.pressure = 0.0
        self.temperature = 0.0
        
        # Réinitialisation du capteur
        self.reset()
        # Lecture des coefficients de calibration (PROM)
        self.read_prom()

    def reset(self):
        self.i2c.writeto(self.address, bytes([0x1E]))
        time.sleep_ms(10)

    def read_prom(self):
        for i in range(7):
            # Commande de lecture PROM : 0xA0 à 0xAE
            self.i2c.writeto(self.address, bytes([0xA0 + (i * 2)]))
            data = self.i2c.readfrom(self.address, 2)
            self.C[i] = (data[0] << 8) | data[1]

    def read(self):
        # 1. Lire la pression (D1) - OSR 8192
        self.i2c.writeto(self.address, bytes([0x4A]))
        time.sleep_ms(20) # Temps de conversion max pour OSR 8192
        self.i2c.writeto(self.address, bytes([0x00]))
        data = self.i2c.readfrom(self.address, 3)
        D1 = (data[0] << 16) | (data[1] << 8) | data[2]

        # 2. Lire la température (D2) - OSR 8192
        self.i2c.writeto(self.address, bytes([0x5A]))
        time.sleep_ms(20)
        self.i2c.writeto(self.address, bytes([0x00]))
        data = self.i2c.readfrom(self.address, 3)
        D2 = (data[0] << 16) | (data[1] << 8) | data[2]

        # 3. Calculs de compensation (selon la datasheet MS5837-30BA)
        dT = D2 - self.C[5] * 256
        OFF = self.C[2] * 65536 + (self.C[4] * dT) / 128
        SENS = self.C[1] * 32768 + (self.C[3] * dT) / 256
        
        TEMP = 2000 + (dT * self.C[6]) / 8388608

        # Compensation second ordre (basses températures)
        if TEMP < 2000:
            Ti = (11 * dT * dT) / 34359738368
            OFFi = (31 * (TEMP - 2000) * (TEMP - 2000)) / 8
            SENSi = (21 * (TEMP - 2000) * (TEMP - 2000)) / 4
        else:
            Ti = 0
            OFFi = 0
            SENSi = 0

        OFF -= OFFi
        SENS -= SENSi
        TEMP -= Ti

        P = (((D1 * SENS) / 2097152) - OFF) / 32768

        # Stockage des valeurs finales
        self.temperature = TEMP / 100.0  # En °C
        self.pressure = P / 10.0          # En mbar (ou hPa)

    def get_pressure(self):
        return self.pressure

    def get_temperature(self):
        return self.temperature

