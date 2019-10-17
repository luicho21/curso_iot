#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main () {
	string line;
	ifstream infile("prueba.txt");
	if (infile.is_open())
	{	while( getline(infile,line) ){
			cout<<line<<"\n";
		}
	} 
	infile.close();
//	system("pause");
	return 0;

}
