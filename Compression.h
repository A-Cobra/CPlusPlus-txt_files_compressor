#pragma once
#include<iostream>
#include<unordered_map>
#include<queue>
#include<fstream>
#include<conio.h>
#include<string>
#include<cmath>


#include "Node.h"
#include "Comparison.h"
#include "Character.h"
using namespace std;

class Compressor
{
private:
	int numberOfCharacters;
	int zerosAdded;
	int bitNumber;
	int charNumber;
	int index;

	unordered_map<char, int> iterationsMap;
	unordered_map<char, string> huffmanCodes;
	string text;
	string fileName;
	string compressedText;
	string compressedCharText;
	Characters* charactersData;
	priority_queue<Node*, vector<Node*>, comparison> priorityQueue;

public:
	Compressor();
	~Compressor();

	void compressFile();
	void generateCodes(Node* headCopy, string code, unordered_map<char, string>& HuffmanCodes);
	priority_queue<Node*/*Elemento del queue*/, vector<Node*>/*Contenedor de los elementos del queu*/, comparison>&
	createTree(priority_queue<Node*, vector<Node*>, comparison>& priorityQueue, unordered_map<char, int>& iterationsMap,
	unordered_map<char, string>& huffmanCodes);
	void writeHuffmanTree(Characters* charactersData, int numberOfCharacters, string fileName);
	void writeCompressedText();
	void readFile(string& text, string& fileName);
	Characters* countIterations(unordered_map<char, int>& iterationsMap, string text, Characters* charactersData);
	Characters* storeCodes(unordered_map<char, string> huffmanCodes, Characters* charactersData);
	void getCompressedText();
	void binaryToChar();
	int binaryIntToInt(int bitNumber);



};

Compressor::Compressor()
{
	

}

Compressor::~Compressor()
{
}

void Compressor::compressFile()
{
	text = "";
	fileName = "";
	compressedText = "";
	compressedCharText = "";
	zerosAdded = 0;
	bitNumber = 0;
	charNumber = 0;

	readFile(text, fileName);
	charactersData = countIterations(iterationsMap, text, charactersData);//We get the iterations
	numberOfCharacters = iterationsMap.size();

	/*cout << endl << endl << endl << endl << "El texto original es " << endl << text << endl;*/


	priorityQueue = createTree(priorityQueue, iterationsMap, huffmanCodes);
	charactersData = storeCodes(huffmanCodes, charactersData);//We get the codes into the huffmanCode to be 
	//written in the .csv
	getCompressedText();
	zerosAdded = 8 - (compressedText.length() % 8);
	
	
	
	writeHuffmanTree(charactersData, numberOfCharacters, fileName);//Generate a .csv
	writeCompressedText();//Generate a .txt that contains the compressed code


	cout << "Comprimiendo..." << endl;
	cout << "Presione una tecla para continuar" << endl;
	_getch();
}


//Utilidades

void Compressor::generateCodes(Node* headCopy, string code, unordered_map<char, string>& HuffmanCodes) {
	if (headCopy == nullptr) {
		return;
	}
	if (!headCopy->left && !headCopy->right) {
		HuffmanCodes[headCopy->character] = code;
	}
	generateCodes(headCopy->left, code + "0", HuffmanCodes);
	generateCodes(headCopy->right, code + "1", HuffmanCodes);
}



