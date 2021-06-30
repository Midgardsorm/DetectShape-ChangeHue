W katalogu "demo" znajduje się krótkie nagranie z działania aplikacji.

Kilka uwag do projektu:
 - projekt skompilowałem za pomocą MSVC2019 i uruchamiałem na systemie Widnows 10.
 - Wykorzystuje Qt6 (powinno działać od wersji  Qt 5.15, ale nie miałem już czasu tego sprawdzić)
 - wykorzystałem OpenCV w wersji 4.5.2 (którą skompilowałem również za pomocą MSVC2019)
 - okno obrazu jest natywne dla OpenCV, okno GUI stworzyłem w QML, z pomocą Qt.Quick, QtQuick.Controls, QtQuick.Layouts i Qt.labs.platform
 - pierwszy raz miałem styczność z OpenCV, więc podczas pracy korzystałem mocno z informacji zamieszczonych na StackOverflow i z kodu zamieszczonego tutaj: [GItHub: bsdnoobz](https://github.com/bsdnoobz/opencv-code/blob/master/shape-detect.cpp)

 Działanie programu
 1. Po uruchomieniu aplikacji naciskamy przycisk "Open file" i wybieramy plik *png
 2. Pojawi się obraz, następnie naciskamy przycisk "Detect shapes"
 3. Zostają wykryte figury i załadowane na listę w GUI. Ponad listą jest informacja o ilości wykrytych trójkątów, prostokątów, kół i elips.
 4. Od tego momentu można swobodnie zaznaczać kształt na liście lub bezpośrednio na obrazie.
 5. Suwakiem na dole zmieniamy wartość HUE dla zaznaczonego elementu.
 6. Kolor zaznaczenia zmieniamy klikając na kolorowy prostokąt poniżej napisu "Selection color (click below)". Pojawi się okno z wyborem koloru.

 Funkcjonalności których nie zrobiłem ze względu na czas. Zamieszczam również planowane podejście do zrobienia tych funkcjonalności:
 1. Przeniesienie wyświetlania z okna OpenCV do GUI w qml. Planowane podejście: zastosowałbmym QImageProvider i przechwytywał koordynaty po naciśnięciu myszy które przekazałbym do funkcjonalności zajmującej się zaznaczeniem elementów na planszy
 2. Odczytywanie innych formatów graficznych. Planowane podejście: dopisałbym inne dozwolone typy w fileDialog w qml i sprawdzał czy ładują się i czy są obsługiwane prawidłowo.
 3. Zapis aktywnego pliku. Planowane podejście: file dialog w qml posiada gotową funkcjonalność - wystarczyłoby w cpp napisać funkcję dostarczającą ścieżkę do zapisania pliku.
 4. Zaznaczanie grupowe kształtów. Planowane podejście: aktywny wybrany kontur zamieniłbym na wektor wybranych konturów (reszta cpp jest przygotowana już do obsługi wielu zaznaczonych konturów). W qml dopisałbym własną obsługę zaznaczania aktywnych indeksów.
 5. Zmniejszenie liczby zmiennych w klasie HueController. Planowane podejście: Zrobiłbym powiązane struktury które trzeba zarejestrować jako nowy typ by były widoczne i możliwe do użycia w qml.
 6. Usprawnienie wykrywania elipsy. Planowane podejście: Dobrym tropem wydaje się zastosowanie transformacji Hougha, która poza wykryciem linii pozwala na wykrywanie kół i elips - sprawdziłbym jak wygląda implementacja tego algorytmu w scikit-image. 



Zadanie wysłane przez formę OrbitVU:

# Witaj! 
Pierwszy krok już za Tobą! Poniżej znajduje się instrukcja opisująca funkcjonalność aplikacji, o której stworzenie chcielibyśmy Cię poprosić.

## Założenia aplikacji
1.  Aplikacja powinna umożliwiać wczytanie obrazu testowego z zadanej ścieżki w formacie  **png** (w repozytorium znajduje się przykładowy obraz testowy),
1.  Po wczytaniu obrazu aplikacja powinna obliczyć ilość figur znajdujących się na obrazku z podziałem na kształty (elipsy, trójkąty i prostokąty),
1.  Użytkownik powinien mieć możliwość wybrania dowolnej figury wykrytej w obrazie (wybrana figura powinna zostać zaznaczona konturem w dowolnym kolorze, którego zdefiniowanie powinno być możliwe w UI),
1.  Po wybraniu figury należy umożliwić użytkownikowi zmianę jego koloru (przesunięcie  **hue**).

## Podsumowanie
Przejrzystość i jakość wykonania zadania będzie miała decydujący wpływ. Oczywiście mile widziane, jeżeli aplikacja będzie zawierać intuicyjny interfejs graficzny stworzony np. za pomocą **QtWidgets**. Preferowana przez nas biblioteka do przetwarzania obrazów to **OpenCV**.