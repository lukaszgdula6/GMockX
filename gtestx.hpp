#ifndef GMOCKX_HPP
#define GMOCKX_HPP

#include "gmock.h"
#include "gtest.h"

#include <map>
#include <list>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <string>

namespace testing
{

template <typename MockType, typename ClassType = void>
class MockList
{
public:
    struct MockForNullException : std::runtime_error
    {
        MockForNullException();
    };
    struct NoMockException : std::runtime_error
    {
        NoMockException();
    };
    struct PendingMockException : std::runtime_error
    {
        PendingMockException(std::size_t numOfMocksPending);
    };
    struct MockAlreadyRegisteredException : std::runtime_error
    {
        MockAlreadyRegisteredException(const MockType *mock);
    };
    struct MockNotRegisteredException : std::runtime_error
    {
        MockNotRegisteredException(const MockType *mock);
    };

    void registerMock(MockType *mock);
    void unregisterMock(MockType *mock);
    MockType *findMockFor(const ClassType *c);
    void forget(const MockType *mock);

    static MockList<MockType, ClassType> &instance();

private:
    typedef std::list<MockType*> Mocks;
    typedef std::map<const ClassType*, MockType*> Map;
    Mocks mocks;
    Map mapping;
};

static unsigned GMOCKX_ATTRIBUTE_MOCK_CTOR = 0x0001;
static unsigned GMOCKX_ATTRIBUTE_MOCK_DTOR = 0x0002;

#define MAKE_GMOCKX_MOCK_TORS_C(className, ctorName, dtorName) \
    private: \
    unsigned gtestxAttributes; \
    public: \
    #className() \
    {} \
    #className(unsigned attributes) : gtestxAttributes(attributes) \
    { \
        if (attributes & GMOCKX_ATTRIBUTE_MOCK_CTOR) \
            #ctorName(); \
    } \
    ~#className() \
    { \
        if (attributes & GMOCKX_ATTRIBUTE_MOCK_DTOR) \
            #dtorName(); \
    }\
//    MOCK_METHOD0(#ctorName); \
//    MOCK_METHOD0(#dtorName); \

#define MAKE_GMOCKX_MOCK_TORS(className) \
    MAKE_GMOCKX_MOCK_TORS_C(#className, ctor, dtor)

namespace
{
template <typename T>
inline std::string to_hex(const T *ptr)
{
    unsigned long long value = reinterpret_cast<unsigned long long>(ptr);
    std::size_t expectedHexSize = sizeof(T*) * 2;
    std::string result;
    result.reserve(expectedHexSize + 2);
    while (value)
    {
        char c = value & 0x0F;
        result.insert(0, 1, c < 10 ? '0' + c : 'A' + c - 10);
        value = value >> 4;
    }
    result.insert(0, expectedHexSize - result.size(), '0');
    result.insert(0, "0x");
    return result;
}
}

template<typename MockType, typename ClassType>
MockList<MockType, ClassType>::MockForNullException::MockForNullException()
    : std::runtime_error(std::string("NULL pointer class requested for mock: ") + typeid(MockType).name())
{}

template<typename MockType, typename ClassType>
MockList<MockType, ClassType>::NoMockException::NoMockException()
    : std::runtime_error(std::string("Unexpected request for mock: ") + typeid(MockType).name())
{}

template<typename MockType, typename ClassType>
MockList<MockType, ClassType>::PendingMockException::PendingMockException(std::size_t numOfMocksPending)
    : std::runtime_error(std::string("Total ") + std::to_string(numOfMocksPending) + " pending mock(s): " + typeid(MockType).name())
{}

template<typename MockType, typename ClassType>
MockList<MockType, ClassType>::MockAlreadyRegisteredException::MockAlreadyRegisteredException(const MockType *mock)
    : std::runtime_error(std::string("Mock: ") + typeid(MockType).name() + " at: " + to_hex(mock) + " already registered")
{}
template<typename MockType, typename ClassType>
MockList<MockType, ClassType>::MockNotRegisteredException::MockNotRegisteredException(const MockType *mock)
    : std::runtime_error(std::string("Mock: ") + typeid(MockType).name() + " at: " + to_hex(mock) + " not registered")
{}

template<typename MockType, typename ClassType>
void MockList<MockType, ClassType>::registerMock(MockType *mock)
{
    if (std::find(mocks.begin(), mocks.end(), mock) != mocks.end())
        throw MockAlreadyRegisteredException(mock);
    mocks.push_back(mock);
}

template<typename MockType, typename ClassType>
void MockList<MockType, ClassType>::unregisterMock(MockType *mock)
{
    if (std::find(mocks.begin(), mocks.end(), mock) != mocks.end())
        throw PendingMockException(mocks.size());
    forget(mock);
}

template<typename MockType, typename ClassType>
MockType *MockList<MockType, ClassType>::findMockFor(const ClassType *c)
{
    if (!c)
        throw MockForNullException();

    typename Map::iterator it = mapping.find(c);
    if (it != mapping.end())
        return it->second;

    if (mocks.empty())
        throw NoMockException();
    MockType *mock = mocks.front();
    mocks.pop_front();
    return mapping[c] = mock;
}

template<typename MockType, typename ClassType>
void MockList<MockType, ClassType>::forget(const MockType *mock)
{
    for (typename Map::iterator it = mapping.begin(); it != mapping.end(); ++it)
        if (it->second == mock)
        {
            mapping.erase(it);
            return ;
        }
    throw MockNotRegisteredException(mock);
}

template<typename MockType, typename ClassType>
MockList<MockType, ClassType> &MockList<MockType, ClassType>::instance()
{
    static MockList<MockType, ClassType> inst;
    return inst;
}

}

#endif // GMOCKX_HPP