priority_queue<Node*/*Elemento del queue*/, vector<Node*>/*Contenedor de los elementos del queu*/, comparison>&
Compressor::createTree(priority_queue<Node*, vector<Node*>, comparison>& priorityQueue, unordered_map<char, int>& iterationsMap,
unordered_map<char, string>& huffmanCodes) {

	//Agregamos elementos a la lista de prioridad
	//Ordenándolos de menor iteracion a mayor
	for (auto pair : iterationsMap) {
		priorityQueue.push(setAndGetNode(pair.first, pair.second, nullptr, nullptr));
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
	Node* root = priorityQueue.top();

	//Generamos los códigos
	generateCodes(root, "", huffmanCodes);
	return priorityQueue;
}



void Compressor::writeHuffmanTree(Characters* charactersData, int numberOfCharacters, string fileName) {
	ofstream file;
	file.open((fileName + "CompData.csv").c_str(), ios::out);

	if (file.fail()) {
		cout << "El archivo no pudo abrirse" << endl;
		_getch();
		exit(0);

	}

	file << zerosAdded << endl;

	for (int i = 0; i < numberOfCharacters; i++)
	{
		//file << charactersData[i].value << "," << /*charactersData[i].iterations*/charactersData[i].bitesNumber << "," << charactersData[i].code << endl;
		file << charactersData[i].value << "," << charactersData[i].iterations /*<< charactersData[i].bitesNumber << "," << charactersData[i].code */ << endl;
	}

	file.close();

}



void Compressor::writeCompressedText() {
	ofstream file;
	file.open((fileName + "Comp.txt").c_str(), ios::out);

	if (file.fail()) {
		cout << "El archivo no pudo abrirse" << endl;
		_getch();
		exit(0);

	}

	//Convertir a chars
	binaryToChar();
	file << compressedCharText;

	file.close();





}




void Compressor::readFile(string& text, string& fileName) {

	ifstream file;
	//string fileName;

	cout << "Busqueda de un archivo de texto para su lectura.\n";
	cout << "Ingrese el nombre o ubicacion del archivo sin incluir el .txt: " << endl;
	getline(cin, fileName);
	//fileName += ".txt";

	file.open((fileName + ".txt").c_str(), ios::in);

	if (file.fail()) {
		cout << "\nNo se pudo abrir el archivo.";
		_getch();
		exit(1);
	}
	cout << "\n";
	string auxText2 = "";
	while (!file.eof()) {
		getline(file, text);
		auxText2 += text;
		//cout << text << endl;
	}
	text = auxText2;
	file.close();

}


Characters* Compressor::countIterations(unordered_map<char, int>& iterationsMap, string text, Characters* charactersData) {


	int i = 0;


	//Llenamos el mapa al contar los caracteres
	for (char character : text)
	{
		iterationsMap[character] ++;
	}

	//Separamos espacio para el arreglo de Caracteres
	charactersData = new Characters[iterationsMap.size()];


	//Llenamos el arreglo de vectores
	for_each(iterationsMap.begin(), iterationsMap.end(),
		[&](std::pair<char, int> key_value)
		{
			charactersData[i].value = key_value.first;
			charactersData[i].iterations = key_value.second;
			i++;
		});

	return charactersData;
}




//Guarda los códigos en el arreglo de Caracteres
Characters* Compressor::storeCodes(unordered_map<char, string> huffmanCodes, Characters* charactersData) {


	int i = 0;




	for_each(huffmanCodes.begin(), huffmanCodes.end(),
		[&](std::pair<char, string> key_value)
		{
			charactersData[i].code = key_value.second;
			charactersData[i].bitesNumber = key_value.second.length();
			i++;
		});



	/*for (auto element:iterationsMap)
	{
		charactersData[i].value = element.first;
		charactersData[i].iterations = element.second;
		i++;
	}*/
	return charactersData;
}




void Compressor::getCompressedText() {
	for (char character : text) {
		compressedText += huffmanCodes[character];
	}
}


void Compressor::binaryToChar() {
	



	//Añadimos tantos "0"s como hagan falta para completar el byte
	/*cout << "Se deben agregar " << zerosAdded << " 0s" << endl;*/


	//Se agrega el número extra de bits agregados para que sea multiplo de 8
	for (size_t i = 0; i < zerosAdded; i++)
	{
		compressedText += "0";
	}

	index = 0;
	for (int i = 0; i <compressedText.length();i += 8) {
		bitNumber = stoi(compressedText.substr(i,8));
		compressedCharText += (char)binaryIntToInt(bitNumber);
	}






}

int Compressor::binaryIntToInt(int bitNumberCopy) {
	/*cout << "El número binario es " << bitNumberCopy;*/
	int addition = 0;
	int index2;

	int divisor;
	while (bitNumberCopy >= 10)	{
		//Obtenemos un índice para usarlo como exponente
				//cout << endl << "iiiiiiii" << endl;
		index2 = log10(bitNumberCopy);
				//cout << endl << "El valor de index2 es : " << index2 << endl;
		addition += pow(2, index2);
				//cout << endl << "El valor de addition es : " << addition << endl;
				//cout << "El valor de bitnumbercopy antes de modificarlo es ";
		divisor = pow(10, index2);
		bitNumberCopy = bitNumberCopy % divisor;

	}

	if (bitNumberCopy == 1) {
		addition += 1;
	}

	/*cout << " El número decimal es :" << addition << endl;*/
	return addition;
}