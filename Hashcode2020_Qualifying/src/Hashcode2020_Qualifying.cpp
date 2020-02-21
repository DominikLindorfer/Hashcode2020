//============================================================================
// Name        : Hashcode2020.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description : Hashcode 2020 Qualifying Round
//============================================================================

#include <bits/stdc++.h>
#include <omp.h>
//#include "Utilities.hpp"

using namespace std;

//void import(vector<double>& in, string name){
//	ifstream infile;
//	infile.open(name);
//
//	int tmp;
//	double tmp_double;
//
//	while(infile.is_open()){
//		if(infile.good()){
//			infile >> tmp >> tmp_double;
//			cout << tmp << "  " << tmp_double << endl;
//			in.push_back(tmp);
//		}
//		else
//			break;
//	}
//
//	infile.close();
//}

class book{
public:
	long id;
	long score;
	bool scanned;
	book(){
		scanned = false;
	}
};

class library{
public:

	long id;
	vector<long> books;
	long signup_time;
	bool signed_up;
	long book_per_day;
	long signed_up_at;
	long score;

	double score_2;

	long td;

	library(){
		signed_up = false;
	}

};

void import(string name, vector<book>& books, vector<library>& libraries, long& days){

	ifstream infile;
	infile.open(name);

	long numb_books, numb_libraries, numb_days = 0;

	infile >> numb_books >> numb_libraries >> numb_days;
	days = numb_days;

	books.resize(numb_books);
	libraries.resize(numb_libraries);

	for(long i = 0; i < numb_books; i++){
		books[i].id = i;
		infile >> books[i].score;
	}

	for(long i = 0; i < numb_libraries; i++){

		long books_i;
		long signup;
		long perday;

		infile >> books_i;
		libraries[i].books.resize(books_i);
		libraries[i].id = i;
		infile >> libraries[i].signup_time >> libraries[i].book_per_day;

		for(long b = 0; b < books_i; b++){
			infile >> libraries[i].books[b];
		}

	}

	infile.close();
}


