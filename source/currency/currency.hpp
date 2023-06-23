#include <string>

class currency_t {

private: int amount{ 0 };
private: std::string name;

currency_t(int amount = 0, std::string name = "");

public: void add(int amount = 1);
public: void remove(int amount = 1);
public: void set(int amount = 0);
public: int get();

public: void set_name(std::string name = "");
public: std::string get_name();

};