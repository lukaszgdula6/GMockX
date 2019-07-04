#ifndef GTESTX_HPP
#define GTESTX_HPP

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

    void registerMock(MockType *mock);
    void unregisterMock(MockType *mock);
    MockType *findMockFor(const ClassType *c);

    static MockList<MockType, ClassType> &instance();

private:
    typedef std::list<MockType*> Mocks;
    typedef std::map<const ClassType*, MockType*> Map;
    Mocks mocks;
    Map mapping;
};

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
void MockList<MockType, ClassType>::registerMock(MockType *mock)
{
    mocks.push_back(mock);
}

template<typename MockType, typename ClassType>
void MockList<MockType, ClassType>::unregisterMock(MockType *mock)
{
    if (std::find(mocks.begin(), mocks.end(), mock) != mocks.end())
        throw PendingMockException(mocks.size());
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
MockList<MockType, ClassType> &MockList<MockType, ClassType>::instance()
{
    static MockList<ClassType, MockType> inst;
    return inst;
}

}

#endif // GTESTX_HPP
