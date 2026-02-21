📟 Raspberry Pi Headless USB Display

Mini dashboard USB pour Raspberry Pi headless
Affiche IP, MAC et RAM en temps réel sur un écran RP2350.

✨ Aperçu

Transforme ton Raspberry Pi headless en serveur visuellement monitoré, sans écran HDMI.

L’écran affiche automatiquement :

🖥 Hostname

🌐 IP WiFi / Ethernet

🔗 Adresse MAC

🧠 Utilisation RAM

🧱 Architecture
Raspberry Pi (Linux)
        │
        │ USB (CDC Serial)
        ▼
RP2350 + LCD 1.47"

Le Raspberry Pi envoie les données système via USB.
Le RP2350 les reçoit et les affiche.

📂 Structure du projet
.
├── firmware/
│   └── HeadlessIpDisplay.ino
└── raspberry-pi/
    └── usb-ip-display.sh
🚀 Installation
1️⃣ Flasher le RP2350

Dans Arduino IDE :

Board → Raspberry Pi Pico 2

USB Stack → Adafruit TinyUSB

USB Mode → CDC only

Uploader :

firmware/HeadlessIpDisplay.ino

Brancher ensuite le RP2350 au Raspberry Pi.

Vérifier qu’il est détecté :

ls /dev/ttyACM*
2️⃣ Installer le script sur le Raspberry Pi

Copier le script :

raspberry-pi/usb-ip-display.sh

Dans :

/usr/local/bin/

Puis :

sudo chmod +x /usr/local/bin/usb-ip-display.sh
3️⃣ Test manuel
sudo /usr/local/bin/usb-ip-display.sh

L’écran devrait immédiatement afficher les informations système.

🔄 Mise à jour automatique (optionnel)

Créer un service systemd :

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
🧠 Comment ça fonctionne ?

Le Raspberry Pi envoie des lignes formatées :

KEY:VALUE

Exemple :

HOST:raspberrypi
WIFI:192.168.1.42
RAM:612MB/1998MB (30%)

Le firmware interprète ces clés et met à jour l’écran.
