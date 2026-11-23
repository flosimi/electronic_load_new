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
6. Verificare siguranță: cu sarcina ON, apasă butonul encoderului → sarcina se oprește (`OFF`).

## 4. Test cu aplicația desktop (`headless-app`)
1. Fișier: `desktop_app/release/windows-64bit/electronic_load_control_app.exe`
2. Alege portul COM al plăcii, viteza **9600**, conectează-te.
3. Din aplicație: setează curentul (ex. 0.5 A), apoi apasă butonul **Load ON/OFF**.
4. Verifică: tensiune, curent, temperatură și grafic se actualizează.
5. Tensiunea de oprire (`cutoff`) este 0 V la pornire = nu se oprește singur. O poți seta din aplicație.
6. Aplicația setează doar curent constant (nu are CP/CR).
7. Dacă vrei să vezi datele brute: `pio device monitor -b 9600` arată linii `m;mV;mA;temp;cutoff;curentSetat;on;timp;CRC`. Comenzi manuale: `o` (ON/OFF), `a0.5` + Enter (curent), `c3.0` + Enter (cutoff), `r` (reset).

## 5. Ce trebuie să observ
- [ ] La 0.5 A setat, ampermetrul arată ~0.5 A (nu ~4 A cum era înainte)
- [ ] Curentul se schimbă când schimb valoarea setată
- [ ] Tensiunea măsurată corespunde cu voltmetrul extern
- [ ] Butonul encoderului oprește sarcina
- [ ] Peste 50 V (limita + 1 V) sarcina se oprește singură
- [ ] La 400 W urmăresc temperatura (ventilatorul pornește de la 30 °C, la 45 °C merge la turație maximă). **Nu există oprire automată la supraîncălzire.**

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
- Varianta completă (`nanoatmega328`) este la 99.8% din flash, aproape plină.

## 8. Git
- Branch-ul vechi, neatins: `main`. Revenire: `git switch main`
- Trimite branch-ul pe GitHub: `git push -u origin headless-serial`
