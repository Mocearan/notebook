#include <iostream>
#include <string>
#include <map>


typedef void* (*Constructor)();

class ObjFactory
{
	using obj_map_t = std::map<std::string, Constructor>;
public:
	static void regist(const std::string& name, Constructor ctor)
	{
		constructor()[name] = ctor;
	}

	static void* create(const std::string& name)
	{
		Constructor ctor = nullptr;
		if (constructor().find(name) not_eq constructor().end())
			ctor = constructor().find(name)->second;

		if (ctor)
			return (*ctor)();
		else return nullptr;
	}
private:
	inline static obj_map_t& constructor()
	{
		static obj_map_t instance;
		return instance;
	}
};



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

#define REG_CLASS(class_name) \
class class_name##Helper \
{ \
public: \
	class_name##Helper() \
	{ \
	ObjFactory::regist(#class_name, class_name##Helper::create); \
	} \
	static void* create() \
	{ \
		return new class_name(); \
	} \
}; \
class_name##Helper class_name##helper;


class Factory
{
public:
	static Api* create()
	{
		return static_cast<Api*>(ObjFactory::create("Impl1"));
	}
};
REG_CLASS(Impl1)

int main(void)
{
	auto* api = Factory::create();
	api->test("test run");
	system("pause");
	return 0;
}