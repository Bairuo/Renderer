#ifndef PROPERTY_H_
#define PROPERTY_H_

#include<functional>

#define property_set(variableType) [&](variableType value)
#define property_get(variableType) [&]()->variableType

template <typename ValueType>
class property_rw
{
public:
    typedef std::function<void (ValueType value)> Setter;
    typedef std::function<ValueType (void)> Getter;
	explicit property_rw(Setter setter, Getter getter) : m_setter(setter), m_getter(getter) {}

    property_rw& operator=(ValueType value)
    {
        this->value = value;
        m_setter(this->value);
        return *this;
    }

    property_rw& operator=(const property_rw &instance)
    {
        this->value = instance.m_getter();
        m_setter(this->value);
        return *this;
    }

    ValueType operator()()
    {
        return m_getter();
    }

    operator ValueType()
    {
        return m_getter();
    }

private:
    ValueType value;
    Setter m_setter;
    Getter m_getter;
};

#endif // PROPERTY_H_
