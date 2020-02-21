//============================================================================
// Name        : PizzaSlices.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description : Google Hashcode Pizza Problem 2020
//============================================================================

#include <bits/stdc++.h>
#include "Utilities.hpp"
using namespace std;

class Pizza{
public:
	long id;
	long slices;
	bool used;

	Pizza(){
		id = 0;
		slices = 0;
		used = false;
	}

	Pizza(long id_, long slices_){
		id = id_;
		slices = slices_;
		used = false;
	}
};

bool swap_1for2(vector<Pizza>& Pizzas, vector<int>& score_ids, long& score, long& max_score){

	auto score_new = score;

	long old_id = rand() % score_ids.size();

	long new_id1 = rand() % Pizzas.size();
	long new_id2 = rand() % Pizzas.size();

	while(new_id1 == new_id2){
		new_id2 = rand() % Pizzas.size();
	}

	if(Pizzas[new_id1].used == true || 	Pizzas[new_id2].used == true){
		return false;
	}

	auto old_pizza_id = score_ids[old_id];

	score_new -= Pizzas[old_pizza_id].slices;
	score_new += Pizzas[new_id1].slices + Pizzas[new_id2].slices;

	if(score_new <= max_score && score_new > score){

		Pizzas[new_id1].used = true;
		Pizzas[new_id2].used = true;
		Pizzas[old_pizza_id].used = false;

		score_ids.erase(score_ids.begin() + old_id);
		score_ids.push_back(Pizzas[new_id1].id);
		score_ids.push_back(Pizzas[new_id2].id);

		score = score_new;
		cout << "New best score found! -> " << score_new << "  " << score_ids << endl;

		return true;
	}
	return false;
}

int main() {

	auto file_names = {"a_example.in", "b_small.in", "c_medium.in", "d_quite_big.in", "e_also_big.in"};

	for(auto name : file_names){

	//-----Read Input-----
	ifstream in_file;
	in_file.open(name);

	long max_slices = 0;
	long numb_pizza = 0;

	in_file >> max_slices >> numb_pizza;

	vector<Pizza> Pizzas;
	long slices;

	for(auto i : range(numb_pizza)){

		in_file >> slices;
		Pizzas.push_back(Pizza(i, slices));
	}

	//-----Sort Pizzas by slices-----
	sort(Pizzas.begin(), Pizzas.end(), [](Pizza& a, Pizza& b){return a.slices < b.slices;});

	//-----Build Greedy Solution-----
	long id = 0;
	for(auto & p : Pizzas){
		p.id = id;
		id++;
//		cout << p.used << " " << p.id << endl;
	}

	auto cur_slices= max_slices;
	vector<int> score_ids;

	while(cur_slices > 0){

		auto low_bound = lower_bound(Pizzas.begin(), Pizzas.end(), cur_slices,
				[](const Pizza & right, long left){
					return  right.slices < left;}
		);

		if(low_bound != Pizzas.begin()){
			low_bound = prev(low_bound, 1);

			while((*low_bound).used){
				low_bound = prev(low_bound, 1);

				if((*low_bound).used == true && low_bound == Pizzas.begin()){
					break;
				}
			}
			cur_slices = cur_slices - (*low_bound).slices;
			(*low_bound).used = true;
			score_ids.push_back((*low_bound).id);

		}
		else{
			if(cur_slices == (*low_bound).slices){
				cur_slices = cur_slices - (*low_bound).slices;
				(*low_bound).used = true;
				score_ids.push_back((*low_bound).id);
			}
			else{
				break;
			}
		}

	}

	sort(score_ids.begin(), score_ids.end());

	//-----Greedy Solution Result-----
	auto score =  max_slices - cur_slices;
	cout << "Score: " << score << "  ids: " << score_ids << endl;

	//-----Optimize Greedy Solution-----
	for(int i = 0; i < 1000000000; i++){
		swap_1for2(Pizzas, score_ids, score, max_slices);
	}
	sort(score_ids.begin(), score_ids.end());

	//-----Output-----
	ofstream outfile;
	outfile.open(string(name) + ".out");

	outfile << score_ids.size() << endl;
	for(auto i : score_ids){
		outfile << i << " ";
	}

	outfile.close();
	in_file.close();
	}

	return 0;
}
