#include <fmt/core.h>
#include <string>
#include <iostream>

#include "lang/parser.hpp"
#include "lang/interpreter.hpp"

/*
 * Baza danych
 * s29441 Adam Skorupski 12c
 * PJATK 2024
 * --------
 * Program obsługuje się poprzez język wymyślony specjalnie pod projekt.
 * Korzysta się z niego w podobny sposób w jaki działają pipeliny w bashu, tzn. większość operacji zwraca
 * ten sam rezultat, ktory mozna przekazac do nastepnej komendy.
 * W ten sposób można w jednej linijce np. stworzyc tabele, dodac kolumne, wiersz itp.
 * Polecenia należy oddzielać od siebie spacją, np.:
 *   get_table(pracownicy,dzial) select(pracownicy.id,nazwisko,dzial.nazwa) where(dzial_id==dzial.id) print()
 * --------
 * Niektóre polecenia mają aliasy zaczerpnięte z SQLa
 * --------
 * Polecenia:
 *  - quit() - wyjdź z programu (alias: exit())
 *  - create_db(plik) - tworzy nową bazę danych z ustawioną scieżką do pliku
 *  - connect_db(plik) - wczytuje baze/polecenia z podanego pliku
 *  - save_db() - zapisuje bazę danych
 *  - export_db(plik) - eksportuje bazę danych do podanej ścieżki
 *  - get_table(tablica1, tablica2, ...) - pobiera podane tablice w argumentach (alias: from(...))
 *  - create_table(nazwa) - tworzy nową tablice w bazie z podaną nazwą
 *
 *  Poniższe polecenia wymagają przed nimi polecenia get_table albo create_table
 *  - add_column(nazwa, typ, nullable) - dodaje kolumnę
 *    nazwa - string
 *    typ - int/string/null
 *    nullable - jeśli kolumna może mieć nulle, należy wpisać nullable, w przeciwnym wypadku pominąć
 *  - get_column(nazwa) - pobiera kolumnę
 *  - add_row(...) - dodaje wiersze do tabeli - należy podać wartości w argumencie w kolejności takiej,
 *      w jakiej zostały utworzone kolumny (alias: insert(...))
 *  - select(kolumna1, kolumna2, ...) - filtruje kolumny
 *  - where(...) - dodaje warunek do operacji, który muszą spełnić wiersze, by były zawarte w rezultacie
 *     - język obsługuje jedynie proste porównania binarne i operatory: > >= == <= < !=
 *     - where() równoważy where(1==1) - tym sposobem można np. kasować wszystkie wiersze w rezultacie
 *  - sort_by(kolumna, asc/desc) - sortuje wiersze po wartości z kolumny (alias: order_by)
 *     - można pominąć drugi argument asc/desc - wtedy będzie sortować rosnąco
 *     - asc - rosnąco, desc - malejąco
 *
 *  Poniższe polecenia wymagają przed nimi jakiegokolwiek polecenia get_ lub create_table:
 *  - info() - podaje informacje o wyznaczonym obiekcie (baza danych, kolumna, rezultat)
 *  - print() - wyświetla w konsoli rezultat operacji w formie tabeli
 *  - rename(nowa_nazwa) - zmienia nazwę wyznaczonego obiektu na podaną w argumencie (tablica/kolumna)
 *  - remove() - kasuje wyznaczone obiekty (tablica/kolumna/wiersze) (alias: drop)
 *     - uwaga - można kasować wiersze tylko z jednej tabeli na raz
 *
 *  Poniższe polecenia wymagają polecenia get_table/create_table oraz warunku w where(...)
 *  - update(kolumna, wartość) - aktualizuje daną kolumnę nową wartością we wszystkich wierszach,
 *      które spełniają warunki
 *
 * --------
 * UWAGI:
 *  - jeżeli chcemy wpisać wartość Null do bazy, należy wpisać NULL (koniecznie z dużych liter)
 *  - program z automatu nie wypisuje wyników DQL - należy uzywać polecenia print()
 *  - porównywanie nulli jest bardzo prymitywne
 *  - aby podać nazwę kolumny z konkretnej tabeli, należy użyć składni tabela.kolumna
 * */

auto main() -> int {
    fmt::println("Welcome to my database project in C++");
    fmt::println("Author: Adam Skorupski s29441");
    auto interpreter = Interpreter();

    while (interpreter.isRunning()) {
        auto prompt = std::string();
        fmt::print(">");
        std::getline(std::cin, prompt);

        try {
            auto parser = parser::Parser(prompt);
            auto result = parser.produceAST();

            interpreter.runAST(result);
        } catch (std::string& message) {
            fmt::println("{}", message);
        }
    }
}
