#include "../GUI Classes/graphwidget.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMessageBox welcomeBox;
    welcomeBox.setWindowTitle("Welcome");
    welcomeBox.setText("Welcome to the Graph Application!");
    welcomeBox.exec();

    GraphWidget widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
