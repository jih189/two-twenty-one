#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char * argv[]){
	char * file = argv[1];
	ifstream infile;
	infile.open(file);

	double average[5];
	int i = 0;
	int j = 0;
	long sum = 0;
	int x = 0;
	while(infile >> x){
		if(j == 0){
			cout << x << endl;
			j++;
		}
		else if(j == 9999){
			sum += x;
			average[i] = (double)sum/9999;
			i++;
			j = 0;
			sum = 0;
		}
		else{
			sum += x; 
			j++;
		}
	}
	for(int k = 0; k < 5; k++){
		cout.precision(12);
		cout << "average at " << k << " is " << average[k] - 31.907 << endl;
	}

	infile.close();
	return 1;
}
