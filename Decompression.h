#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<conio.h>
#include<vector>
#include<queue>
#include "Character.h"
#include "Node.h"
#include "Comparison.h"
using namespace std;



class Decompressor
{
private:

	string decompressedText;
	string encodedText;
	string bitText;
	string fileName;
	int zerosAdded;
	int counter;

	//Para iteraciones
	vector<Characters> huffmanIterations;
	//Para crear la lista de prioridad
	priority_queue<Node*, vector<Node*>, comparison> priorityQueue;
	Characters aux;
	//Nodo padre arbol
	Node* root;
public:
	Decompressor();
	~Decompressor();

	void readFile();
	void readBitText();
	void createHuffmanTree();
	void writeFile();
	void decode(Node* raiz, int& index, string str, string& deco);
	void readTree();
	void charsToBits();
	void intToBinary(int number);
	void readjustChars();
	void bytesToBinary(int decimalNumber);

};

Decompressor::Decompressor()
{
	decompressedText = "";
	fileName = "";
}

Decompressor::~Decompressor()
{
}


void Decompressor::readTree() {

	


	string text = "";//Para leer las lineas
	ifstream file;
	char comma = ',';//Para encontrar limites
	char caracter;
	int iterations;
	//string fileName;

	cout << "Busqueda de un archivo para descompresion.\n";
	cout << "Ingrese el nombre original del archivo sin incluir el .txt: " << endl;
	cout << "Ejemplo: Si usted comprimio un archivo con nombre \"Prueba\", le resultaron dos archivos con extensión Comp.txt y CompData.csv" << endl;
	cout << "Solo escriba Prueba" << endl;
	getline(cin, fileName);
	//fileName += ".txt";

	file.open((fileName + "CompData.csv").c_str(), ios::in);

	if (file.fail()) {
		cout << "\nNo se pudo abrir el archivo.";
		_getch();
		exit(1);
	}
	cout << "\n";


	string auxZerosAdded = "";
	getline(file, auxZerosAdded);//Leemos la primera línea que contendrá el número de 0s añadidos
	zerosAdded = stoi(auxZerosAdded);

	while (/*!file.eof()*/getline(file, text)) {
		//getline(file, text);


		stringstream stream(text);//Transformar en stream
		string caracterLeido = "";
		string iteracionLeida = "";
		getline(stream, caracterLeido, comma);//Leemos por comas
		getline(stream, iteracionLeida, comma);
		caracter = caracterLeido[0];


		stringstream degree(iteracionLeida);
		degree >> iterations;

		//cout << endl << endl << "Numero de iteraciones del caracter " << iterations + 1000 << endl;
		aux.value = caracter;
		aux.iterations = iterations;
		//aux.iterations = (int)stoi(iteracionLeida);

		huffmanIterations.push_back(aux);

		////cout << "Caracter: " <<caracter<<", iteración de caracter: "<<iterations << endl;
	}


	file.close();


}


void Decompressor::readFile()
{
	counter = 0;
	readTree();//Leer el arbol

	encodedText ="";//El de 0s y 1s	
	readBitText();


	createHuffmanTree();//Crear el arbol

	//charsToBits();//Convertir texto de bits a binario o Encoded

	//PRUEBASSSSS
	cout << "Descomprimiendo..." << endl;
	cout << "Presione una tecla para continuar" << endl;

	_getch();
	writeFile();


}

void Decompressor::createHuffmanTree()
{


	//Agregamos elementos a la lista de prioridad
	//Ordenándolos de menor iteracion a mayor
	for (Characters pair : huffmanIterations) {
		priorityQueue.push(setAndGetNode(pair.value, pair.iterations, nullptr, nullptr));
	}


	//Creamos el árbol al comparar y unir nodos
	//Hasta que haya un elemento en el queue, el cual es la raiz
	while (priorityQueue.size() != 1) {
		//Creamos un nodo que va a ser el izquierdo
		Node* left = priorityQueue.top();
		//Sacamos el elemento del queue
		priorityQueue.pop();
		//Creamos un nodo que va a ser el derecho
		Node* right = priorityQueue.top();
		//Sacamos el elemento del queue
		priorityQueue.pop();
		//Obtenemos las iteraciones del nuevo nodo
		int addition = left->iterations + right->iterations;
		//Creamos un nodo de esas características y loa ñadimos a la lista de prioridad segun la comparasion
		priorityQueue.push(setAndGetNode('\0', addition, left, right));

	}
	root = priorityQueue.top();

}






//JUST FOR TESTING
void Decompressor::readjustChars() {
	int auxint;
	for (int i = 0; i < bitText.length(); i++){
		//cout << "El caracter " << i + 1 << " y el numero de ansi: " << (int)bitText[i] << endl;
		if ((int)bitText[i] < 0) {
			//cout << "corrigiendo\t";
			auxint = (int)bitText[i];
			auxint += 256;
			//cout << auxint << "\t";
			//bitText[i] = (char)auxint;
			//bitText[i] = 'A';

		}
		else {
			auxint = (int)bitText[i];
		}
		intToBinary(auxint);

	}

	//For testing ONLY
	/*cout << "\n\n\n\nCaracteres coregidos\n\n\n\n\n\n\n";
	for (int i = 0; i < bitText.length(); i++) {
		cout << "El caracter " << i + 1 << " y el numero de ansi: " << (int)bitText[i] << endl;
		
	}*/





}





//SOLO PARA PROBAR

void Decompressor::intToBinary(int number) {
	//encodedText = "";
	for (int i = 7; i >= 0; i--) {
		if (number >= pow(2, i)) {
			encodedText += "1";//Escribimos en la cadena de texto encriptada un 1
			number -= pow(2, i);//Decrementamos el numero en 2^i unidades
		}
		else {
			encodedText += "0";//Escribimos en la cadena de texto encriptada un 0
		}
	}


}



void Decompressor::readBitText()
{
	//Obtenemos los bits
	bitText = "";
	fstream file;

	file.open((fileName + "Comp.txt").c_str(), ios::in);


	if (file.fail()) {
		cout << "\nNo se pudo abrir el archivo.";
		_getch();
		exit(1);
	}
	//cout << "\n";
	//char a = '\n';
	int counter2 = 1;
	string auxText = "";
	while (/*!file.eof()*/getline(file, bitText)) {//Obtener la linea
		//cout << endl << endl << "Vuelta " << counter2 << endl << endl;
		counter += bitText.length();
		readjustChars();//Reajustar numeros no enteros positivos 
	}


	encodedText = encodedText.substr(0, encodedText.length() - zerosAdded);

	


}





void Decompressor::writeFile()
{
	int index = -1;
	//cout << "\nEl texto decodificado es: \n";

	while (index < (int)encodedText.size() - 2) {
		decode(root, index, encodedText, decompressedText);
	}


	fstream file;

	file.open((fileName + "Decomp.txt").c_str(), ios::out);

	if (file.fail()) {
		cout << "\nNo se pudo abrir el archivo.";
		_getch();
		exit(1);
	}
	cout << "\n";
	file << decompressedText;


}

void Decompressor::decode(Node* root, int& index, string encodedText, string& decodedText)
{

	if (root == nullptr) {
		return;
	}
	if (!root->left && !root->right) {
		//cout << root->simbolo;
		decodedText += root->character;
		return;
	}
	index++;
	if (encodedText[index] == '0')
		decode(root->left, index, encodedText, decodedText);
	else
		decode(root->right, index, encodedText, decodedText);

	//cout << "Texto descomprimido: " << endl;
	//cout << decodedText;

}