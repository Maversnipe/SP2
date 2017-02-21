#ifndef MONEY_H
#define MONEY_H

class Money
{
public: 
	~Money();
	static Money *getInstance();

	bool notEnoughMoney = false;
	int getMoney();
	void setMoney(int);
	void addMoney(int);
	void deductMoney(int);
private:
	Money() : money(0) {}
	signed int money; 
	static Money *currency;
};

#endif