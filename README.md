### dependencies
- see `Dockerfile`, or install docker and [fig](fig.sh/install.html) and run `fig run main bash`.

### run
`make` from repo root


### helper macros
- in `lib/macros.h`


### code structure
- main.c is der startpunkt, logischerweise ;). 
- logic.c enthaelt die logik zum bewegen und kommunizieren der ideen 
- idea.c enthaelt die definition des struct idea und die konstruktoren.
- lib/helpers.c enthaelt ein par helper funktionen...;)
- lib/macros.h enthaelt gerade ziemlich viel, koennte/sollte man noch aufsplitten
- allheads.h enthaelt header, die in jeder source file verfügbar sind.
- constants.h enthaelt konstanten hahaha. 
