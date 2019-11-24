#include <iostream>

struct NullType {};

template <class Head, class Tail = NullType>
struct TypeList {
    typedef Head head;
    typedef Tail tail;
};


template<typename T>
T* ConvertToPtr(T& t) {
    return &t;
}
template<typename T>
T* ConvertToPtr(T* t) {
    return t;
}

template<typename T>
T& ConvertToRef(T& t) {
    return t;
}
template<typename T>
T& ConvertToRef(T* t) {
    return *t;
}


template <class... Tail>
class ArgsPack;

template <>
class ArgsPack<> {
public:
    ArgsPack() {}

    template <class Delegate, typename... Args>
    auto Call(const Delegate& delegate, Args&&... args) -> decltype(delegate(std::forward<Args>(args)...)) {
        return delegate(std::forward<Args>(args)...);
    }
};

template <class Head, class... Tail>
class ArgsPack<Head, Tail...> : public ArgsPack<Tail...> {
private:
    Head m_Param;
public:
    typedef ArgsPack<Tail...> _base;
    template <typename _Head, typename... _Tail>
    ArgsPack(_Head&& head, _Tail&&... tail) : _base(std::forward<_Tail>(tail)...), m_Param(std::forward<_Head>(head)) {}

    template <class Delegate, typename... Args>
    auto Call(const Delegate& delegate, Args&&... args) -> decltype(this->_base::Call(delegate, std::forward<Args>(args)..., ConvertToRef(m_Param))) {
        return this->_base::Call(delegate, std::forward<Args>(args)..., ConvertToRef(m_Param));
    }

    template <class Delegate, typename... Args>
    auto Call(const Delegate& delegate, Args&&... args) -> decltype(this->_base::Call(delegate, std::forward<Args>(args)..., ConvertToPtr(m_Param))) {
        return this->_base::Call(delegate, std::forward<Args>(args)..., ConvertToPtr(m_Param));
    }
};



template<class TObject, typename TCE>
struct SimpleDelegate {
private:
    TObject& m_Object;
    TCE m_CallableEntity;
public:
    SimpleDelegate(TObject& object, const TCE& ptr)
        : m_Object(object), m_CallableEntity(ptr) {}
    template<typename... Params2>
    auto operator()(Params2... args) const
    -> decltype((m_Object.*m_CallableEntity) (args...)) {
        return (m_Object.*m_CallableEntity) (args...);
    }
};

template <class TCE>
struct CEWrapper {
private:
    TCE m_CallableEntity;
public:
    CEWrapper(const TCE& ptr) : m_CallableEntity(ptr) {}

    template<typename... Params2>
    auto operator()(Params2... args) const -> decltype(m_CallableEntity(args...)) {
        return m_CallableEntity(args...);
    }
};

template<class TCallableEntity, class... Params>
class Functor {
private:
    ArgsPack<Params...> params_;
    TCallableEntity m_CallableEntity;
public:
    Functor(const TCallableEntity& m_CallableEntity, Params&&... params) : m_CallableEntity(m_CallableEntity), params_(std::forward<Params>(params)...) {}

    template<class TCE = TCallableEntity>
    auto Call() -> decltype(params_.Call(CEWrapper<TCE>(m_CallableEntity))) const {
        return params_.Call(m_CallableEntity);
    }
    template <class Object>
    auto Call(Object& obj) -> decltype(params_.Call(SimpleDelegate<Object, TCallableEntity>(obj, m_CallableEntity))) const {
        return params_.Call(SimpleDelegate<Object, TCallableEntity>(obj, m_CallableEntity));
    }
};

template<class TCallableEntity, typename...Params>
Functor<TCallableEntity, Params...> make_functor(const TCallableEntity& m_CallableEntity, Params&&... given_params) {
    return Functor<TCallableEntity, Params...>(m_CallableEntity, std::forward<Params>(given_params)...);
}

int function(int a, int b, int c) {
    return a + b + c;
}

class Y {
public:
    int val;
};

class X {
public:
    double process(Y& y) {
        return y.val / 3;
    }
};

int main() {
// обычные функции
    auto functor1 = make_functor(&function, 1, 2, 3);
    int x1 = functor1.Call();
    std::cout << x1 << std::endl;
// функции-члены
    auto functor2 = make_functor(&X::process, Y());
    X x;
    float y1 = functor2.Call(x);
    std::cout << y1 << std::endl;
// лямбды
    Y y;
    auto functor3 = make_functor([](Y*) {return 3;}, y);
    std::cout << functor3.Call() << std::endl;
    return 0;
}