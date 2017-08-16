#pragma once
#include <iostream>
using namespace std;

class AbstractProduct
{
public:
	AbstractProduct() {};
	virtual ~AbstractProduct() {};
};

class ProductA:public AbstractProduct
{
public:
	friend class FactoryA;
protected:
	ProductA() {};
	virtual ~ProductA() {};
};

class AbstractFactory
{
public:
	virtual AbstractProduct* createProduct()
	{
		cout << "AbstractFactory :: createProduct " << endl;
		return nullptr;
	}
	AbstractFactory() {};
	virtual ~AbstractFactory() {};
};

class FactoryA :public AbstractFactory
{
public:
	virtual ProductA* createProduct()
	{
		ProductA* t = new ProductA;
		cout << "FactoryA :: createProduct " << endl;
		return t;
	}
	FactoryA() {};
	virtual ~FactoryA() {};
};