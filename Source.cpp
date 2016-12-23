#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <bitset>

using namespace std;

//parametrs	for generation stream
const float p = 0.9;
const int sizeStream = 1000000;
char *nameStreamFile = "stream.txt";
char *nameDecodingStreamFileNonOptimal = "decodingStreamNonOptimal.txt";
char *nameDecodingStreamFileOptimal = "decodingStreamOptimal.txt";
char *nameCodingStreamFileNonOptimal = "codingStreamNonOptimal.txt";
char *nameCodingStreamFileOptimal = "codingStreamOptimal.txt";

void writeStremInFile(char *nameStreamFile, vector<unsigned char> &stream)
{
	FILE *file = fopen(nameStreamFile, "wb");

	for (int i = 0; i < sizeStream; i++)
	{
		fwrite(&stream[i], 1, sizeof(unsigned char), file);
	}

	fclose(file);
}

void generateStream()
{
	vector <unsigned char> stream;
	bool flag;
	int count;
	float tmp;

	for (int i = 0; i < sizeStream; i++)
	{
		flag = false;
		count = 0;

		while (!flag)
		{
			tmp = float(rand()) / RAND_MAX;
			if (tmp > p)
			{
				flag = true;
			}
			count++;
			
			if (count > 255)
			{
				count = 255;
				flag = true;
			}
		}

		stream.push_back(count);
	}

	writeStremInFile(nameStreamFile, stream);
}

void readStremFromFile(char *nameStreamFile, vector<unsigned char> &stream)
{
	FILE *file = fopen(nameStreamFile, "rb");
	unsigned char ch;

	for (int i = 0; i < sizeStream; i++)
	{
		fread(&ch, 1, sizeof(unsigned char), file);
		stream.push_back(ch);
	}

	fclose(file);
}

void getCodingStream(vector<unsigned char> &streamForCoding, vector<bool> &encodingStream, int m)
{
	int r = 0;
	int c = 0;
	int q = 0; 
  	int n = 0;
	int f = 0;
	int sizeOfFisrtTable = 0;

	c = ceil(log2(float(m)));
	sizeOfFisrtTable = pow((float)2, (float)c) - m;

	for (int i = 0; i < sizeStream; i++)
	{
		n = streamForCoding[i];
		q = floor(n / m);
		r = n - m*q;
		
		for (int count = 0; count < q; count++)
		{
			encodingStream.push_back(1);
		}
		encodingStream.push_back(0);

		if (r < sizeOfFisrtTable)
		{	
			vector<bool> addCode;
			
			for (int k = 0; k < (c - 1); k++)
			{
				addCode.push_back(r % 2);
				r /= 2; 
			}
			
			for (int k = c-2; k >= 0; k--)
			{
				encodingStream.push_back(addCode[k]);
			}
			
		}
		else
		{
			vector<bool> addCode;
			r += sizeOfFisrtTable;
			for (int k = 0; k < c; k++)
			{
				addCode.push_back(r % 2);
				r /= 2;
			}

			for (int k = c - 1; k >= 0; k--)
			{
				encodingStream.push_back(addCode[k]);
			}

		}
	}
}

void getDecodingStream(vector<unsigned char> &decodingStream, vector<bool> &streamForDecoding, int m)
{
	int q = 0;
	int n = 0;
	int c = 0;
	int sizeOfFisrtTable = 0;
	int currPos = 0;

	c = ceil(log2(float(m)));
	sizeOfFisrtTable = pow((float)2, (float)c) - m;

	while (currPos < streamForDecoding.size())
	{
		q = 0;
		while (streamForDecoding[currPos] == 1)
		{
			q++;
			currPos++;
		}
		currPos++;
		
		n = 0;
		for (int i = currPos; i < currPos + c - 1; i++)
		{
			n = n * 2 + streamForDecoding[i];
		}
		currPos += (c - 1);

		if (n <  sizeOfFisrtTable)
		{
			decodingStream.push_back(q * m + n);
		}
		else
		{
			n = n * 2 + streamForDecoding[currPos];
			decodingStream.push_back(q * m + (n - sizeOfFisrtTable));
			currPos++;
		}
	}
}

int getBestM(float p)
{
	int m = 1;
	float pr = float(1) / (p + 1);
	bool flag = false;
	float lP = 0;
	float rP = 0;

	while (1)
	{
		lP = pow(p, (float)m);
		rP = pow(p, (float)(m - 1));
		
		if (pr >= lP && pr < rP)
		{
			flag = true;
			break;
		}
		else
		{
			m++;
		}
	}
	return m;
}

void writeBits(char *nameStreamFile, vector <bool> &encodingStream)
{
	FILE *file = fopen(nameStreamFile, "wb");
	bitset <8> byte;
	unsigned char ch;
	bool exit = false;

	for (int i = 0; i < encodingStream.size(); i += 8)
	{
		for (int j = i; j < i + 8; j++)
		{
			if (j >= encodingStream.size())
			{
				exit = true;
			}
			else
			{
				byte[j - i] = encodingStream[j];
			}
		}

		ch = byte.to_ullong();
		fwrite(&ch, 1, sizeof(unsigned char), file);
		if (exit == true)
			break;
	}
	fclose(file);
}

int main()
{
	int m = 2; // not optimal coding parametr
	int mBest = 0; // best coding parametr

	vector <unsigned char> stream;

	//generate stream and write in file
	generateStream();
	
	//read stream from file
	readStremFromFile(nameStreamFile, stream);
	
	//////////////////////////NOT OPTIMAL PARAMETR M///////////////////////////////
	
	{
		vector<bool> encodingStream;
		vector<unsigned char> decodingStream;

		//coding and write 
		getCodingStream(stream, encodingStream, m);
		writeBits(nameCodingStreamFileNonOptimal, encodingStream);

		//read and decoding stream write in file
		getDecodingStream(decodingStream, encodingStream, m);
		writeStremInFile(nameDecodingStreamFileNonOptimal, decodingStream);

		//test diffs
		for (int i = 0; i < decodingStream.size(); i++)
		{
			int dif = decodingStream[i] - stream[i];
			if (dif != 0)
			{
				printf("Erorr dif = %d/n", dif);
			}
		}
	}
	//////////////////////////BEST PARAMETR M///////////////////////////////
	{
		vector<bool> encodingStream;
		vector<unsigned char> decodingStream;
		
		mBest = getBestM(p);
		printf("oprimal parametr M - %d\n", mBest);

		//coding and write 
		getCodingStream(stream, encodingStream, mBest);
		writeBits(nameCodingStreamFileOptimal, encodingStream);

		//read and decoding stream write in file
		getDecodingStream(decodingStream, encodingStream, mBest);
		writeStremInFile(nameDecodingStreamFileOptimal, decodingStream);

		//test diffs
		for (int i = 0; i < decodingStream.size(); i++)
		{
			int dif = decodingStream[i] - stream[i];
			if (dif != 0)
			{
				printf("Erorr dif = %d/n", dif);
			}
		}
	}
	
	
	system("pause");
	return 0;
}