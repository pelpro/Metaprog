#include <string>
#include <iostream>

struct NullType {};

template <class Head, class Tail = NullType>
struct TypeList {
    typedef Head head;
    typedef Tail tail;
};

template <class TList, template <class> class Unit>
class GenScatterHierarchy;

template <class T1, class T2, template <class> class Unit> 
class GenScatterHierarchy<TypeList<T1, T2>, Unit> :
    public GenScatterHierarchy<T1, Unit>,
    public GenScatterHierarchy<T2, Unit> { 
public:
    typedef TypeList<T1, T2> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase; 
    typedef GenScatterHierarchy<T2, Unit> RightBase;
    template<typename T>
    struct Rebind {
        typedef Unit<T> Result;
    };

};  

template <class AtomicType, template <class> class Unit> 
class GenScatterHierarchy : public Unit<AtomicType> { 
public:
    typedef Unit<AtomicType> LeftBase;
    template<typename T>
    struct Rebind {
        typedef Unit<T> Result;
    };
};

template <template <class> class Unit>
class GenScatterHierarchy<NullType, Unit> {
    template<typename T>
    struct Rebind {
        typedef Unit<T> Result;
    };
};

template <class T> 
struct Holder {
    T value; 
};

template <class T, class H>
typename H::template Rebind<T>::Result& Field(H& obj){
    return obj;
}

using WidgetInfo = GenScatterHierarchy< TypeList<int, TypeList<std::string, double>>, Holder>;

int main() {
    WidgetInfo widgetInfo;
    Field<int>(widgetInfo).value = 1;
    Field<std::string>(widgetInfo).value = "String";
    Field<double>(widgetInfo).value = 1.234;
    std::cout << Field<double>(widgetInfo).value << std::endl;
    std::cout << Field<std::string>(widgetInfo).value << std::endl;
    std::cout << Field<int>(widgetInfo).value << std::endl;
    return 0;
}

