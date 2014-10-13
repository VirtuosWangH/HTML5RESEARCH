#pragma once
typedef void(*CallFunc)(void *selector, int value);
class MyClass
{
public:
	MyClass(void *selector,CallFunc func);
	~MyClass();
	void testFunction(int value);

private:
	void *m_selector;
	CallFunc m_callback;
};

