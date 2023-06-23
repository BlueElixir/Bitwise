#include "player.hpp"


std::map<std::string, int> player_t::get_upgrades() {
	return this->upgrades;
}

void player_t::set_upgrade(std::string upgrade_name, int value) {

	this->upgrades[upgrade_name] = value;

}

void player_t::inc_upgrade(std::string upgrade_name) {

	this->upgrades[upgrade_name] += 1;

}

void player_t::dec_upgrade(std::string upgrade_name) {

	if (this->upgrades[upgrade_name] - 1 >= 0)
		this->upgrades[upgrade_name] -= 1;

}
