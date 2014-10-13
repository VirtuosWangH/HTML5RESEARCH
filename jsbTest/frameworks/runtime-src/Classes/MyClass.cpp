#include "MyClass.h"
#include "cocos2d.h"


MyClass::MyClass(void* selector,  CallFunc func)
{
	m_selector = selector;
	m_callback = func;
}

MyClass::~MyClass()
{
}

void MyClass::testFunction(int value){
	cocos2d::log("MyClass.cpp ->testFunction called");

	m_callback(m_selector, value);
}
