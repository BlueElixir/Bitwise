#include "currency.hpp"

currency_t::currency_t(int amount = 0, std::string name = "") {
	this->amount = amount;
	this->name = name;
}

void currency_t::add(int amount = 1) {

	this->amount += amount;

}

void currency_t::remove(int amount = 1) {

	if (this->amount - amount >= 0)
		this->amount -= amount;

}

void currency_t::set(int amount = 0) {

	this->amount = amount;

}

int currency_t::get() {

	return this->amount;

}

void currency_t::set_name(std::string name) {

	this->name = name;

}

std::string currency_t::get_name() {

	return this->name;

}
