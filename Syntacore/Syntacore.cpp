// Syntacore.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>

const int N = 30;

void calculate(std::array <int, N + 1>& distribution, const std::vector <std::array <int, N>>& A, int begin, int end)
{
	int K = A.size();
	std::array <int, N> a{};
	int sum = 0;
	int g = (begin >> 1) ^ begin;
	int h = 0;
	int pos = 0;

	for (int i = 0; i < K; ++i)
		if ((g >> i) & 1)
			for (int j = 0; j < N; ++j)
				a[j] = !(a[j] == A[i][j]);
	for (int j = 0; j < N; ++j)
		sum += a[j];
	++distribution[sum];

	for (int i = begin; i < end; ++i)
	{
		sum = 0;
		h = i + 1;
		h = (h >> 1) ^ h;
		pos = log2(g ^ h);
		for (int j = 0; j < N; ++j)
		{
			a[j] = !(a[j] == A[pos][j]);
			sum += a[j];
		}
		++distribution[sum];
		g = h;
	}
}

int main()
{
	int K = 0;
	char temp_char = 0;
	std::array <int, N> temp_array;
	std::vector <std::array <int, N>> A;
	std::ifstream input;
	input.open("data.txt");
	while (input >> temp_char)
	{
		temp_array[0] = temp_char == '1';
		for (int j = 1; j < N; ++j)
		{
			input >> temp_char;
			temp_array[j] = temp_char == '1';
		}
		A.push_back(temp_array);
		++K;
	}	
	input.close();

	//for (int i = 0; i < K; ++i)
	//{
	//	for (int j = 0; j < N; ++j)
	//		std::cout << A[i][j];
	//	std::cout << std::endl;
	//}
	//std::cout << K << std::endl << std::endl;

	int k = 0;
	int rank = 0;
	for (int j = 0; j < N; ++j)
	{
		k = rank;
		while (k < K && !A[k][j]) ++k;
		if (k < K)
		{
			if (rank != k) std::swap(A[rank], A[k]);
			for (int i = rank + 1; i < K; ++i)
				if (A[i][j])
					for (int l = 0; l < N; ++l)
						A[i][l] = !(A[i][l] == A[rank][l]);
			++rank;
		}
	}
	A.resize(K = rank);
	std::vector <std::array <int, N>> (A).swap(A);

	//for (int i = 0; i < K; ++i)
	//{
	//	for (int j = 0; j < N; ++j)
	//		std::cout << A[i][j];
	//	std::cout << std::endl;
	//}
	//std::cout << A.capacity() << std::endl << std::endl;

	std::array <int, N + 1> distribution{};
	distribution[0] = 1;

	//auto start_time = std::chrono::high_resolution_clock::now();
	int T = 4;
	int M = pow(2, K);
	int L = M / T;
	std::vector <std::thread> threads;
	threads.reserve(T);
	for (int i = 0; i < T - 1; ++i)
		threads.push_back(std::thread(calculate, std::ref(distribution), A, i * L + 1, (i + 1) * L));
	threads.push_back(std::thread(calculate, std::ref(distribution), A, (T - 1) * L + 1, M - 1));
	for (int i = 0; i < T; ++i)
		threads[i].join();
	//auto end_time = std::chrono::high_resolution_clock::now();

	//for (int i = 0; i < N + 1; ++i)
	//	std::cout << distribution[i] << " ";
	//std::cout << std::endl << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << std::endl;

	//start_time = std::chrono::high_resolution_clock::now();
	//calculate(std::ref(distribution), A, 1, M - 1);
	//end_time = std::chrono::high_resolution_clock::now();
	//std::cout << std::endl << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << std::endl;

	std::ofstream output;
	output.open("weights.txt");
	for (int i = 0; i < N + 1; ++i)
		output << i << '\t' << distribution[i] << '\n';
	output.close();
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
