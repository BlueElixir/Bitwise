#include <map>
#include <string>

class player_t {

	    // stores upgrades map
private: std::map<std::string, int> upgrades;
	   
	    // returns upgrades map
public: std::map<std::string, int> get_upgrades();

		// set the value of an upgrade
public: void set_upgrade(std::string upgrade_name, int value);

		// increment the value of an upgrade by 1
public: void inc_upgrade(std::string upgrade_name);

		// decrease the value of an upgrade by 1
public: void dec_upgrade(std::string upgrade_name);

};