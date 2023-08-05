#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

#define EMPTY_STRING ""

static unsigned char currentByte = 0;
static int bitPosition = 0;
static int compressedFileSize = 0;
static int inputFileSize = 0;
void writeBitToFile(ofstream &fileStream, bool bit)
{
	// Set or clear the bit in the current byte based on the 'bit' parameter
	if (bit)
	{
		currentByte |= (1 << bitPosition);
	}
	else
	{
		currentByte &= ~(1 << bitPosition);
	}

	bitPosition++;

	// If we have written 8 bits (one byte), flush the byte to the file
	if (bitPosition == 8)
	{
		fileStream.put(currentByte);
		compressedFileSize++;
		currentByte = 0;
		bitPosition = 0;
	}
}

// A Tree node
struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

class Compress
{
	// Comparison object to be used to order the heap
	struct comp
	{
		bool operator()(const Node *l, const Node *r) const
		{
			// the highest priority item has the lowest frequency
			return l->freq > r->freq;
		}
	};

	bool isLeaf(Node *root)
	{
		return root->left == nullptr && root->right == nullptr;
	}

	// Function to allocate a new tree node
	Node *getNode(char ch, int freq, Node *left, Node *right)
	{
		Node *node = new Node();

		node->ch = ch;
		node->freq = freq;
		node->left = left;
		node->right = right;

		return node;
	}

public:
	void encode(Node *root, string str, unordered_map<char, string> &huffmanCode, ofstream &codesFile)
	{
		if (root == nullptr)
		{
			return;
		}

		// found a leaf node
		if (isLeaf(root))
		{
			if (str != EMPTY_STRING)
				huffmanCode[root->ch] = str;
			else
				huffmanCode[root->ch] = "1";
			codesFile << huffmanCode[root->ch] << ' ' << root->ch << endl;
		}

		encode(root->left, str + "0", huffmanCode, codesFile);
		encode(root->right, str + "1", huffmanCode, codesFile);
	}

	void buildHuffmanTree(string &text, string filename)
	{
		if (text == EMPTY_STRING)
		{
			cout << "EMPTY FILE!!.\nNo Compression performed.";
			return;
		}

		unordered_map<char, int> freq;
		for (char &ch : text)
			freq[ch]++, inputFileSize++;

		priority_queue<Node *, vector<Node *>, comp> pq;

		// Create a leaf node for each character and add it
		// to the priority queue.
		for (auto &pair : freq)
			pq.push(getNode(pair.first, pair.second, nullptr, nullptr));

		while (pq.size() != 1)
		{
			Node *left = pq.top();
			pq.pop();
			Node *right = pq.top();
			pq.pop();

			int sum = left->freq + right->freq;
			pq.push(getNode('\0', sum, left, right));
		}

		// `root` stores pointer to the root of Huffman Tree
		Node *root = pq.top();

		// Traverse the Huffman Tree and store Huffman Codes
		// in a map. Also, print them
		unordered_map<char, string> huffmanCode;
		ofstream codesFile, encodedFile;
		codesFile.open("codes.txt");
		encode(root, EMPTY_STRING, huffmanCode, codesFile);

		cout << "Huffman Codes are:\n"
			 << endl;
		for (auto pair : huffmanCode)
		{
			cout << pair.first << " " << pair.second << endl;
		}
		codesFile << "Completed ";

		// Write encoded string to file
		string compressedFile = filename + ".cmp";
		encodedFile.open(compressedFile, ios::binary);

		for (char &ch : text)
		{
			string binaryCode = huffmanCode[ch];
			for (char &bit : binaryCode)
				if (bit == '1')
					writeBitToFile(encodedFile, true);
				else
					writeBitToFile(encodedFile, false);
		}

		if (currentByte)
		{
			codesFile << 8 - bitPosition;
			encodedFile.put(currentByte);
			compressedFileSize++;
		}
		else
			codesFile << 0;
		codesFile.close();
		encodedFile.close();
	}
	void compressionRatio(string fileName)
	{
		cout << endl;
		cout << "File size of " + fileName + " : " << inputFileSize << endl;
		cout << "File size of " + fileName + ".cmp : " << compressedFileSize << endl;
		long double cratio = (long double)(compressedFileSize) / (long double)(inputFileSize)*100;
		cout << "\nCompression Ratio achieved : " << cratio << " % ";
	}
};

int main()
{
	string filename;
	cout << "\n\t====================================";
	cout << "\n\t\t  Text File Compressor\n";
	cout << "\t====================================";
	cout << "\n\nEnter Name of File to Compress : ";
	cin >> filename;

	ifstream inputFile(filename);

	string text;
	getline(inputFile, text);

	Compress c;
	c.buildHuffmanTree(text, filename);
	c.compressionRatio(filename);

	inputFile.close();
	cout << "\nCompression Successful !!!\n";
	int v;
	cout << "\nEnter any key to exit : ";
	cin >> v;
	return 0;
}