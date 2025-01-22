## Wie bekommt man es zum laufen?

- Lege einen /build Ordner im Projektordner an ->   mkdir build
- Navigiere in den /build Ordner ->                 cd build
- Generiere mithilfe von CMake ein build system ->  cmake ..
- Kompiliere das Program mit dem build system ->    cmake --build .

Jetzt kann man das entstandene Program so wie in denn Systemanforderungen
der Aufgabenstellung festgelegt ausführen. Die result_<eingabe_datei> Datei
befindet wird vom Program im build Ordner angelegt.

Übbrigens: Ein kleiner Hinweis, der das Korregieren eventuell erleichtert.
Die drei Klassen von Tries unterscheiden sich lediglich in der Implementierung der
Nodes, der Rest ist vollkommen identisch. 
Ich wollte diesen Teil eigentlich auch in die Tries Klasse, von der die anderen erben auslagern,
habe dabei aber Probleme bekommen, da diese Funktionen auf Node zugreifen müssen. Ich konnte das
leider nicht auflösen, da ich die Sprache nie gelernt habe und noch nicht richtig behersche.