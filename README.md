# Projekt z grafiki

Projekt z grafiki komputerowej 2018/2019 AGH WFIIS IS

Temat projektu: [Morfologia terenu](temat.pdf)

## Kompilacja

Zalecane środowisko: Visual Studio 2017

Wykorzystywane biblioteki
- SFML 2.5.1

#### Pliki nagłówkowe
Folder _headers_ należy dodać jako *Additional Include Directories*

#### Wszystkie pliki *.hpp *.cpp należy dodać do projektu (np. przeciągając)

#### Folder resurce
Należy skopiować do folderu z projektem  
  
Dodatkowo (opcjonalnie) można utworzyć plik *config.ini* podając np. rozmiary ekranu
```
width: 1400
height: 800
```
#### Informacje dodatkowe
W pliku Main.cpp linijki:
```
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

...

_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
```
służą w *Visual Studio* do rejestrowania wycieków pamięci w trybie *Debug*
## Autorzy

Piotr Litwin  
Paweł Krężel  
Michał Pabjan  
