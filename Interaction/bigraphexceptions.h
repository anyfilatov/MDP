#pragma once
#include <QString>

namespace integration {

    class MyException{
    protected:
        QString errMsg;
    public:
        QString getMsg(){
            return errMsg;
        }
    };

    class Excpt_vec_out_of_range : public MyException{
    public:
        Excpt_vec_out_of_range(){
            errMsg = "Exception \"vector out of range\" occured!";
        }
    };

    class Excpt_null_ptr : public MyException{
    public:
        Excpt_null_ptr(){
            errMsg = "Exception \"null pointer exception\" occured!";
        }
    };
}
