# SpeculazioniC

Raccolta di progetti di esempio in C realizzati durante corsi di formazione.

## Progetti

### 🖼️ bitmap_handling
Gestione e disegno su bitmap a 24 bit.
Implementazione da zero della lettura, scrittura e manipolazione di file `.bmp`,
con supporto al disegno pixel-level su immagini RGB.

### 📝 dynamic_strings
Libreria per la gestione di stringhe dinamiche in C.
Implementazione di un tipo stringa con allocazione dinamica della memoria,
operazioni di concatenazione, copia e ridimensionamento automatico del buffer.

### 📚 stack_queue
Implementazione di stack e code con relativo expression parser.
Include due approcci alla valutazione di espressioni matematiche:
- **Shunting Yard** (algoritmo di Dijkstra): conversione da notazione infissa a RPN
- **Parser classico**: parsing ricorsivo discendente

### ⋮ points
Gestione di una lista dinamica di punti con pattern visit e riduzione.

## Requisiti

- GCC con supporto C17 (`-std=c17`)
- Make

## Compilazione

Ogni progetto dispone di un Makefile comune nella root. Per compilare:
```bash
make OUT_DIR=<nome_progetto>
# esempio:
make OUT_DIR=stack_queue
```
