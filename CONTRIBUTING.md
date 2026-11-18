# Jak přispívat

Díky za zájem o projekt! Chyby a nápady hlaste přes [Issues](https://github.com/Bojacnik/ivs-projekt-2/issues),
změny posílejte jako pull request.

## Příprava prostředí

Závislosti a příkazy pro sestavení jsou v [README](README.md#sestavení-ze-zdrojových-kódů).
Pro kontrolu formátu je potřeba `clang-format-18`.

## Postup

1. Vytvořte větev z `master`.
2. Udělejte změnu a přidejte k ní testy:
   - matematika → `tests/operation_tests.c`
   - chování kalkulačky (tlačítka, klávesy) → `tests/calculator_tests.c`
3. Před odesláním spusťte:
   ```sh
   make format          # naformátuje kód
   make test            # testy
   make SANITIZE=1 test # testy s AddressSanitizer + UBSan
   ```
4. Zapište změnu do sekce `[Unreleased]` v [CHANGELOG.md](CHANGELOG.md).
5. Otevřete pull request. CI musí projít (formát, testy, sestavení balíčku).

## Architektura

- `src/backend` – matematická knihovna nad MPFR. Funkce berou a vracejí čísla jako řetězce;
  volající výsledek uvolní `free()`. Chyba = `OP_ERROR`, nedostatek paměti = `NULL`
  (`op_is_error()` pozná obojí).
- `src/calculator` – stavový automat kalkulačky, **bez závislosti na GUI**. Veškerá logika patří sem,
  aby ji šlo testovat.
- `src/frontend` – pouze kreslení a převod tlačítek/kláves na `CalcAction`.

## Styl kódu

- C11 (`-std=gnu11`), kompilace bez varování s `-Wall -Wextra`.
- Formát určuje `.clang-format` (taby, 120 sloupců); CI ho kontroluje.
- Veřejné funkce dokumentujte komentáři pro Doxygen (`/** @brief ... */`).
- Commity: krátký první řádek v rozkazovacím způsobu (např. „Fix rounding of small numbers“), detaily v těle.

## Vydání verze

Viz [README](README.md#vydání-nové-verze).
