Prostredi
---------

Linux - Manjaro 64bit (GLIBC 2.38 >=)

Autori
------

Cyberdyne Systems
- xnemecv00 Vít Němec
- xnovype00 Petr Nový
- xdrdlat00 Tomáš Drdla
- xmandym00 Michal Mandys

Licence
-------
This program is distributed under a [GNU General Public License v3.0](LICENSE).

# ivs-projekt-2
Společný projekt "kalkulačka" jako druhý projekt předmětu IVS

Struktura repozitáře
--------------------

```
.
├── src/
│   ├── backend/        # matematická knihovna (operation.c/.h)
│   ├── frontend/       # GUI kalkulačky (raylib/raygui)
│   └── profiling/      # program pro profiling (směrodatná odchylka)
├── tests/              # testy matematické knihovny
├── profiling/          # vstupní data a výstupy profilingu, zpráva
├── packaging/          # .desktop soubor a ikona pro instalátor
├── install/            # sestavené .deb instalátory
├── docs/
│   ├── images/         # screenshot.png, debugging.png
│   ├── mockup/         # návrh GUI
│   ├── project/        # plán, hodnocení, odevzdání, skutečnost
│   ├── doc.pdf         # programátorská dokumentace (Doxygen)
│   └── UzivatelskaDokumentace.pdf
├── Makefile            # sestavení, testy, profiling, dokumentace
├── CMakeLists.txt      # balení .deb (CPack)
└── Doxyfile
```

Sestavení
---------

Všechny příkazy se spouští z kořene repozitáře:

- `make` – stáhne závislosti (raylib, raygui), sestaví a spustí kalkulačku (`build/calc`)
- `make test` – sestaví a spustí testy matematické knihovny
- `make prof` – sestaví profiling a vygeneruje `profiling/vystup.txt`
- `make doc` – vygeneruje dokumentaci do `docs/doc.pdf`
- `make pack` – vytvoří archiv k odevzdání
- `make clean` – smaže build výstupy a stažené závislosti
