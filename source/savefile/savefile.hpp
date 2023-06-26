#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class savefile_t {

public: json config;

public: void create_config();

	  // call with no params to read from C:/Games/Bitwise/savefile.json
public: json read_config(std::string filename = "");

	  // call with no params to write this->config to C:/Games/Bitwise/savefile.json
public: void write_config(json config = nullptr, std::string filename = "");

};

extern savefile_t savefile;

enum upgrades {

	up_newcpu = 0,
	up_bettercode = 1,
	up_highres = 2,
	up_dst = 3,
	up_idlebits = 4,

};

enum games {
	
	gm_programming = 0,
	gm_codereviews = 1,
};