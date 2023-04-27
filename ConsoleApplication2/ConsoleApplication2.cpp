#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <clocale>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cctype>

using namespace std;
bool spolgloska(wchar_t d) {
    if (d == 'b' || d == 'c' || d == 'd' || d == 'f' || d == 'g' || d == 'h' ||
        d == 'j' || d == 'k' || d == 'l' || d == 'm' || d == 'n' || d == 'p' || 
        d == 'r' || d == 's' || d == 't' || d == 'w' || d == 'x' || d == 'z' )
        return 1;
    else
        return 0;

}

bool is_vowel(wchar_t c) {
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y' )
        return 1;
    else
        return 0;

}

bool is_consonant(wchar_t c) {
    return !is_vowel(c);
}

void wczytajSlowa( string nazwa_pliku,  vector< string>& slowa) {
     ifstream plik(nazwa_pliku);
     string linia;
    while ( getline(plik, linia)) {
        slowa.push_back(linia);
    }
    plik.close();
}

 string losujHaslo(const  vector< string>& slowa) {
    if (slowa.empty()) {
        return "";
    }
    return slowa[ rand() % slowa.size()];
}

void spin_wheel() {
     vector<char> wheel_segments = { '(', 'O', ')' };
    int wheel_size = wheel_segments.size();
    int num_spins = 9;

    for (int i = 0; i < num_spins * wheel_size; ++i) {
         cout << "\r" << wheel_segments[i % wheel_size] <<  flush;
         this_thread::sleep_for( chrono::milliseconds(100));
    }
     cout << "\r" << wheel_segments[0] <<  endl;
}

struct Gracz {
     string imie;
    int punkty;
};

 vector<Gracz> gracze;

