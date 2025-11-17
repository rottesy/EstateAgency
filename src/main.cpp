#include "../include/ui/MainWindow.h"
#include <QApplication>
#include <QLoggingCategory>
#include <exception>

namespace
{
constexpr const char *APPLICATION_STYLE = "Fusion";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    try
    {
        app.setStyle(APPLICATION_STYLE);

        MainWindow window;
        window.show();

        return app.exec();
    }
    catch (const std::exception &e)
    {
        qCritical() << "Exception caught:" << e.what();
        return 1;
    }
    catch (...)
    {
        qCritical() << "Unknown exception caught";
        return 1;
    }
}
