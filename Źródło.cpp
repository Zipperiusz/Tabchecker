#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;
int incorrectLines,
tabsy = 0,
menu;
string nazwaPliku,
sciezka;
vector <int> errors;

vector<string> pliki;
string line;
fstream listaPlikow;
int chooseFile;
void whichFiles() {
    pliki.clear();
    cout << "Wybierz plik: \n";
    system("dir /n /b * > file_names.txt");
    
    listaPlikow.open("file_names.txt", ios::in);
    if (!listaPlikow.good()) {
        cout << "Something went wrong" << endl;
        exit(0);
    }
    while (getline(listaPlikow, line)) {
        if ((line[line.size() - 3] == 't') && (line[line.size() - 2] == 'x') && (line[line.size() - 1] == 't')) {
            if (line != "file_names.txt") pliki.push_back(line);
        }
    }
    listaPlikow.close();
    for (int i = 0; i < pliki.size(); i++) {
        cout << i + 1 << ". " << pliki[i] << endl;
    }
    cin >> chooseFile;
    //Obs³uga inputu
    while (!cin >> chooseFile) {
        cout << "Cyfre podaj, cyfre... \n";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> chooseFile;
    }
    nazwaPliku = pliki[chooseFile - 1];
}
void check() {
    fstream plik;
    //Wybierz który plik chcesz sprawdziæ
    whichFiles();
    //Otwórz plik
    plik.open(nazwaPliku, ios::in);
    if (!plik.good()) {
        cout << "Something went wrong" << endl;
        exit(0);
    }
    string linia;
    int licznik = 1;
    while (getline(plik, linia)) {
        //Przeszukaj wszystkie linijki i sprawdŸ czy liczba tabulatorów siê zgadza
        for (int i = 0; i < linia.length(); i++) {
            if (linia[i] == '	') tabsy++;
        }
        if ((tabsy != 7) || (linia[linia.length()-1]=='	')) {
            errors.push_back(licznik);
        }
        tabsy = 0;
        licznik++;
    }
    //SprawdŸ czy s¹ z³e linijki
    if (errors.size() == 0) {
        cout << "Wszystko jest w porzadku!\n";
    }
    else {
        cout << "W tych linijkach jest blad\n";
        for (int i = 0; i < errors.size(); i++) {
            cout << errors[i] << endl;
        }
        //Wyczyœæ listê b³êdów
        errors.clear();
    }
    plik.close();
}
void exportToFile() {
    whichFiles();
    fstream importedFile,
        exportedFile;
    string rawLine,
        polishText;    
    int lastTab;

    importedFile.open(nazwaPliku,ios::in);
    exportedFile.open(nazwaPliku.replace(nazwaPliku.size()-4,4,"_PL.txt"),ios::out);
    if (!importedFile.good() || !exportedFile.good()) {
        cout << "Something went wrong\n";
        exit(0);
    }

    while (getline(importedFile, rawLine)) {
        lastTab = rawLine.find_last_of('	');
        polishText = rawLine.substr(lastTab+1,rawLine.size()-1);
        exportedFile << polishText << endl;
    }
    importedFile.close();
    exportedFile.close();
}
void replaceTranslation() {
            fstream ab,
                translation,
                testtest;
            int lastTabTranslation,
                lastTabAb,
                firstTabTranslation,
                firstTabAb,
                translationCounter=1,
                abCounter = 1,
                vectorCounter=0;
            string abLine,
                translationLine,
                translationComparable,
                abComparable,
                textAtTheEndAb,
                textAtTheEndTranslation;
            vector<string>linesFromTranslation;
            translation.open("test.txt", ios::in);
            ab.open("tlumacztest.txt", ios::in | ios::out | ios::app);
            testtest.open("testtest.txt", ios::out);
            if ((!translation.good()) || (!ab.good())) exit(0);

            while (getline(translation, translationLine)) {
                
                firstTabTranslation = translationLine.find_first_of('	');
                lastTabTranslation = translationLine.find_last_of('	');
                textAtTheEndTranslation = translationLine.substr(lastTabTranslation + 1, translationLine.size() - 1);
                translationComparable = translationLine.substr(firstTabTranslation + 1, translationLine.size() - (textAtTheEndTranslation.size() + firstTabTranslation + 2));
                linesFromTranslation.push_back(translationComparable);
                //Test tekstu do porównania
               /* testtest << translationComparable << endl;*/
            }
            
            while (getline(ab, abLine)) {
                firstTabAb = abLine.find_first_of('	');
                lastTabAb = abLine.find_last_of('	');
                textAtTheEndAb = abLine.substr(lastTabAb + 1, abLine.size() - 1);
                abComparable = abLine.substr(firstTabAb + 1, abLine.size()- (textAtTheEndAb.size()+firstTabAb+2));
                if ((abComparable == linesFromTranslation[vectorCounter])&&(vectorCounter<linesFromTranslation.size())) {
                    testtest << abComparable << endl;
                    if (!(vectorCounter == linesFromTranslation.size()-1))vectorCounter++;
                }
                //Test tekstu do porównania
                /*testtest << abComparable << endl;*/
            }
            ab.close();
            translation.close();
            testtest.close();
}
int main() {    
    while (true) {
        cout << " Menu \n 1. Sprawdz tlumaczenie \n 2. Importuj tekst do pliku \n 3. Podmien tlumaczenie\n";
        char menu = _getch();
        switch (menu) {
        case '1':check();
            break;
        case '2': exportToFile();
            break;
        case '3': replaceTranslation();
            break;
        default: exit(0);
        }
    }

    return 0;
}
