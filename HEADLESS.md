# Pași pentru testul pe hardware (varianta fără LCD și tastatură)

Branch: `headless-serial`. Placă: 8 A / 50 V / 400 W, shunt 0.025 Ω.

## 0. Înainte de orice
- [ ] Sunt pe branch-ul bun: `git branch --show-current` → `headless-serial`
- [ ] Modificările sunt comise (`git status` curat). Dacă nu: `git add -A` și `git commit`
- [ ] **Primul test cu sursă limitată în curent** (ex. 5 V, limită 1 A), nu direct cu sursa mare. Relația DAC → curent s-a schimbat de ~6× față de codul vechi.
- [ ] Sarcina e **OFF** la pornire (firmware-ul o oprește la boot).

## 1. Mediile PlatformIO

| Mediu | Ce face | Upload |
|---|---|---|
| `nanoatmega328` | varianta completă, cu LCD + tastatură | USB (bootloader) |
| `nanoatmega328_icsp` | la fel, dar prin USBasp (șterge bootloader-ul) | ICSP |
| `headless` | fără LCD/tastatură, **terminal serial** | USB |
| `headless-app` | fără LCD/tastatură, **aplicația desktop** | USB |
| `simulide` | simulare | — |

Comandă de build pentru toate: `pio run`

## 2. Upload prin USB (headless)
1. Conectează placa prin USB (convertorul de la JP2).
2. Pentru terminal: `pio run -e headless -t upload`
3. Pentru aplicația desktop: `pio run -e headless-app -t upload`

> Dacă uploadul prin USB nu merge, vezi secțiunea 6 (ICSP).

## 3. Test cu terminal serial (`headless`)
1. `pio device monitor -b 9600`
2. Setează terminalul să trimită **newline** la sfârșit de linie.
3. Trebuie să apară `READY`.
4. Comenzi:
   - `HELP` → lista comenzilor
   - `MEAS?` → `M;tensiune;curent;putere;temperatură;on;mod;valoare_setată`
   - `CC 0.5` → curent constant 0.5 A (`OK CC 0.500`)
   - `CP <W>` → putere constantă, `CR <ohm>` → rezistență constantă
   - `ON` / `OFF`
5. Verificare: la `CC 0.5` + `ON`, ampermetrul extern arată ~0.5 A și `MEAS?` arată ~0.5 A.
6. Verificare siguranță: cu sarcina ON, apasă butonul encoderului → sarcina se oprește și apare `!OFF ENCODER`.
7. Liniile care încep cu `!` nu sunt răspunsuri la o comandă: `!OFF ENCODER` și `!FAULT <mască>` (vezi secțiunea 5).

## 4. Test cu aplicația desktop (`headless-app`)
1. Fișier: `desktop_app/release/windows-64bit/electronic_load_control_app.exe`
2. Alege portul COM al plăcii, viteza **9600**, conectează-te.
3. Din aplicație: setează curentul (ex. 0.5 A), apoi apasă butonul **Load ON/OFF**.
4. Verifică: tensiune, curent, temperatură și grafic se actualizează.
5. Tensiunea de oprire (`cutoff`) este 0 V la pornire = nu se oprește singur. O poți seta din aplicație.
6. Aplicația setează doar curent constant (nu are CP/CR).
7. Dacă vrei să vezi datele brute: `pio device monitor -b 9600` arată (de 5 ori pe secundă) linii `m;mV;mA;temp;cutoff;curentSetat;on;timp;CRC`. Comenzi manuale:
   - `o` → comută ON/OFF; `N` → ON; `F` → OFF (majuscule, idempotente)
   - `a0.5` + Enter → curent; `c3.0` + Enter → tensiune de oprire; `r` + Enter → reset capacitate
   - Comenzile cu valoare se aplică **doar după newline**; fără newline în 1 s sunt ignorate.
8. La o eroare apare o linie `!FAULT <mască>`; aplicația o ignoră, dar sarcina se oprește.
9. Capacitatea (mAh) se calculează acum prin integrare pe pași; timpul afișat vine tot din RTC și se resetează la 24 h.

## 5. Ce trebuie să observ
- [ ] La 0.5 A setat, ampermetrul arată ~0.5 A (nu ~4 A cum era înainte)
- [ ] Curentul se schimbă când schimb valoarea setată
- [ ] Tensiunea măsurată corespunde cu voltmetrul extern
- [ ] Butonul encoderului oprește sarcina
- [ ] Peste 50.5 V sarcina se oprește singură (`!FAULT 8`) și `ON` e refuzat până scade tensiunea
- [ ] Cu termistorul deconectat: ventilatorul pornește la maxim, sarcina nu pornește, `!FAULT 2`
- [ ] Peste 85 °C sarcina se oprește (`!FAULT 4`). Ventilatorul pornește de la 30 °C, la 45 °C merge la turație maximă.
- [ ] Dacă ADS1115 nu răspunde (cablu I2C scos): `!FAULT 1`, sarcina se oprește (înainte firmware-ul rămânea blocat)
- [ ] Dacă LCD-ul (varianta completă) arată caractere corupte sau apar `FAULT 1` sporadice, compilează cu `-DI2C_CLOCK_HZ=100000`

Măștile `!FAULT`: 1 = ADC nu răspunde, 2 = termistor, 4 = supratemperatură, 8 = supratensiune (se adună).

## 6. Dacă uploadul nu merge (ICSP / USBasp)
Firmware-ul nu poate ține micro-ul în reset; cauza e la hardware sau programator.
1. Alimentează placa la 5 V. **Deconectează convertorul USB de la JP2.** Jumperul USBasp pe 5 V (nu 3.3 V).
2. Test: `avrdude -c usbasp -p m328p -B 125kHz -v`
3. Trebuie să apară semnătura `0x1e950f`.
4. Dacă nu răspunde: verifică 5 V, jumperul USBasp și dacă oscilează cristalul de 16 MHz (poți injecta temporar un ceas pe XTAL1).
5. Fuse-uri corecte pentru cristal extern 16 MHz: lfuse `0xFF`, hfuse `0xD9` sau `0xDA`, efuse `0xFD` sau `0xFF`. **Nu le scrie fără să fii sigur.**
6. Upload: `pio run -e nanoatmega328_icsp -t upload` (șterge bootloader-ul; pentru USB trebuie reînscris).
7. Dacă nu detectează chipul, adaugă `-B` și `125kHz` în `upload_flags` din `platformio.ini`.

## 7. Limite și lucruri de știut
- Nu există mod cu tensiune constantă; tensiunea se citește, nu se setează.
- Dacă se pierde conexiunea USB cu sarcina ON, sarcina rămâne pornită. Oprire: comanda `OFF`, din aplicație, sau butonul encoderului.
- Calibrarea de pe panou este dezactivată (flash). EEPROM-ul gol folosește valori neutre.
- Varianta completă (`nanoatmega328`) este la ~98.8% din flash. Dependențele sunt fixate la versiunile instalate.
- Nu există watchdog: dacă firmware-ul se blochează din alt motiv, sarcina rămâne pornită.
- Protecția la supratensiune presupune că ADC-ul de tensiune se saturează la ~51.2 V; dacă firele de sense sunt deconectate, tensiunea citită e ~0 și limita de 400 W nu mai funcționează (de verificat în schemă).
- În varianta cu LCD, după o oprire prin eroare ecranul poate afișa încă „ON” până reintri în mod.

## 8. Git
- Branch-ul vechi, neatins: `main`. Revenire: `git switch main`
- Trimite branch-ul pe GitHub: `git push -u origin headless-serial`