int main() {

//	vector<string> file_names = {"a_example.txt"};
//	vector<string> file_names = {"a_example.txt", "b_read_on.txt", "c_incunabula.txt", "d_tough_choices.txt", "e_so_many_books.txt", "f_libraries_of_the_world.txt"};
	vector<string> file_names = {"f_libraries_of_the_world.txt", "e_so_many_books.txt"};

	long total_score = 0;

	for (const string& name : file_names) {

		cout << name << endl;

		long days;
		long time_done;

		vector<book> books;
		vector<library> libraries;

		//----- Import -----
		import(name, books, libraries, days);

		//-----Build Greedy Solution-----
//		for(auto& lib : libraries){
//
//			sort(lib.books.begin(), lib.books.end(), [&](long id_a, long id_b){ return books[id_a].score > books[id_b].score;} );
//			lib.td = days - lib.signup_time;
//
//			long score = 0;
//
//			for(long id = 0; (id < lib.td*lib.book_per_day) && (id < lib.books.size()); id++){
//
//				score += books[lib.books[id]].score;
//			}
//
//			lib.score = score;
//		}
//
//		sort(libraries.begin(), libraries.end(), [](library a, library b){ return (double)a.score/(a.signup_time) > (double)b.score/(b.signup_time);} );

		//-----Solution Vector-----
		vector<library> solution;

		//-----Optimize Greedy Solution-----

		for(auto& lib : libraries){

			sort(lib.books.begin(), lib.books.end(), [&](long id_a, long id_b){ return books[id_a].score > books[id_b].score;} );
		}

		long fix_days = days;

		while(days >= 0 && libraries.size()){

			double best = 0;
			long best_id = 0;

			long cur_id = 0;

			for(auto& lib : libraries){

				lib.td = days - lib.signup_time;

				if(lib.td <= 0){
					lib.score_2 = 0;
					cur_id++;
					continue;
				}

				long score = 0;
				long missed_score = 0;

				bool finished = (lib.books.size() < lib.td * lib.book_per_day);
				for(long id = 0; (id < lib.td*lib.book_per_day) && (id < lib.books.size()); id++){

					score += books[lib.books[id]].score;
				}
				if(!finished)
				{
					for(long id = lib.td*lib.book_per_day; (id < (lib.td+2)*lib.book_per_day) && (id < lib.books.size()); id++){

						missed_score += books[lib.books[id]].score;
					}
				}
				missed_score = 0;

				double cutoff = 0.5;

				if ((double) days / (double) fix_days > cutoff) {
					lib.score = score;
					lib.score_2 = (double) (lib.score - missed_score)
							/ ((double) lib.signup_time);
				}
				if ((double) days / (double) fix_days <= cutoff) {
					lib.score = score;
					lib.score_2 = (double) (lib.score - missed_score) * (double) (lib.score - missed_score)
							/ ((double) lib.signup_time);
				}
				if(!finished)
					lib.score_2 *= 1.25;

//				lib.score = score;
//				lib.score_2 = (double)lib.score / (double)lib.signup_time;
//				lib.score_2 = (double)lib.score;
//				lib.score_2 = 1.0 / (double)lib.signup_time;
//				lib.score_2 = lib.score * (double)lib.score / (double)lib.signup_time;
//				lib.score_2 = lib.score / ((double)lib.signup_time * (double)lib.signup_time);
//				lib.score_2 = lib.score * (double)lib.score / ((double)lib.signup_time);
//				lib.score_2 = (double)lib.books.size() * (double)lib.score / ((double)lib.signup_time);

				if(best <= lib.score_2){
					best = lib.score_2;
					best_id = cur_id;
				}

				cur_id++;
			}

			if(libraries[best_id].books.size() == 0){
				break;
			}

			solution.push_back(libraries[best_id]);
			libraries.erase(libraries.begin() + best_id);

			for(long b = 0; b < solution[solution.size() - 1].books.size() && b < solution[solution.size() - 1].td*solution[solution.size() - 1].book_per_day; b++){
				for (auto &lib : libraries) {

					for (long b_lib = 0; b_lib < lib.books.size(); b_lib++) {

						if (lib.books[b_lib] == solution[solution.size() - 1].books[b]) {
							lib.books.erase(lib.books.begin() + b_lib);
						}
					}
				}
			}

//			for(auto& b : solution[solution.size() - 1].books){
//				//optimize here
//				for(auto& lib : libraries){
//
//					for(long b_lib = 0; b_lib < lib.books.size(); b_lib++){
//
//						if(lib.books[b_lib] == b){
//							lib.books.erase(lib.books.begin() + b_lib);
//						}
//					}
//				}
//			}

			days -= solution[solution.size() - 1].signup_time;
		}



//		sort(solution.begin(), solution.end(), [](library a, library b){return a.books.size() < b.books.size();});


		//-----Greedy Solution Result-----
		//-----Output-----
		ofstream output;
		output.open(name + ".out");

		output << solution.size() << endl;

		for(long id = 0; id < solution.size(); id++){

			if(solution[id].books.size() > 0)
				output << solution[id].id << " " << solution[id].books.size() << endl;

			for(auto b_id : solution[id].books){
				output << b_id << " ";
			}

			output << endl;
		}

//		output << libraries.size() << endl;
//
//		for(long id = 0; id < libraries.size(); id++){
//
//			output << libraries[id].id << " " << libraries[id].books.size() << endl;
//
//			for(auto b_id : libraries[id].books){
//				output << b_id << " ";
//			}
//
//			output << endl;
//		}

		output.close();

	}


	//-----Pre-Prepared Stuff-----
	//		bool already_cached = find(caches[get<0>(cache)].videos.begin(), caches[get<0>(cache)].videos.end(), request.video_id) != caches[get<0>(cache)].videos.end();
	//		sort(Pizzas.begin(), Pizzas.end(), [](Pizza& a, Pizza& b){return a.slices < b.slices;});
	//		auto low_bound = lower_bound(Pizzas.begin(), Pizzas.end(), cur_slices,
	//					[](const Pizza & right, long left){
	//				return  right.slices < left;}
	//	vector<int> myvector = { 32, 71, 12, 45, 26, 80, 53, 33 };
	//
	//	sort(myvector.begin(), myvector.end(), [](int i, int j) {return i > j;});
	//
	//	for (int &i : myvector) {
	//		cout << i << endl;
	//	}
	//    unordered_map<string, photo> Result;
	//    Result["test"] = photo(1, {100,200,300}, true);
	//    Result["test2"] = photo(1, {100,200,300}, true);
	//    cout << Result["test"].tags << endl;
	//
	//    for(auto it : Result){
	//    	cout << it.first << "  " << it.second.tags << endl;
	//    }
	//
	//    for(auto it = Result.begin(); it != Result.end(); it++){
	//    	cout << (*it).first << "  " << (*it).second.tags << endl;
	//    }
	//
	//	#pragma omp parallel for
	//	for(long i = 0; i < 700000000; i++){
	//
	//		#pragma omp atomic
	//			total_score += i;
	//	}
	//
	//	generator = mt19937_64(1234);
	//
	//	Timer t;
	//	t.start();
	//	t.interval();
	//	t.stop();
	//	cout << t.duration() << endl;
	//
	//	class Timer {
	//	private:
	//		bool isRunning;
	//		chrono::high_resolution_clock::time_point startTime;
	//		chrono::high_resolution_clock::time_point endTime;
	//		double lastIntervalTotalTime;
	//
	//	public:
	//		Timer() {
	//			isRunning = false;
	//			lastIntervalTotalTime = 0;
	//		}
	//
	//		void start() {
	//			startTime = chrono::high_resolution_clock::now();
	//			isRunning = true;
	//		}
	//
	//		void stop() {
	//			endTime = chrono::high_resolution_clock::now();
	//			isRunning = false;
	//		}
	//
	//		double duration() {
	//			return chrono::duration_cast<chrono::milliseconds>(
	//					(isRunning ? chrono::high_resolution_clock::now() : endTime)
	//							- startTime).count();
	//		}
	//
	//		double durationSeconds() {
	//			return duration() / 1000.0;
	//		}
	//
	//		double interval() {
	//			double tmp = duration();
	//			double interval = tmp - lastIntervalTotalTime;
	//			lastIntervalTotalTime = tmp;
	//			return interval;
	//		}
	//	};
	//	mt19937_64 generator;
	//	uniform_real_distribution<double> distUniform(0.0, 1.0);
	//	normal_distribution<double> distNormal(0.0, 1.0);
	//
	//	double random01() {
	//		return distUniform(generator);
	//	}
	//
	//	double randomNormal() {
	//		return distNormal(generator);
	//	}
	//
	//	double randomNormal(double sigma) {
	//		return randomNormal() * sigma;
	//	}
	//
	//	double randomNormal(double sigma, double mu) {
	//		return randomNormal() * sigma + mu;
	//	}
	//
	//	int randomInt(int maxValue) {
	//		//INFO: test for maxValue > 0 is only needed for Gaussian wavepacket simulation where only one  particle is used
	//		return maxValue > 0 ?
	//				((int) floor(random01() * maxValue)) % maxValue : maxValue;
	//		//return ((int) floor(random01() * maxValue)) % maxValue;
	//	}
	//
	//	int randomInt(int minValue, int maxValue) {
	//		return randomInt(maxValue - minValue) + minValue;
	//	}
	//
	//	auto is_swap_1 = [&](vector<long> swap){ return (swap[0] == (*swap_1)[0]);};
	//	auto swap_1 = next(start_1, sw_1);
	//	//-----Check if Store from Route 1 is already in Route 2 and vice versa-----
	//	//-----and swap Demands accordingly-----
	//	if(end_2 == find_if(start_2,end_2, is_swap_1) && end_1 == find_if(start_1,end_1, is_swap_2)){
	//		auto temp = *swap_1;
	//		*swap_1 = *swap_2;
	//		*swap_2 = temp;
	//	}
	//
	//    std::vector<int> v{0, 2, 3, 25, 5};
	//    std::vector<int> t{3, 19, 10, 2};
	//
	//    auto result = std::find_first_of(v.begin(), v.end(), t.begin(), t.end());
	//
	//    if (result == v.end()) {
	//        std::cout << "no elements of v were equal to 3, 19, 10 or 2\n";
	//    } else {
	//        std::cout << "found a match at "
	//                  << std::distance(v.begin(), result) << "\n";
	//    }

	return 0;
}
