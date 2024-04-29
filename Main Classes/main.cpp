#include "../GUI Classes/graphwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WelcomePage welcomePage;
    welcomePage.resize(800, 600);  // Set the size of the welcome page

    GraphWidget *widget = new GraphWidget;  // Create GraphWidget on the heap
    widget->resize(800, 600);

    QObject::connect(widget, &GraphWidget::exitToMainMenu, [&] {
        widget->hide();
        if (welcomePage.exec() == QDialog::Accepted) {
            widget->show();
        }
    });

    if (welcomePage.exec() == QDialog::Accepted) {
        widget->show();
    }

    return app.exec();
}