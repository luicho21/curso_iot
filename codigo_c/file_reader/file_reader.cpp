#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main (int argc, char* argv[]) {
	string line;
	cout<<argv[1]<<endl;	
	ifstream infile(argv[1]);
	if (infile.is_open())
	{	while( getline(infile,line) ){
			cout<<line<<"\n";
		}
	} 
	infile.close();
//	system("pause");
	return 0;

}
