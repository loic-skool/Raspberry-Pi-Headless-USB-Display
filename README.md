# 📟 Raspberry Pi Headless USB Display (Version bricolée 😄)

Petit projet perso pour afficher l’IP de mon Raspberry Pi headless sur un écran RP2350 branché en USB.

J’ai ajouté en bonus :
- Hostname
- IP WiFi / Ethernet
- Adresse MAC
- Utilisation RAM

Rien de fancy, juste du simple qui marche.

---

# 🧱 Comment je l’ai fait

## 1️⃣ Côté RP2350 (la clé USB)

J’ai flashé le firmware avec Arduino IDE.

Paramètres :

- Board → Raspberry Pi Pico 2
- USB Stack → Adafruit TinyUSB
- USB Mode → CDC only

Le Pico agit simplement comme un périphérique USB série (CDC).

Il attend des lignes formatées comme :

HOST:raspberrypi  
WIFI:192.168.1.42  
RAM:612MB/1998MB (30%)

Et il les affiche sur l’écran.

---

## 2️⃣ Côté Raspberry Pi

J’ai créé un script bash simple qui envoie les infos système vers `/dev/ttyACM0`.

Je l’ai mis ici :

/usr/local/bin/usb-ip-display.sh

Contenu du script :

```bash
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

```bash
sudo chmod +x /usr/local/bin/usb-ip-display.sh
