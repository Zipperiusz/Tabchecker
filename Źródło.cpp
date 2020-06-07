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

int findNthOccur(string str, char ch, int N) {
    int occur = 0;
    // Loop to find the Nth 
    // occurence of the character 
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ch) {
            occur += 1;
        }
        if (occur == N) return i;
    }
    return -1;
}
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
    //wyci¹gnij polskie t³umaczenie do osobnego pliku
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
                output;
            int lastTabTranslation,
                lastTabAb,
                thirdTabTranslation,
                thirdTabAb,
                vectorCounter=0,
                lineCounter=1;
            string abLine,
                translationLine,
                translationComparable,
                abComparable,
                textAtTheEndAb,
                textAtTheEndTranslation;
            bool everythingFine;
            vector<string>linesFromTranslation;
            vector<string>fullLinesReadyToPaste;
            //plik z t³umaczeniem
            whichFiles();
            translation.open(nazwaPliku, ios::in);
            //plik g³ówny ab_skryty_pl
            ab.open("ab_skrypty_pl.txt", ios::in);
            //plik wyjœciowy
            output.open("output.txt", ios::out);
            if ((!translation.good()) || (!ab.good())) exit(0);

            while (getline(translation, translationLine)) {                
                thirdTabTranslation = findNthOccur(translationLine, '	', 3);
                lastTabTranslation = translationLine.find_last_of('	');
                textAtTheEndTranslation = translationLine.substr(lastTabTranslation + 1, translationLine.size() - 1);
                translationComparable = translationLine.substr(thirdTabTranslation + 1, translationLine.size() - (textAtTheEndTranslation.size() + thirdTabTranslation + 2));
                linesFromTranslation.push_back(translationComparable);
                fullLinesReadyToPaste.push_back(translationLine);                
            }
            
            while (getline(ab, abLine)) {
                thirdTabAb = findNthOccur(abLine, '	', 3);
                lastTabAb = abLine.find_last_of('	');
                textAtTheEndAb = abLine.substr(lastTabAb + 1, abLine.size() - 1);
                //tekst do porównania z pliku g³ównego
                abComparable = abLine.substr(thirdTabAb + 1, abLine.size()- (textAtTheEndAb.size()+thirdTabAb+2));
                //podmieñ zawartoœæ je¿eli znajduje siê w pliku z t³umaczeniem, jeœli nie szukaj dalej zgodnych linijek, a linijki nienale¿¹ce do t³umaczenie wklej z powrotem
                if ((abComparable == linesFromTranslation[vectorCounter])&&(vectorCounter<linesFromTranslation.size())) {
                    output << fullLinesReadyToPaste[vectorCounter] << endl;
                    if (!(vectorCounter == linesFromTranslation.size()-1)) vectorCounter++;
                    lineCounter++;
                }
                else {
                    output << abLine << endl;
                }                
            }
            //sprawdŸ czy wszystkie linijki zosta³y podmienione
            if (vectorCounter == (linesFromTranslation.size() - 1)) {
                cout << "Git gut poszlo!\n";
                cout << "Podmienilem " << vectorCounter+1 << " linijek \n";
                everythingFine = true;
            }
            else {
                cout << "W tej linii cos jest nie tak: " << lineCounter << endl;
                everythingFine = false;
            }
            ab.close();
            translation.close();
            output.close();
            if (everythingFine) {
                remove("ab_skrypty_pl_old.txt");
                rename("ab_skrypty_pl.txt", "ab_skrypty_pl_old.txt");
                rename("output.txt", "ab_skrypty_pl.txt");
            }
}
int main() {    
    while (true) {
        cout << "======================================\n";
        cout << " Menu \n 1. Sprawdz tlumaczenie \n 2. Importuj tekst do pliku \n 3. Podmien tlumaczenie\n";
        cout << "======================================\n\n";
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
