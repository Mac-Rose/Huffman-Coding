#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <unordered_map>
using namespace std;

// A tree node
struct Node
{
	char ch;
	Node *left, *right;
};

Node *getNode(char ch, Node *left, Node *right)
{
	Node *node = new Node();

	node->ch = ch;
	node->left = left;
	node->right = right;

	return node;
}

void decode()
{
	string filename;
	cout << "\nEnter Name of Compressed file : ";
	cin >> filename;

	ifstream codesFile("codes.txt"), encodedFile(filename, ios::binary);
	if (!codesFile.is_open())
	{
		cout << "Error !!! Cannot open code file.\n";
		exit(1);
	}

	Node *head = new Node();
	unordered_map<string, char> huffmanCode;

	char leafNode;
	while (codesFile)
	{
		string binaryCode;
		codesFile >> binaryCode;
		codesFile.get(leafNode);
		codesFile.get(leafNode);
		if (binaryCode == "Completed")
			break;
		huffmanCode[binaryCode] = leafNode;
	}
	codesFile.close();

	char byte;
	string binaryCode;

	encodedFile.seekg(0, ios::end);
	int fileSize = encodedFile.tellg();
	encodedFile.seekg(0, ios::beg);

	ofstream decodedFile("Decompressed.txt");
	for (int i = 0; i < fileSize - 1; i++)
	{
		encodedFile.get(byte);
		for (int i = 0; i < 8; i++)
		{
			bool bit = (byte >> i) & 1;
			if (bit)
				binaryCode += "1";
			else
				binaryCode += "0";
			if (huffmanCode.find(binaryCode) != huffmanCode.end())
			{
				decodedFile << huffmanCode[binaryCode];
				binaryCode = "";
			}
		}
	}
	for (int i = 0; i < 8 - (leafNode - '0'); i++)
	{
		encodedFile.get(byte);
		bool bit = (byte >> i) & 1;
		if (bit)
			binaryCode += "1";
		else
			binaryCode += "0";
		if (huffmanCode.find(binaryCode) != huffmanCode.end())
		{
			decodedFile << huffmanCode[binaryCode];
			binaryCode = "";
		}
	}
	encodedFile.close();
	decodedFile.close();
}

int main()
{
	cout << "\n\t====================================";
	cout << "\n\t\t Text File Decompressor\n";
	cout << "\t====================================";

	decode();
	cout << "\nProcessing..... Plz Wait\n";

	cout << "\nDecompression Successful !!!\n";
	int l;
	cout << "\nEnter any key to exit : ";
	cin >> l;
	return 0;
}