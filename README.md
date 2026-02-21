# Raspberry-Pi-Headless-USB-Display
Mini dashboard USB pour Raspberry Pi headless.

Affiche automatiquement :

Hostname

Adresse IP (WiFi / Ethernet)

Adresse MAC

Utilisation RAM

Aucun écran HDMI requis.
Parfait pour serveurs headless, homelab et projets cybersécurité.

🧰 Prérequis

Raspberry Pi (Zero 2 W / 3 / 4 / 5)

Raspberry Pi OS Lite

RP2350 + écran 1.47"

Câble USB

Arduino IDE

🧱 Architecture
Raspberry Pi (Linux)
        ↓ USB CDC
RP2350 (périphérique série)
        ↓
Écran LCD

Le Raspberry Pi envoie les informations système via USB.
Le Pico les reçoit et les affiche en temps réel.

🔹 1️⃣ Firmware Pico
Configuration Arduino IDE

Tools →

Board → Raspberry Pi Pico 2

USB Stack → Adafruit TinyUSB

USB Mode → CDC only

Téléverser le firmware sur le RP2350.

Une fois branché au Raspberry Pi, le Pico doit apparaître comme :

/dev/ttyACM0
🔹 2️⃣ Script Raspberry Pi

Créer le script :

sudo nano /usr/local/bin/usb-ip-display.sh

Coller :

#!/bin/bash

DEVICE="/dev/ttyACM0"

[ ! -e "$DEVICE" ] && exit 0

HOSTNAME=$(hostname)

WIFI_IP=$(ip -4 addr show wlan0 2>/dev/null | awk '/inet / {print $2}')
ETH_IP=$(ip -4 addr show eth0 2>/dev/null | awk '/inet / {print $2}')

WIFI_MAC=$(cat /sys/class/net/wlan0/address 2>/dev/null)
ETH_MAC=$(cat /sys/class/net/eth0/address 2>/dev/null)

RAM_TOTAL=$(free -m | awk '/Mem:/ {print $2}')
RAM_USED=$(free -m | awk '/Mem:/ {print $3}')
RAM_PERCENT=$((RAM_USED * 100 / RAM_TOTAL))

echo -e "HOST:$HOSTNAME\n" > $DEVICE
[ -n "$WIFI_IP" ] && echo -e "WIFI:$WIFI_IP\n" > $DEVICE
[ -n "$ETH_IP" ] && echo -e "ETH:$ETH_IP\n" > $DEVICE
[ -n "$WIFI_MAC" ] && echo -e "WIFI_MAC:$WIFI_MAC\n" > $DEVICE
[ -n "$ETH_MAC" ] && echo -e "ETH_MAC:$ETH_MAC\n" > $DEVICE
echo -e "RAM:${RAM_USED}MB/${RAM_TOTAL}MB (${RAM_PERCENT}%)\n" > $DEVICE

Rendre exécutable :

sudo chmod +x /usr/local/bin/usb-ip-display.sh
🔹 3️⃣ Automatisation avec systemd
Service
sudo nano /etc/systemd/system/usb-ip-display.service
[Unit]
Description=USB IP Display Service
After=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/usb-ip-display.sh
Timer (mise à jour toutes les 30 secondes)
sudo nano /etc/systemd/system/usb-ip-display.timer
[Unit]
Description=Run USB IP Display every 30 seconds

[Timer]
OnBootSec=10
OnUnitActiveSec=30
Unit=usb-ip-display.service

[Install]
WantedBy=timers.target
Activer
sudo systemctl daemon-reload
sudo systemctl enable usb-ip-display.timer
sudo systemctl start usb-ip-display.timer
🧪 Test manuel
sudo /usr/local/bin/usb-ip-display.sh
📸 Exemple d’affichage
HOST: raspberrypi
WIFI: 192.168.1.42
ETH: ---
WIFI_MAC: b8:27:eb:xx:xx:xx
RAM: 612MB/1998MB (30%)
