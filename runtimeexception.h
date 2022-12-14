#ifndef RUNTIMEEXCEPTION_H
#define RUNTIMEEXCEPTION_H

#include <QException>
#include <QString>

class RuntimeException : public QException
{
public:
    explicit RuntimeException(const char *what) : whatMes(what) { }
    void raise() const override { throw *this; }
    RuntimeException *clone() const override { return new RuntimeException(*this); }
    const char* what() const noexcept override { return whatMes; }

private:
    const char *whatMes;
};

#endif // RUNTIMEEXCEPTION_H
