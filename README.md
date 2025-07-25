# Jack Pop's Erschreck-O-Mat 👻  
Das hier ist ein kleines, feines Elektronikprojekt, bei dem ein Arduino-kompatibles Pro Micro Board mit einem Ultraschall-Abstandssensor (HC-SR04), einem Mp3-Modul (DF Player) und einem Mini-Lautsprecher verbunden wird, um immer dann einen Sound abzuspielen, wenn sich etwas vor dem Abstandssensor bewegt.

---
# Hardware

## 🔧 Komponenten & Werkzeuge

- <a href="https://amzn.to/4f71PXd" target="_blank" class="textlink">Pro Micro Entwicklungsboard ("Mini-Computer")</a>  
- <a href="https://amzn.to/3TWOzuo" target="_blank" class="textlink">HC-SR04 Ultraschall-Abstandssensor</a>  
- <a href="https://amzn.to/4o7AHLM" target="_blank" class="textlink">DFPlayer Mini MP3-Modul</a>  
- <a href="https://amzn.to/3H7AxTM" target="_blank" class="textlink">1k Widerstand für DF Player RX-Pin</a>  
- <a href="https://amzn.to/4kXpyKM" target="_blank" class="textlink">Micro-SD-Karte mit Audiodateien (z. B. MP3 oder WAV)</a>  
- <a href="https://amzn.to/44N7X3d" target="_blank" class="textlink">Mini-Lautsprecher (4–8 Ohm)</a>  
- <a href="https://amzn.to/4o0QAU5" target="_blank" class="textlink">Jumper-Kabel (female-female, 10cm, ca. 10 Stück)</a>  
- <a href="https://amzn.to/4kTfgLH" target="_blank" class="textlink">Stromversorgung: 9V-Block-Gehäuse mit On/Off-Schalter</a>  
- Lötkolben & Lötmaterial, alternativ: Klemmverbindung/Stoßverbindung (hier bitte bei Profis nachfragen)  

- **Optional: 3D-gedrucktes Gehäuse** (Link siehe unten), alternativ: 📦 kleine Pappkiste
- Sticker/Frontmotiv für Gehäuse-Deckel (Vorlage siehe unten)
  
**Disclaimer**

Ich übernehme keine Haftung für Bauteile, die am Ende nicht passen. Alle hier erwähnten Produkte habe ich selbst bestellt und getestet, manchmal ändern Hersteller aber Maße und/oder Funktionen, darauf habe ich keinen Einfluss.<br><br>Alle Bauteile bekommt ihr bei größeren Elektronik-Portalen wie <a href="https://www.reichelt.de/" target="_blank" class="textlink">Reichelt</a> etwas günstiger. Widerstände, zum Beispiel, kann man dort einzeln kaufen und nicht im 10er- oder 100er-Pack. Leider wollte trotz mehrfacher Nachfrage keiner der drei von mir angeschriebenen deutschen Online-Shops dieses Projekt supporten, weshalb es am Ende hier **Amazon-Affiliate-Links** geworden sind. Wenn ihr über diese Links kauft, bekomme ich einen kleinen Teil davon ab und ihr unterstützt mich und meine Projekte. Die Affiliate-Links an sich erzeugen für euch keine höheren Kosten.

Wenn ihr euch die Bauteile selbst zusammenstellt und mich trotzdem etwas supporten möchtet, könnt ihr das <a href="https://www.paypal.com/donate/?hosted_button_id=9MJAF9HTDPYHL" target="_blank" class="textlink">**hier über meinen Paypal-Spenden-Button**</a> tun. Ich freue mich auch über kleine Beträge, die mir helfen, die Entwicklung solcher Gadgets und die ganze Video- und Tutorial-Arbeit umzusetzen. 1000 Dank dafür! 🙏🏼

## 🖨️ 3D-gedrucktes Gehäuse