int main() {
     srand( time(nullptr));
    setlocale(LC_ALL, "pl_PL.UTF-8");

    char litera;
    int wylosowane_punkty;

     vector<string> slowa;

    wczytajSlowa("hasla.txt", slowa);

    int opcja = 0;
    while (opcja != 4) {

        cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
        cout << "Menu: \n";
        cout << "1. Graj \n";
        cout << "2. Dodaj/usun graczy \n";
        cout << "3. Lista graczy \n";
        cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
        cout << "Wybierz opcje: \n";
        cin >> opcja;
            if (opcja == 1) {
                if (gracze.empty()) {
                    cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                    cout << "\x1B[33mNie ma zadnych graczy, dodaj graczy przed rozpoczeciem gry.\033[0m\t\t\n";
                }
                else {
                    string haslo = losujHaslo(slowa);
                    if (haslo.empty()) {
                        cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                        cout << "\x1B[33mBrak hasel w pliku. Dodaj hasla do pliku przed rozpoczeciem gry.\033[0m\t\t\n";

                    }
                    else {

                        // Rozpoczęcie gry
                        string ukryte_haslo(haslo.size(), '*');
                        for (size_t i = 0; i < haslo.size(); ++i) {
                            if (haslo[i] == ' ') {
                                ukryte_haslo[i] = ' ';
                            }
                        }

                        int liczba_graczy = static_cast<int>(gracze.size());
                        int aktualny_gracz = 0;
                        bool gra_trwa = true;

                        while (gra_trwa) {
                            Gracz& gracz = gracze[aktualny_gracz];
                            cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                            cout << "Tura gracza " << gracz.imie << ". Masz " << gracz.punkty << " punktow.\n";
                            cout << "Ukryte haslo: " << ukryte_haslo << '\n';

                            int wybor;
                            cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                            cout << "1. Krec kolem \n";
                            cout << "2. Kup samogloske (500 punktow) \n";
                            cout << "3. Odgadnij haslo \n";
                            cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                            cout << "Wybierz opcje: ";
                            cin >> wybor;

                            if (wybor == 1) {
                                spin_wheel();
                                int wylosowane_punkty = 100 * (rand() % 50 + 1);
                                cout << "Wylosowano: " << wylosowane_punkty << " punktow.\n";

                                // Losowanie bankruta lub pominięcia kolejki
                                int szansa = rand() % 100;
                                if (szansa < 5) {
                                    cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                    cout << "\x1B[91mBankrut! Tracisz wszystkie punkty i konczysz turę.\033[0m\t\t\n";
                                    gracz.punkty = 0;
                                    aktualny_gracz = (aktualny_gracz + 1) % liczba_graczy;
                                    continue;
                                }
                                else if (szansa < 10) {
                                    cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                    cout << "\x1B[31mPominiecie kolejki. Nastepny gracz.\033[0m\t\t\n";
                                    aktualny_gracz = (aktualny_gracz + 1) % liczba_graczy;
                                    continue;
                                }

                                char trafione;

                                cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                cout << "Podaj spolgloske: ";
                                char zgadywana_litera;
                                cin >> zgadywana_litera;
                                if (spolgloska(zgadywana_litera)) {
                                    int found_count = 0;
                                    char lower_lit = tolower(static_cast<unsigned char>(zgadywana_litera));

                                    for (size_t i = 0; i < haslo.size(); ++i) {
                                        if (tolower(static_cast<unsigned char>(haslo[i])) == lower_lit && ukryte_haslo[i] == '*') {
                                            ukryte_haslo[i] = haslo[i];
                                            found_count++;
                                        }
                                    }

                                    if (found_count > 0) {
                                        int punkty = wylosowane_punkty * found_count;
                                        cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                        cout << "Dobrze! Litera " << zgadywana_litera << " wystepuje w hasle " << found_count << " razy!\n";
                                        cout << "Zdobywasz " << punkty << " punktow (mnożnik: x" << found_count << ").\n";
                                        gracz.punkty += punkty;
                                    }
                                    else {
                                        cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                        cout << "\x1B[31mNiestety, litera " << zgadywana_litera << " nie wystepuje w hasle.\033[0m\t\t\n";
                                    }
                                }

                            }
                            else if (wybor == 2) {
                                if (gracz.punkty >= 500) {
                                    cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                    cout << "Podaj samogloske: ";
                                    char zgadywana_litera;
                                    cin >> zgadywana_litera;
                                    if (is_vowel(zgadywana_litera)) {
                                        bool trafienie = true;
                                        for (size_t i = 0; i < haslo.size(); ++i) {
                                            if (tolower(haslo[i]) == tolower(zgadywana_litera) && ukryte_haslo[i] == '_') {
                                                ukryte_haslo[i] = haslo[i];
                                                trafienie = true;
                                            }
                                        } 

                                        if (trafienie) {
                                            cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                            cout << "\x1B[32mTrafiles! Tracisz 500 punktow za kupno samogloski.\033[0m\t\t\n";
                                            gracz.punkty -= 500;
                                        }
                                        else {
                                            cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                            cout << "\x1B[31mNie ma takiej samogloski w hasle. Nastepny gracz.\033[0m\t\t\n";
                                            aktualny_gracz = (aktualny_gracz + 1) % liczba_graczy;
                                        }
                                    }
                                    else {
                                        cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                        cout << "\x1B[31mTo nie jest samogloska. Następny gracz.\033[0m\t\t\n";
                                        aktualny_gracz = (aktualny_gracz + 1) % liczba_graczy;
                                    }
                                }
                                else {
                                    cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                    cout << "\x1B[31mNie masz wystarczajacej liczby punktow, aby kupic samogloske. Nastepny gracz.\033[0m\t\t\n";
                                    aktualny_gracz = (aktualny_gracz + 1) % liczba_graczy;
                                }
                            }
                            else if (wybor == 3) {
                                cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                cout << "Podaj haslo: ";
                                string zgadywane_haslo;
                                cin.ignore();
                                getline(cin, zgadywane_haslo);

                                string zgadywane_haslo_lower = zgadywane_haslo;
                                string haslo_lower = haslo;

                                transform(zgadywane_haslo_lower.begin(), zgadywane_haslo_lower.end(), zgadywane_haslo_lower.begin(), ::tolower);
                                transform(haslo_lower.begin(), haslo_lower.end(), haslo_lower.begin(), ::tolower);

                                if (zgadywane_haslo_lower == haslo_lower) {

                                    cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                    cout << "\x1B[32mGratulacje! Odgadles haslo:\033[0m\t\t " << haslo << "\n";
                                    gracz.punkty += 1000; // Przyznawanie punktów za odgadnięcie hasła
                                    gra_trwa = false;     // Kończenie gry
                                }
                                else {
                                    cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                    cout << "\x1B[31mNiestety, to nie jest prawidlowe haslo.\033[0m\t\t\n";
                                    aktualny_gracz = (aktualny_gracz + 1) % liczba_graczy; // Przejście do następnego gracza
                                }

                            }
                            else {
                                cout << "\x1B[36m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                                cout << "\x1B[31mNieprawidlowy wybor. Sprobuj ponownie.\033[0m\t\t\n";
                            }
                        }
                    }
                }

            }
    
        else if (opcja == 2) {
            // Dodawanie/usuwanie graczy
            int wybor;
             cout << "\x1B[96m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
             cout << "1. Dodaj gracza\n";
             cout << "2. Usun gracza\n";
             cout << "\x1B[96m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
             cout << "Wybierz opcje: ";
             cin >> wybor;

             if (wybor == 1 || wybor == 2)
             {
                 if (wybor == 1) {
                     string imie;
                     cout << "Podaj imie gracza: ";
                     cin >> imie;
                     gracze.push_back({ imie, 0 });
                     cout << "Gracz " << imie << "\x1B[92m zostal dodany.\033[0m\t\t\n";
                 }
                 else if (wybor == 2) {
                     int gracz_do_usuniecia;
                     cout << "Wybierz gracza do usuniecia:\n";
                     for (size_t i = 0; i < gracze.size(); ++i) {
                         cout << i + 1 << ". " << gracze[i].imie << '\n';
                     }
                     cout << "Wybierz numer gracza: ";
                     cin >> gracz_do_usuniecia;
                     gracz_do_usuniecia--;

                     string usuniety_gracz = gracze[gracz_do_usuniecia].imie;
                     gracze.erase(gracze.begin() + gracz_do_usuniecia);
                     cout << "Gracz " << usuniety_gracz << "\x1B[92m zostal usuniety.\033[0m\t\t\n";
                 }
                 else {
                     cout << "\x1B[96m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
                     cout << "\x1B[31mNieprawidlowy wybor. Sprobuj ponownie.\033[0m\t\t\n";
                 }
             }
             else
             {
                 return 0;
             }
        }
        else if (opcja == 2) {
             cout << "\x1B[96m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
             cout << "\x1B[92mLista graczy:\033[0m\t\t\n";
            for (size_t i = 0; i < gracze.size(); ++i) {
                 cout << i + 1 << "| " << gracze[i].imie << '\n';
            }
        }
        else {
             cout << "\x1B[96m- - - - - - - - - - - - - - - - - - - - - - - -\033[0m\t\t\n";
             cout << "\x1B[31mNieprawidlowy wybor. Sprobuj ponownie.\033[0m\t\t\n";
        }
    }

    return 0;
}
