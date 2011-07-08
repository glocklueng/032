#ifndef QUADCONSOLE_H
#define QUADCONSOLE_H

#include <QMainWindow>

namespace Ui {
    class quadconsole;
}

class quadconsole : public QMainWindow
{
    Q_OBJECT

public:
    explicit quadconsole(QWidget *parent = 0);
    ~quadconsole();

private:
    Ui::quadconsole *ui;
};

#endif // QUADCONSOLE_H