Ein passendes Gehäuse für das Projekt kann auf Thingiverse heruntergeladen und selbst gedruckt werden:  
🔗 **[Thingiverse: Erschreck-O-Mat Gehäuse](https://www.thingiverse.com/thing:7098738)**

Das Gehäuse bietet Platz für alle Komponenten und sorgt für eine saubere, stabile und unauffällige Unterbringung.
Alternativ eignet sich auch eine stabile Pappkiste 📦 mit folgenden Maßen: `9cm (Breite) x 4cm (Tiefe) x 8cm (Höhe)`

## 📸 Sticker-Vorlage

Der Foto-Sticker für den Deckel der Box kann entweder als 10x15cm-Freiform-Sofortsticker im Drogeriemarkt am Fotoautomat erstellt werden oder zu Hause auf Papier ausgedruckt werden. Eine Vorlage mit der passenden Größe für den 3D-Druck-Deckel liegt im Ordner [`/docs`](./docs).

## 📂 Dateistruktur auf der SD-Karte

Die Micro-SD-Karte im DFPlayer Mini sollte im Root-Verzeichnis einen Ordner `mp3` enthalten mit diesen zwei Dateien:

/mp3/  
- 0000.mp3 (Startsound / Countdown)
- 0001.mp3 (Erschreck-Sound)  

Die Dateien müssen genau so benannt sein!

## 🛠️ Aufbau, Verdrahtung & Dokumentation

Eine detaillierte Anleitung gibt es auf meiner Website unter **[www.circus-of-science.de/erschreck-o-mat](https://www.circus-of-science.de/erschreck-o-mat)**.

Bilder und Schaltplan sind im Ordner [`/docs`](./docs).  

---
# Software

## 🧪 Sketch

Der Arduino-Sketch liegt hier [`erschreck-o-mat.ino`](./erschreck-o-mat.ino).

## 🔌 Dependencies

Dieses Projekt benötigt die folgende zusätzliche Arduino-Bibliothek:
`SoftwareSerial.h` – wird benötigt, um mit dem DFPlayer Mini über serielle Kommunikation zu kommunizieren.

Die Bibliothek ist in der Arduino-IDE standardmäßig enthalten. Falls nicht, kann sie über den Bibliotheksverwalter installiert werden.

## ⚙️ Programmablauf

1. Beim Einschalten wird der DFPlayer Mini initialisiert.
2. Anschließend wird der Startsound `0000.mp3` abgespielt (als akustischer 3-Sekunden-Countdown zum Verstecken der Box).
3. Danach misst der HC-SR04 den Abstand zum nächstgelegenen Hindernis vor dem Sensor und speichert diesen als Referenzwert.
4. Wenn sich der Abstand ab diesem Zeitpunkt verändert (z. B. durch eine Bewegung), wird der Erschreck-Sound `0001.mp3` abgespielt.
5. Nach dem Abspielen macht das Gerät nichts mehr. Neustart durch aus- und wieder einschalten.

## 🛠️ Einstellbare Variablen

`triggerAbstand` (Zeile 53) → Ab welcher Abstandsänderung soll der Sound losgehen? (in cm, Werte von `1` bis `100`)  
`soundVolume` (Zeile 54) → Wie laut soll der Sound abgespielt werden? (Werte von `0` bis `30`)

---
# Rechtliches

## ⚠️ Haftungsausschluss

Dieses Projekt dient ausschließlich zu Bildungs- und Demonstrationszwecken.  
Der Nachbau erfolgt **auf eigene Gefahr**. Es wird keine Haftung übernommen für Schäden an Personen, Tieren, Gegenständen oder Daten.  
Arbeiten an elektronischen Bauteilen sollten **nur mit entsprechenden Fachkenntnissen** durchgeführt werden.

## 📄 Lizenz

Dieses Projekt steht unter der **MIT License**.  
Details in der Datei [`LICENSE`](./LICENSE) oder am Anfang des Arduino-Sketches.

---

Viel Spaß beim Nachbauen und Erschrecken!   
*Jack Pop*
