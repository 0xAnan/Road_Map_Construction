#include "../GUI Classes/graphpage.h"
#include "../GUI Classes/welcomepage.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WelcomePage welcomePage;
    welcomePage.resize(800, 600);  // Set the size of the welcome page

    GraphWidget *widget = new GraphWidget;  // Create GraphWidget on the heap
    widget->resize(800, 600);

    QObject::connect(widget, &GraphWidget::exitToMainMenu, [&] {
    widget->saveGraphData("../Data/GUIgraph_data.txt");  // Save the graph data
    widget->saveToFile("../Data/graph_data.txt");
    widget->hide();
    if (welcomePage.exec() == QDialog::Accepted) {
        widget->show();
    }
});



    if (welcomePage.exec() == QDialog::Accepted) {
        widget->show();
    }
    //widget->getGraph().Graph.printadjcentlist();
    return app.exec();
}