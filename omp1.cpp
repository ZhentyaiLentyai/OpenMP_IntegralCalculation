#include <iostream>
#include <math.h>
#include <fstream>
#include <omp.h>
#include <ctime>

using namespace std;

int CheckArgc(int argc)
{
	if (argc < 4 || argc > 4) {
		cout << "Wrong number of arguments\n";
		return 1;
	}
	return 0;
}

int DefinitionThreads(char* argv[])
{
	int threads = atoi(argv[3]);

	switch (threads)
	{
	case -1:
		threads = 1;
		break;
	case 0:
		threads = omp_get_num_procs();
		break;
	default:

		break;
	}
	return threads;
}

bool Omp(int threads)
{
	bool omp = true;
	if (threads == 1) {
		omp = false;
	}
	return omp;
}

int CheckFileInput(char* argv[])
{
	ifstream input(argv[1]);

	if (!input.is_open()) {
		cout << "Failed to open file_input" << endl;
		return 1;
	}

	string temp[10];

	int count = 0;

	for (int i = 0; i < 10; i++) {
		input >> temp[i];

		size_t found = temp[i].find_first_not_of("1234567890.-");
		if (found != string::npos) {
			cout << "File format not supported\n";
			return 1;
		}

		if (temp[i] == "") {
			break;
		}

		count++;
	}

	if (count < 3 || count > 3) {
		cout << "File format not supported\n";
		return 1;
	}
	input.close();
	return 0;
}

double fuction(double x)
{
	double temp = (sin(x));
	double temp2 = log(temp);
	return temp2;
}

double Integral(double a, double b, double n, double y)
{
	return ((b - a) / (2 * n) * y);
}

double TrapezoidMethod(double a, double b, const int n, double In, bool omp, int threads)
{
	double h;
	double y = 0;

	h = (b - a) / n;

	y += fuction(a) + fuction(b);

#pragma omp parallel reduction(+:y) if (omp) num_threads(threads)
	{
		if (omp_in_parallel())
		{
#pragma omp for schedule(dynamic, 4)
			for (int i = 1; i < n; i++)
			{
				y += 2 * (fuction(a + h * i));
			}
		}
		else
		{
#pragma omp single
			for (int i = 1; i < n; i++)
			{
				y += 2 * (fuction(a + h * i));
			}
		}
	}
	return Integral(a, b, n, y);
}

double Runge(double a, double b, float error_val, bool omp, int threads)
{
	double In = 0;
	int n = 1;

	double z1 = TrapezoidMethod(a, b, n, In, omp, threads);
	double z2;
	double curEps;
	do {
		n *= 2;
		z2 = TrapezoidMethod(a, b, n, In, omp, threads);
		curEps = abs(z1 - z2);
		z1 = z2;

	} while (curEps > error_val);
	return z2;
}

int CheckFileOutPut(char* argv[], double In)
{
	ofstream output(argv[2]);

	if (!output.is_open()) {
		cout << "Failed to open file_output" << endl;
		return 1;
	}

	output << ("%g\n", In);
	output.close();
	return 0;
}


int main(int argc, char* argv[])
{
	if (CheckArgc(argc) == 1) {
		return 1;
	}

	int threads = DefinitionThreads(argv);
	bool omp = Omp(threads);

	if (CheckFileInput(argv) == 1) {
		return 1;
	}

	double a = 0, b = 0;
	float error_val = 0.0;

	ifstream open(argv[1]);

	open >> a;
	open >> b;
	open >> error_val;

	open.close();

	double In;

	int lastTime  = int(omp_get_wtime() * 1000);


	In = Runge(a, b, error_val, omp, threads);

	int currTime = int(omp_get_wtime() * 1000);
	int endTime = (currTime - lastTime);

	if (CheckFileOutPut(argv, In) == 1) {
		return 1;
	}

	printf("Time %i thread(s) %i %i %g ms", threads, lastTime, currTime, endTime);

	cout << "\nLaboratory work omp1 the end!\n";
	return 0;
}

