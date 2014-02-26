#include <QCoreApplication>
#include <string>
#include <iostream>

//---------------------------------------------------------------
class CompanyA {
public:
    // ...
    void sendClearText(const std::string& msg);
    void sendEncryptedText(const std::string& msg);
    // ...
};

void CompanyA::sendClearText(const std::string& msg) {
    std::cout << "sendClearText of CompanyA: " << msg << std::endl;
}

void CompanyA::sendEncryptedText(const std::string& msg) {
    std::cout << "sendEncryptedText of CompanyA: " << msg << std::endl;
}

//---------------------------------------------------------------
class CompanyB {
public:
    // ...
    void sendClearText(const std::string& msg);
    void sendEncryptedText(const std::string& msg);
    // ...
};

void CompanyB::sendClearText(const std::string& msg) {
    std::cout << "sendClearText of CompanyB: " << msg << std::endl;
}

void CompanyB::sendEncryptedText(const std::string& msg) {
    std::cout << "sendEncryptedText of CompanyB: " << msg << std::endl;
}

//---------------------------------------------------------------
// этот класс не представляет функции sendCleartext
class CompanyZ {
public:
    // ...
    void sendEncrypted(const std::string& msg);
    // ...
};

void CompanyZ::sendEncrypted(const std::string& msg) {
    std::cout << "sendEncryptedText of CompanyZ: " << msg << std::endl;
}

//---------------------------------------------------------------
// полная специализация MsgSender; отличается от общего шаблона
// только отсутствием функции sendCleartext
//template <>
//class MsgSender<CompanyZ> {
//public:
//    // ...
//    void sendSecret(const MsgInfo& info)
//    {
//        std::string msg;
//        // создать msg из info
//        msg = info.msg + " " + msg;
//        CompanyZ c;
//        c.sendEncrypted(msg);
//    }
//};

//---------------------------------------------------------------
// классы для других компаний
// ...

//---------------------------------------------------------------
// класс, содержащий информацию, используемую для создания сообщения
class MsgInfo {
public:
    std::string msg;
};

//---------------------------------------------------------------
template <typename Company>
class MsgSender {
public:
    // конструктор, деструктор и т. п.
    // ...

    void sendClear(const MsgInfo& info)
    {
        std::string msg;
        // создать msg из info
        msg = info.msg + " " + msg;
        Company c;
        c.sendClearText(msg);
    }

    // аналогично sendClear, но вызывает c.sendEncrypted(msg)
    void sendSecret(const MsgInfo& info)
    {
        std::string msg;
        // создать msg из info
        // ...
        Company c;
        c.sendEncrypted(msg);
    }
};

//---------------------------------------------------------------
template <typename Company>
class LoggingMsgSender: public MsgSender<Company> {
public:
    // второй способ (см. о чём идёт речь ниже)
    // сообщает компилятору о том, что
    // sendClear есть в базовом классе
    //using MsgSender<Company>::sendClear;
    // ...

    void sendClearMsg(const MsgInfo& info)
    {
        // записать в протокол перед отправкой;
        std::cout << "It is written in protocol before sending" << std::endl;

        // вызвать функцию из базового класса
        // этот код не будет компилироваться, так как в шаблонных классах
        // компилятор не ищет в базовом
        // sendClear(info);

        // чтобы избежать этого нужно написать (первый способ):
        this->sendClear(info);

        // второй способ: using MsgSender<Company>::sendClear;
        // sendClear(info);

        // третий способ: явно указать, что
        // вызываемая функция находится в базовом классе:
        // MsgSender<Company>::sendClear(info);
        // примечание: но этот способ хуже прочих, посколько если
        // вызываемая функция виртуальна, то явная квалификация
        // отключает динамическое связывание.

        // записать в протокол после отправки;
        std::cout << "It is written in protocol after sending" << std::endl;
    }
    // ...
};

//---------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //    CompanyA ca;
    //    ca.sendClearText("Hello!");
    //    ca.sendEncryptedText("Hello!");

    //    CompanyB cb;
    //    cb.sendClearText("Hello!");
    //    cb.sendEncryptedText("Hello!");


    //    MsgInfo msginfo;
    //    msginfo.msg = "Urrraaaaa!";

    //    MsgSender<CompanyA> msgSender;
    //    msgSender.sendClear(msginfo);

    MsgInfo msginfo;
    msginfo.msg = "Urrraaaaa!";

    LoggingMsgSender<CompanyA> loggingMsgSender;
    loggingMsgSender.sendClearMsg(msginfo);
    return a.exec();
}
