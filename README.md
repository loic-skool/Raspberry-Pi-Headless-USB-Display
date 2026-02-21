# Raspberry-Pi-Headless-USB-Display
# 📟 Raspberry Pi Headless USB Display

Mini dashboard USB pour Raspberry Pi headless.

Affiche automatiquement :

- Hostname
- IP WiFi
- IP Ethernet
- Adresse MAC
- Utilisation RAM

Aucun écran HDMI requis.

---

## 🧰 Prérequis

- Raspberry Pi (Zero 2 W / 3 / 4 / 5)
- Raspberry Pi OS Lite
- RP2350 + écran 1.47"
- Arduino IDE
- Câble USB

---

## 🧱 Architecture

Raspberry Pi (Linux)
        ↓ USB CDC
RP2350 (périphérique série)
        ↓
Écran LCD

Le Raspberry Pi envoie les informations système via USB.
Le Pico les affiche.

---

# 🚀 Installation

## 1️⃣ Flasher le Pico

Dans Arduino IDE :

- Board → Raspberry Pi Pico 2
- USB Stack → Adafruit TinyUSB
- USB Mode → CDC only

Téléverser le firmware.

Une fois branché au Raspberry Pi, le Pico doit apparaître comme :

/dev/ttyACM0

---

## 2️⃣ Installer le script sur le Raspberry Pi

Copier le fichier `usb-ip-display.sh` dans :

/usr/local/bin/

Puis :

sudo chmod +x /usr/local/bin/usb-ip-display.sh

---

## 3️⃣ Activer le service automatique

Copier les fichiers :

usb-ip-display.service → /etc/systemd/system/
usb-ip-display.timer   → /etc/systemd/system/

Puis exécuter :

sudo systemctl daemon-reload
sudo systemctl enable usb-ip-display.timer
sudo systemctl start usb-ip-display.timer

---

## 🧪 Test manuel

sudo /usr/local/bin/usb-ip-display.sh

---

## 📸 Exemple d'affichage

HOST: raspberrypi
WIFI: 192.168.1.42
ETH: ---
WIFI_MAC: b8:27:eb:xx:xx:xx
RAM: 612MB/1998MB (30%)

---

## 💡 Extensions possibles

- Température CPU
- Uptime
- Disk usage
- Load average
- Signal WiFi

Projet open-source, libre à modifier.
