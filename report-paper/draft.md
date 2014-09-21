<!-- vim: set ts=4 sw=4: --> 
Konzepte des zugrundeliegenden (Anwendungs)-Modells


Parallelisierungsschema (Kommunikationsmuster, Verteilung der Daten & Aufgaben)

Es sollte mit MPI parallelisiert werden (optional: Shared-Memory Parallelisierung mit Threads oder OpenMP).

Leistungsanalyse des sequentiellen Codes (Verhält sich dieser Erwartungskonform)

Skalierungsverhalten der parallelen Version

Speedup-Diagramme

Potentiell Analyse mit Vampir/Sunshot

Durchführung einer Optimierung der parallelen Version (Kommunikationsschema etc.)


---
# Parallelisierungsschema
Das Feld wird implementiert durch ein 2D-Array of struct Ideas.

```c
#define malloc_idea_matrix(name)                              \
    Idea **name = (Idea **)malloc(num_rows * sizeof(Idea *)); \
    for (int i = 0; i < num_rows; ++i)                        \
        name[i] = (Idea *)malloc(num_cols * sizeof(Idea));    \
```

Es werden *zwei* Felder erstellt:

```c
malloc_idea_matrix(field)
malloc_idea_matrix(field_new)
```

`field` wird daraufhin mit leeren Ideen initialisiert und an zufaelligen Positionen mit Ideen bespawned. Der Inhalt von `field` wird dann in `field_new` kopiert. Man braucht zwei Felder, damit eine Idee nur ein mal pro Runde ziehen kann: es wird über den Inhalt von `field` iteriert, die gezogenen Ideen werden in `field_new` reingeschrieben. Wenn dannzum Beispiel eine Idee nach rechts zieht, wird sie im naechsten Iterationsschritt nicht noch einmal zum Zuge kommen. Kopieren ist folgendermaßen implementiert:

```c
#define copy_field_into_field_new()        \
    for_every(i, num_rows, {               \
        for_every(j, num_cols, {           \
            field_new[i][j] = field[i][j]; \
          });                              \
    });
```

Wir haben hier erst versucht, mittels `memcpy` das ganze effizienter zu gestalten, nur um irgendwann festzustellen, dass das mit derartigen 2D-Arrays nicht funktioniert, da ja  bei memcpy nur die Pointer, die auf eine row des Arrays verweisen, kopiert werden, was wir nicht wollen, da die Ideen by value kopiert werden müssen.

Die Aufteilung dieses Feldes auf die MPI-Prozesse funktioniert nun per horizontaler Spaltung des Feldes. Das hat den Vorteil, dass nur an zwei Kanten kommuniziert werden muss, oben und unten. An den Raendern gibt es "ghost rows": Nach unten hin betrachtet wird die letzte "reale" row die "ghost row" des naechsten ranks, und wird in dessen Feld als zusaetzliche Reihe über dessen erster "real row" repraesentiert. Die Kommunikation besteht also im Austauschen der eigenen real und ghost rows mit den drunter und drüber liegenden Nachbarn. 

![](pics/real-ghost-rows.jpg)

Dies ermöglicht, dass jeder Prozess seine Züge unabhaengig von den anderen Prozessen machen kann: wenn eine Idee auf einer "real row" nach oben respektive unten zieht, so hat sie die Informationen, was sich in der Zielzelle, die ja eigentlich vom benachbarten Prozess bearbeitet wird, befindet. 

Der Ablauf der MPI-Kommunikation/
