#ifndef MYMANIPULATORS
#define MYMANIPULATORS
#include <iostream>
#include <iomanip>

using std::ostream;
using std::istream;

namespace integration{
    namespace manipulator{
        template <typename T>
        class MyManipulator{
            T firstParam;
            ostream& (*execFunc)(ostream&, T);
        public:
            MyManipulator(ostream& (*execFunc)(ostream&, T), T firstParam):firstParam(firstParam),execFunc(execFunc){}
            //template <typename T1>
            friend ostream& operator << (ostream& stream, const MyManipulator<T>& manip){
                return manip.execFunc(stream,manip.firstParam);
            }
        };

        template <typename T, typename V>
        class MyManipulatorTwo{
            T firstParam;
            V secondParam;
            ostream& (*execFunc)(ostream&, T, V);
        public:
            MyManipulatorTwo(ostream& (*execFunc)(ostream&, T, V), T firstParam, V secondParam):firstParam(firstParam),secondParam(secondParam),execFunc(execFunc){}
            //template <typename T1, typename V1>
            friend ostream& operator << (ostream& stream, const MyManipulatorTwo<T,V>& manip){
                return manip.execFunc(stream,manip.firstParam, manip.secondParam);
            }
        };
    }
    inline ostream& outFull(ostream& stream, int number, int offset){
        if(number == 0){
            stream << "---";
            return stream;
        }
        for(int i = 0; i < offset+1; i++)
            stream << " ";
        stream << "|" << std::endl;
        for(int i = 0; i < offset+1; i++)
            stream << " ";
        stream << "+-";
        return stream;
    }

     inline manipulator::MyManipulatorTwo<int,int> out(int number, int offset){
        return manipulator::MyManipulatorTwo<int,int>(&outFull, number, offset);
    }
}

#endif // MYMANIPULATORS

