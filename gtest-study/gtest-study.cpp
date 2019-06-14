// gtest-study.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "gtest/gtest.h"
#pragma comment(lib, "gtestd.lib")

int Add(int a, int b)
{
	if (a == b)
		return a;
	return a + b;
}

///1. 使用VS编译gtest.lib文件
///2. 设置测试工程的属性（头文件，lib文件， / MT参数（和编译gtest时使用一样的参数就行了））
///3. 使用TEST宏开始一个测试案例，使用EXPECT_*, ASSER_*系列设置检查点。
TEST(Test, Test)
{
	int x[2] = { 1,2 }, y[2] = { 1,3 };
	for (int i = 0; i < 2; ++i)
		EXPECT_EQ(x[i], y[i]) << "x != y when i == " << i;

	//布尔值检查
	EXPECT_TRUE(false);
	EXPECT_FALSE(true);

	//数值型数据检查
	EXPECT_EQ(Add(2, 1), 2);//equal
	EXPECT_NE(1, 1);//not equal
	EXPECT_LT(1, 2);//lower than
	EXPECT_LE(1, 2);//lower equal
	EXPECT_GT(1, 2);//greater than
	EXPECT_GE(1, 2);//greater equal
}

//字符串检查
TEST(StringCmpTest, Demo)
{
	const char* pszCoderZh = "CoderZh";
	const wchar_t* wszCoderZh = L"CoderZh";
	std::string strCoderZh = "CoderZh";
	std::wstring wstrCoderZh = L"CoderZh";

	EXPECT_STREQ("CoderZh", pszCoderZh);
	EXPECT_STREQ(L"CoderZh", wszCoderZh);

	EXPECT_STRNE("CnBlogs", pszCoderZh);
	EXPECT_STRNE(L"CnBlogs", wszCoderZh);

	EXPECT_STRCASEEQ("coderzh", pszCoderZh);//忽略大小写
	//EXPECT_STRCASEEQ(L"coderzh", wszCoderZh);    不支持

	EXPECT_STREQ("CoderZh", strCoderZh.c_str());
	EXPECT_STREQ(L"CoderZh", wstrCoderZh.c_str());
}

//显示返回成功或失败
TEST(ExplicitTest, Demo)
{
	ADD_FAILURE() << "Sorry"; // None Fatal Asserton，继续往下执行。
	//FAIL(); // Fatal Assertion，不往下执行该案例。
	SUCCEED();
}

int Foo(int a, int b)
{
	if (a == 0 || b == 0)
	{
		throw "don't do that";
	}
	int c = a % b;
	if (c == 0)
		return b;
	return Foo(b, c);
}

//异常检查
TEST(exception, exception)
{
	EXPECT_NO_THROW(Foo(10, 1));
	EXPECT_ANY_THROW(Foo(10, 1));
	EXPECT_THROW(Foo(0, 5), char*);
}

bool MutuallyPrime(int m, int n)
{
	return Foo(m, n) > 1;
}
//Predicate Assertions
TEST(PredicateAssertionTest, Demo)
{
	int m = 5, n = 6;
	EXPECT_PRED2(MutuallyPrime, m, n);
}
//自定义输出格式
testing::AssertionResult AssertFoo(const char* m_expr, const char* n_expr, const char* k_expr, int m, int n, int k) {
	if (Foo(m, n) == k)
		return testing::AssertionSuccess();
	testing::Message msg;
	msg << m_expr << " 和 " << n_expr << " 的最大公约数应该是：" << Foo(m, n) << " 而不是：" << k_expr;
	return testing::AssertionFailure(msg);
}
TEST(AssertFooTest, HandleFail)
{
	EXPECT_PRED_FORMAT3(AssertFoo, 3, 6, 2);
}

