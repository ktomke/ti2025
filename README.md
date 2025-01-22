## Wie bekommt man es zum laufen?

- Lege einen /build Ordner im Projektordner an ->   mkdir build
- Navigiere in den /build Ordner ->                 cd build
- Generiere mithilfe von CMake ein build system ->  cmake ..
- Kompiliere das Program mit dem build system ->    cmake --build .

Jetzt kann man das entstandene Program so wie in denn Systemanforderungen
der Aufgabenstellung festgelegt ausführen. Die result_<eingabe_datei> Datei
befindet wird vom Program im build Ordner angelegt.