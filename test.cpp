#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

double calcdist(vector<double> a, vector<double> b, long size){
    double res = 0, temp;
    for(int i = 0 ;i < size; i++){
	cout << "a, b: " << a[i] << b[i] << endl;
        temp = a[i]-b[i];
	cout << "temp: " << temp << endl;
        res += pow(temp, 2);
	cout << "res: " << res << endl;
    }
    return sqrt(res);
}

int main(){

	vector<double> a;// = {15.26,14.84,0.871,5.763,3.312,2.221,5.22,1};
	a.push_back(15.26);
	a.push_back(14.84);
	a.push_back(0.871);
	a.push_back(5.763);
	a.push_back(3.312);
	a.push_back(2.221);
	a.push_back(5.22);
	a.push_back(1);

	vector<double> b;
	b.push_back(12.76);
	b.push_back(13.38);
	b.push_back(0.8964);
	b.push_back(5.073);
	b.push_back(3.155);
	b.push_back(2.828);
	b.push_back(4.83);
	b.push_back(3);


	//vector<double> b = {12.76,13.38,0.8964,5.073,3.155,2.828,4.83,3};

	double temp = calcdist(a, b, a.size()-1);

	cout << "temp: " << temp << endl;
	return 0;
}
