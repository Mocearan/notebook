
#include <string>	
#include <iostream>

class Api {
public:
	virtual void test(std::string s) = 0;

protected:
	Api() {}
};

class Impl : public Api
{
public:
	void test(std::string s) {
		std::cout << "impl" << " " << s << std::endl;
	}

};

//int main(void)
//{
//	Api* api = new Impl();
//	api->test("test run");
//
//	system("pause");
//	return 0;
//}