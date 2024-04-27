#include "../GUI Classes/graphwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GraphWidget widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
