
template<typename T>
void writeTo(std::ostream& out, const T& arg){
	static_assert(false);
}
template<>
void writeTo<int>(std::ostream& out, const int& arg){
	out << "_i(" + std::to_string(arg) + ")";
}

class DataHandler {

	bool readData() {

		return false;
	}

	bool signData() {

		return false;
	}

	bool storeData() {

		return false;
	}

	bool sendData() {

		return false;
	}
};
