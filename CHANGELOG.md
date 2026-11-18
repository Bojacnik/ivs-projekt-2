# Changelog

Všechny podstatné změny projektu. Formát vychází z [Keep a Changelog](https://keepachangelog.com/cs/1.1.0/),
verze odpovídají [Semantic Versioning](https://semver.org/lang/cs/).

## [Unreleased]

## [1.0.0] - 2026-11-18

### Přidáno
- Funkce `cos` a `tan`, přepínání radiány/stupně (ve stupních přesně: `sin 180° = 0`, `tan 90° = Err`).
- Řádek s rozpracovaným výpočtem nad displejem (`2 +`, `2 + 3 =`).
- Tlačítko `+/-`, kopírování a vkládání čísel (Ctrl+C / Ctrl+V), klávesové zkratky
  (`s c t r n d`), číslice z horní řady klávesnice; `Esc` maže místo zavření okna.
- Řetězení operací: `2 + 3 * 4 =` → `20`.
- Logika kalkulačky oddělená od GUI (`src/calculator`) a pokrytá testy, které nepotřebují displej.
- Testy chování při selhání alokace paměti.
- Balíček `.deb` sestavovaný v CI a vydávaný přes GitHub Releases, CI (testy, ASan/UBSan, formát kódu).
- Uživatelský návod `docs/navod.md`.

### Změněno
- Výsledky se zobrazují na 10 platných číslic bez zbytečných nul (`1000.1` místo `1.00010e+03`),
  interně se počítá s ~100 číslicemi.
- Pevné verze raylib 6.2 a raygui 5.0; okno je o řádek tlačítek vyšší.
- Struktura repozitáře, `Makefile` v kořeni, sestavení do `build/`.

### Opraveno
- Přesnost výpočtů byla potichu jen 53 bitů a docházelo k únikům paměti (dvojí inicializace MPFR).
- Malá čísla (např. `0.0000001`) se zobrazovala jako `0`.
- `1/0` zobrazovalo `inf`, neplatný vstup nebyl rozpoznán – nyní `Err`.
- Třetí odmocnina záporného čísla vracela NaN; faktoriál velkého čísla zamrzl a `2.5!` dával `2`.
- Zápis mimo pole při `Del` na prázdném vstupu a přetečení bufferu při dlouhém vstupu.
- Psaní číslice po výsledku ji připojilo k výsledku; `2 + 3 +` ztratilo `2`.
- Okno v GNOME (Wayland) nemělo titulek ani tlačítka zavřít/minimalizovat.
- Profiling: chybné načtení počtu (`"5l"`), zaokrouhlování mezivýsledků na 6 číslic, úniky paměti.
- Balíček: binárky nebyly spustitelné pro ostatní uživatele, závislosti uváděly `-dev` balíčky.

## [0.69.42] - 2024-04-25

Verze odevzdaná v rámci předmětu IVS.

[Unreleased]: https://github.com/Bojacnik/ivs-projekt-2/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/Bojacnik/ivs-projekt-2/releases/tag/v1.0.0
