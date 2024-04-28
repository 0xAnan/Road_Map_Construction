#include "../GUI Classes/graphwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WelcomePage welcomePage;
    welcomePage.resize(800, 600);  // Set the size of the welcome page
    int result = welcomePage.exec();

    if (result == QDialog::Rejected) {
        return 0;  // Exit the application if the user clicks "Exit"
    }

    GraphWidget widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}