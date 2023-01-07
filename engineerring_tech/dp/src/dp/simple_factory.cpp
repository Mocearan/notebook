#include <iostream>
#include <string>

class Api {
public:
	virtual void test(std::string s) = 0;
protected:
	Api() {}
};

class Impl1 : public Api {
public:
	void test(std::string s) { std::cout << "Impl1 " << s << std::endl; }
};

class Impl2 : public Api {
public:
	void test(std::string s) { std::cout << "Impl2 " << s << std::endl; }
};

enum class IMPL_CODE : uint8_t
{
	IMPL_1,
	IMPL_2
};

class Factory {
public:
	static Api* createApi(IMPL_CODE type) {
		Api* api = nullptr;
		switch (type)
		{
		case IMPL_CODE::IMPL_1:
			api = new Impl1();
			break;
		case IMPL_CODE::IMPL_2:
			api = new Impl2();
			break;
		default:
			break;
		}
		return api;
	}
};
//
//int main(void)
//{
//	auto* api = Factory::createApi(IMPL_CODE::IMPL_2);
//	api->test("test run");
//	system("pause");
//	return 0;
//}