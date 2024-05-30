#include "gamepanel.h"

#include <QApplication>
#include "loading.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");

    Loading loading;
    loading.show();
    return a.exec();
}