//浮点型检查
TEST(FloatTest, FloatTest)
{
	EXPECT_FLOAT_EQ(1.200000000f, 1.200000001f);
	EXPECT_DOUBLE_EQ(1.200000000, 1.200000001);
	EXPECT_NEAR(1.200000000, 1.200000001, 0.000000002);
	EXPECT_PRED_FORMAT2(testing::FloatLE, 1.200000000f, 1.200000001f);
	EXPECT_PRED_FORMAT2(testing::DoubleLE, 1.200000002, 1.200000001);
}

//Windows HRESULT assertions
TEST(Hresult, hresult)
{
	EXPECT_HRESULT_SUCCEEDED(S_OK);
	EXPECT_HRESULT_FAILED(S_OK);
}

//类型检查
template <typename T> class FooType {
public:
	void Bar() { testing::StaticAssertTypeEq<int, T>(); }
};
TEST(TypeAssertionTest, Demo)
{
	//FooType<bool> fooType;
	FooType<int> fooType;
	fooType.Bar();
}

///全局事件
///要实现全局事件，必须写一个类，继承testing::Environment类，实现里面的SetUp和TearDown方法。
class FooEnvironment : public testing::Environment
{
public:
	virtual void SetUp()
	{
		std::cout << "Foo FooEnvironment SetUP" << std::endl;
	}
	virtual void TearDown()
	{
		std::cout << "Foo FooEnvironment TearDown" << std::endl;
	}
};

///TestSuite事件
///我们需要写一个类，继承testing::Test，然后实现两个静态方法
class FooTest : public testing::Test
{
protected:
	static void SetUpTestCase() 
	{
		std::cout << "FooTest SetUpTestCase" << std::endl;
	}
	static void TearDownTestCase() 
	{
		std::cout << "FooTest TearDownTestCase" << std::endl;
	}
};
///在编写测试案例时，我们需要使用TEST_F这个宏，第一个参数必须是我们上面类的名字，代表一个TestSuite
TEST_F(FooTest, FooTest1)
{
	EXPECT_EQ(4, 5);
}

///TestCase事件
///TestCase事件是挂在每个案例执行前后的，实现方式和上面的几乎一样，不过需要实现的是SetUp方法和TearDown方法
class FooCalcTest :public testing::Test
{
protected:
	virtual void SetUp()
	{
		std::cout << "FooCalcTest SetUp" << std::endl;
	}
	virtual void TearDown()
	{
		std::cout << "FooCalcTest TearDown" << std::endl;
	}
};
TEST_F(FooCalcTest, FooCalcTest1)
{
	EXPECT_EQ(4, 5);
}

///使用参数化
///必须添加一个类，继承testing::TestWithParam<T>，其中T就是你需要参数化的参数类型
class AddParamTest : public::testing::TestWithParam<int>
{
};
TEST_P(AddParamTest, AddParamTest1)
{
	int n = GetParam();
	EXPECT_LT(n, 20);
}
///设定测试范围
INSTANTIATE_TEST_CASE_P(TrueReturn, AddParamTest, testing::Values(3, 5, 11, 23, 17));

///死亡测试
void Foo2()
{
	int *pInt = 0;
	*pInt = 42;
}
TEST(FooDeathTest, Demo)
{
	//testing::FLAGS_gtest_death_test_style = "threadsafe";
	testing::FLAGS_gtest_death_test_style = "fast";
	EXPECT_DEATH(Foo2(), "");
	EXPECT_DEBUG_DEATH(Foo2(), "");
}
TEST(ExitDeathTest, Demo)
{
	EXPECT_EXIT(_exit(1), testing::ExitedWithCode(1), "");
}

TEST(Foo000Test, Demo000)
{
	EXPECT_EQ(1, 1);
}

//在Main函数中初始化环境，再使用RUN_ALL_TEST()宏运行测试案例。
int main(int argc, char* argv[])
{
	//相当于命令行 --gtest_output=xml:
	testing::GTEST_FLAG(output) = "xml:";
	//挂载全局事件
	testing::AddGlobalTestEnvironment(new FooEnvironment);
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");

	return 0;
}