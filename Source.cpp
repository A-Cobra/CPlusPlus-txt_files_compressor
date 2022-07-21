#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<unordered_map>
#include<queue>
#include <iterator>
#include <algorithm>

#include "Character.h"
#include "Decompression.h"
#include "Node.h"
#include "Comparison.h"
#include "Compression.h"



using namespace std;


int main() {
	//setlocale(LC_ALL,"");

	Compressor* compressingMachine = new Compressor();
	Decompressor* decompressingMachine = new Decompressor();


	int option = -1;

	while (option!=3) {
		cout << "Ingrese una opcion" << endl;
		cout << "1. comprimir" << endl;
		cout << "2. descomprimir" << endl;
		cout << "3. Salir de la aplicacion" << endl;
		cin >> option;
		//_getch();
		system("cls");
		cin.ignore();

		if (option == 1) {
			compressingMachine->compressFile();

		} else if (option == 2) {
			decompressingMachine->readFile();


		}
		system("cls");

	
	}

}



