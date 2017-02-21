#include "Money.h"
#include <iostream>

Money *Money::currency = NULL;
//Money is using Singleton pattern, so only one instance 
//of money is created throughout the whole game. 

Money *Money::getInstance()
{
	if (currency == NULL)
		currency = new Money();
	return currency;
}

Money::~Money()
{
	delete currency;
}

int Money::getMoney()
{
	return money;
}

void Money::setMoney(int amt)
{
	money = amt;
}

void Money::addMoney(int amt)
{
	money += amt;
}

void Money::deductMoney(int amt)
{
	if ((money - amt) < 0)
		notEnoughMoney = true;

	else
	{
		money -= amt;
		notEnoughMoney = false;
	}

}