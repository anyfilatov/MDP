#pragma once
#include <QString>

template<typename THost>
class Content {
public:
    Content(const QString& data, const THost& host) : data_(data), host_(host){}
    QString getData() const {
        return data_;
    }
    void setData(const QString& data) {
        data_ = data;
    }
    THost getHost() const {
        return host_;
    }
    void send() {
        host_.send(*this);
    }
private:
    QString data_;
    THost host_;
};
