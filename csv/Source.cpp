#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>


template<typename Tn>
Tn readCSVstr(std::istream& input, char separator) {
	Tn tn;
	input >> tn;
	return tn;
}

template<typename T1, typename T2, typename... Tn>
std::tuple<T1, T2, Tn...> readCSVstr(std::istream& input, char  separator) {
	T1 t1;
	std::string str;
	std::getline(input, str, separator);
	std::istringstream iss(str);
	iss >> t1;

	std::tuple<T1> t1tuple = std::make_tuple(t1);
	std::tuple<T2, Tn...> t2tntuple = readCSVstr<T2, Tn...>(input, separator);
	return std::tuple_cat(t1tuple, t2tntuple);
}

template<typename T1, typename...Tn>
std::vector<std::tuple<T1, Tn...>> readCSV(std::istream& file, char separator) {
	std::vector<std::tuple<T1, Tn...>> content;

	while (input.peek()) {
		content.push_back(readCSVstr<T1, Tn...>(file, separator));
	}
	return content;
}


int main() {
	return 0;
}