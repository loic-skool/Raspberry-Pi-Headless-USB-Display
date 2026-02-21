📟 Raspberry Pi Headless USB Display
IP • MAC • RAM Monitoring via RP2350

Mini dashboard USB pour Raspberry Pi headless.

Affiche automatiquement :

Hostname

IP WiFi

IP Ethernet

Adresse MAC

Utilisation RAM

Aucun écran HDMI requis.
Idéal pour homelab, serveurs headless et projets cybersécurité.

🧰 Prérequis

Raspberry Pi (Zero 2 W / 3 / 4 / 5)

Raspberry Pi OS Lite

RP2350 + écran 1.47"

Arduino IDE

Câble USB

🧱 Architecture
Raspberry Pi (Linux)
        ↓ USB CDC
RP2350 (périphérique série)
        ↓
Écran LCD

Le Raspberry Pi envoie les informations système via USB.
Le Pico les reçoit et les affiche.

📂 Structure du projet
.
├── firmware/
│   └── HeadlessIpDisplay.ino
└── raspberry-pi/
    └── usb-ip-display.sh
🚀 Installation
🔹 1️⃣ Flasher le RP2350 (la clé USB)

Ouvrir Arduino IDE.

Tools →

Board → Raspberry Pi Pico 2

USB Stack → Adafruit TinyUSB

USB Mode → CDC only

Téléverser le firmware :

firmware/HeadlessIpDisplay.ino

Une fois branché au Raspberry Pi, le Pico doit apparaître comme :

/dev/ttyACM0
🔹 2️⃣ Installer le script sur le Raspberry Pi

Copier le fichier :

raspberry-pi/usb-ip-display.sh

Dans :

/usr/local/bin/

Puis rendre exécutable :

sudo chmod +x /usr/local/bin/usb-ip-display.sh
🔹 3️⃣ Test manuel

Brancher le RP2350 au Raspberry Pi puis exécuter :

sudo /usr/local/bin/usb-ip-display.sh

L’écran devrait immédiatement afficher les informations système.

🔁 (Optionnel) Automatisation avec systemd

Créer un service :

sudo nano /etc/systemd/system/usb-ip-display.service
[Unit]
Description=USB IP Display Service
After=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/usb-ip-display.sh

Créer un timer :

sudo nano /etc/systemd/system/usb-ip-display.timer
[Unit]
Description=Run USB IP Display every 30 seconds

[Timer]
OnBootSec=10
OnUnitActiveSec=30
Unit=usb-ip-display.service

[Install]
WantedBy=timers.target

Activer :

sudo systemctl daemon-reload
sudo systemctl enable usb-ip-display.timer
sudo systemctl start usb-ip-display.timer
📸 Exemple d’affichage
HOST: raspberrypi
WIFI: 192.168.1.42
ETH: ---
WIFI_MAC: b8:27:eb:xx:xx:xx
RAM: 612MB/1998MB (30%)
