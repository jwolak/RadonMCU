# NiosLoggerDemo

Minimalna aplikacja pokazujaca, jak uzywac biblioteki EquiniosLogger na Nios II.

## Co pokazuje demo

- konfiguracje poziomu logowania,
- ustawienie provider-a timestamp,
- ustawienie interwalu `log_process`,
- cykliczne logowanie przez `LOGD/LOGW/LOGE` i oproznianie bufora.

## Wymagania

- Nios II EDS w PATH (`nios2-app-generate-makefile`, `nios2-elf-gcc`, `make`),
- ustawione `SOPC_KIT_NIOS2`,
- wygenerowany BSP (domyslnie `SoftwareExamplesForRadonMCU/RadonHelloWorld_bsp`),
- zbudowany projekt FPGA (`.sof`) dla docelowego systemu.

## Szybki start

Z katalogu tego przykładu:

```bash
./create-this-app
```

Skrypt:

1. buduje `../../libEquiniosLogger.a`,
2. generuje Makefile aplikacji,
3. buduje plik ELF.

## Uruchomienie na plytce

```bash
nios2-configure-sof <path-to-sof>
nios2-terminal
make download-elf
```

Po uruchomieniu w terminalu JTAG zobaczysz okresowe komunikaty loggera.

## Przydatne zmienne

Mozesz nadpisac domyslne sciezki przed uruchomieniem skryptu:

- `BSP_DIR` - katalog BSP,
- `QUARTUS_PROJECT_DIR` - katalog projektu Quartus,
- `ELF_NAME` - nazwa pliku wynikowego ELF.
