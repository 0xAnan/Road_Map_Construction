#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QPushButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QCursor>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>

#include <map>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <iostream>
#include "Headers/Graph.h"
#include "Headers/User_Interface.h"

inline User_Inrerface UI;
inline int return_value;
inline graph Graph;

// Comparator for QPoint objects
struct PointComparator {
    bool operator()(const QPoint &a, const QPoint &b) const {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    }
};

// Graph class represents the graph visualization
class GraphGUI : public QWidget
{
    Q_OBJECT

public:
    // Constructor for the Graph class
    GraphGUI(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(300, 300);
    }

    QVector<QPoint> points;  // Stores the points in the graph
    std::map<QPoint, QString, PointComparator> pointNames;  // Stores the names of the points
    QList<QPair<QPair<QPoint, QPoint>, double>> lines;  // Stores the lines in the graph
    QVector<QPair<QPointF, QString>> relativePoints;  // Stores the points relative to the size of the widget
    QList<QPair<QPair<QPoint, QPoint>, double>> highlightedLines;  // Stores the lines to be highlighted


    void saveToFileGUI(const QString &filename) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        // Save points
        for (const auto &pair : relativePoints) {
            out << pair.first.x() << ' ' << pair.first.y() << ' ' << pair.second << '\n';
        }
        out << "LINES\n";
        // Save lines
        for (const auto &line : lines) {
            out << pointNames[line.first.first] << ' ' << pointNames[line.first.second] << ' ' << line.second << '\n';
        }
    }

    void Load_GUI_Data(const QString &filename) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
        QString line;
        points.clear();
        pointNames.clear();
        lines.clear();
        relativePoints.clear();

        // Load points
        while (!(line = in.readLine()).isNull()) {
            if (line == "LINES") break;
            int firstSpace = line.indexOf(' ');
            int secondSpace = line.indexOf(' ', firstSpace + 1);
            qreal x = line.left(firstSpace).toDouble();
            qreal y = line.mid(firstSpace + 1, secondSpace - firstSpace - 1).toDouble();
            QString name = line.mid(secondSpace + 1);
            QPointF relativePoint(x, y);
            relativePoints.push_back(qMakePair(relativePoint, name));
        }
        updatePoints();

        // Load lines
        while (!(line = in.readLine()).isNull()) {
            QStringList parts = line.split(' ');
            QString point1Name = parts[0];
            QString point2Name = parts[1];
            double value = parts[2].toDouble();
            QPoint point1, point2;
            for (const auto &pair : pointNames) {
                if (pair.second == point1Name) point1 = pair.first;
                if (pair.second == point2Name) point2 = pair.first;
            }
            lines.push_back(qMakePair(qMakePair(point1, point2), value));
        }
    }

    void highlightLines(const QStringList &cityPairs) {
        highlightedLines.clear();
        for (const auto &cityPair : cityPairs) {
            QStringList cities = cityPair.split(" - ");
            if (cities.size() != 2) continue;
            QString city1 = cities[0];
            QString city2 = cities[1];
            for (const auto &line : lines) {
                if ((pointNames[line.first.first] == city1 && pointNames[line.first.second] == city2) ||
                    (pointNames[line.first.first] == city2 && pointNames[line.first.second] == city1)) {
                    highlightedLines.push_back(line);
                    break;
                    }
            }
        }
    }


protected:
    // Event handler for mouse press events
    void mousePressEvent(QMouseEvent *event) override
    {
        // If a point is clicked, store it in draggedPoint
        for (auto &point : points) {
            if (QLineF(event->pos(), point).length() < 10.0) {
                draggedPoint = &point;
                break;
            }
        }
    }

    // Event handler for mouse move events
    void mouseMoveEvent(QMouseEvent *event) override
    {
        // If a point is being dragged, update its position
        if (draggedPoint) {
            QApplication::setOverrideCursor(Qt::CrossCursor);
            QPoint oldPos = *draggedPoint;
            QPoint newPos = event->pos();

            // Define a larger margin
            int margin = 60;  // Adjust this value as needed

            // Check if the new position is within the valid area
            if (newPos.x() < margin || newPos.y() < margin || newPos.x() > width() - margin || newPos.y() > height() - margin) {
                return;
            }

            // Update the point's position
            *draggedPoint = newPos;

            // Update the point's name in pointNames
            QString name = pointNames[oldPos];
            pointNames.erase(oldPos);
            pointNames[newPos] = name;

            // Update the corresponding point in relativePoints
            for (auto &pair : relativePoints) {
                if (pair.second == name) {
                    pair.first = QPointF((qreal)newPos.x() / width(), (qreal)newPos.y() / height());
                    break;
                }
            }

            // Update the lines connected to the point
            for (auto &line : lines) {
                if (line.first.first == oldPos) {
                    line.first.first = newPos;
                } else if (line.first.second == oldPos) {
                    line.first.second = newPos;
                }
            }

            // Update the highlighted lines connected to the point
            for (auto &line : highlightedLines) {
                if (line.first.first == oldPos) {
                    line.first.first = newPos;
                } else if (line.first.second == oldPos) {
                    line.first.second = newPos;
                }
            }

        }
        else {
            QApplication::setOverrideCursor(Qt::ArrowCursor);
        }
        update();
    }

    // Event handler for mouse release events
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        // Stop dragging the point
        draggedPoint = nullptr;
    }

    void contextMenuEvent(QContextMenuEvent *event) override {
        // If a point is right-clicked, allow the user to edit its name
        for (auto &point : points) {
            if (QLineF(event->pos(), point).length() < 10.0) {
                bool ok;
                QString newName = QInputDialog::getText(this, tr("Edit Point Name"),
                                                        tr("New name:"), QLineEdit::Normal,
                                                        pointNames[point], &ok);
                if (ok && !newName.isEmpty()) {
                    QString oldName = pointNames[point];

                    // Update the backend data structure
                    return_value = UI.UpdateName(oldName.toStdString(), newName.toStdString(), Graph);
                    if (return_value == 0) {
                        // Update the point's name in pointNames
                        pointNames[point] = newName;

                        // Update the corresponding point in relativePoints
                        for (auto &pair : relativePoints) {
                            if (pair.second == oldName) {
                                pair.second = newName;  // Update the name in relativePoints
                                break;
                            }
                        }
                        QMessageBox::information(this, tr("Success"), tr("City Name Changed Successfully."));
                    } else if (return_value == 1) {
                        QMessageBox::warning(this, tr("Error"), tr("Changing The City Name Failed."));
                    } else if (return_value == 2) {
                        QMessageBox::warning(this, tr("Error"), tr("The City Doesn't Exist."));
                    } else if (return_value == 3) {
                        QMessageBox::warning(this, tr("Error"), tr("The New Name Already Exists."));
                    }
                    update();
                }
                break;
            }
        }
    }

    // Event handler for paint events
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QPen pen;  // Declare the QPen object here
        pen.setWidth(2);  // Set the width of the pen to 3

        QFont font = painter.font() ;
        font.setPointSize ( 14 ); // Set the size of font
        font.setBold(true); // Make font bold
        painter.setFont(font); // Set the font

        // Draw lines
        for (const auto &line : lines) {
            if (highlightedLines.contains(line)) {
                pen.setColor(Qt::green);
            } else {
                pen.setColor(Qt::darkGray);
            }
            painter.setPen(pen);
            painter.drawLine(line.first.first, line.first.second);

            // Always draw the line value in white
            pen.setColor(Qt::white);
            painter.setPen(pen);
            QPoint midpoint = (line.first.first + line.first.second) / 2;
            painter.drawText(midpoint, QString::number(line.second));
        }

        // Reset the color of the text for other drawings
        pen.setColor(Qt::white);
        painter.setPen(pen);

        // Draw points
        for (const auto &point : points) {
            // Check if the point is connected to any other point
            bool isConnected = false;
            for (const auto &line : lines) {
                if (line.first.first == point || line.first.second == point) {
                    isConnected = true;
                    break;
                }
            }

            // Draw the point as a filled circle
            int radius = 5;  // Adjust this value to change the size of the circle
            if (isConnected) {
                painter.setBrush(Qt::green);
            } else {
                painter.setBrush(Qt::gray);
            }
            painter.drawEllipse(point, radius, radius);

            // Draw the point name
            pen.setColor(Qt::white);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);  // Reset the brush before drawing text
            auto it = pointNames.find(point);
            if (it != pointNames.end()) {
                painter.drawText(point + QPoint(10, -10), it->second);
            }
        }
    }

    // Event handler for resize events
    void resizeEvent(QResizeEvent *event) override
    {
        // Update the positions of the lines
        for (auto &line : lines) {
            QPoint point1 = line.first.first;
            QPoint point2 = line.first.second;
            for (const auto &pair : relativePoints) {
                if (pair.second == pointNames[point1]) {
                    int x = pair.first.x() * width();
                    int y = pair.first.y() * height();
                    line.first.first = QPoint(x, y);
                }
                if (pair.second == pointNames[point2]) {
                    int x = pair.first.x() * width();
                    int y = pair.first.y() * height();
                    line.first.second = QPoint(x, y);
                }
            }
        }

        // Update the positions of the points
        updatePoints();
        QWidget::resizeEvent(event);
    }

private:
    QPoint *draggedPoint = nullptr;  // The point currently being dragged

    // Update the positions of the points
    void updatePoints() {
        points.clear();
        pointNames.clear();
        for (const auto &pair : relativePoints) {
            int x = pair.first.x() * width();
            int y = pair.first.y() * height();
            QPoint point(x, y);
            points.push_back(point);
            pointNames[point] = pair.second;
        }
    }
};

// Dialog for connecting points
class ConnectPointsDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectPointsDialog(const QStringList &pointNames, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Combo box for selecting the first point
        point1ComboBox = new QComboBox;
        point1ComboBox->addItems(pointNames);
        layout->addWidget(point1ComboBox);

        // Combo box for selecting the second point
        point2ComboBox = new QComboBox;
        point2ComboBox->addItems(pointNames);
        layout->addWidget(point2ComboBox);

        // Line edit for entering the value of the line
        valueLineEdit = new QLineEdit;
        layout->addWidget(valueLineEdit);

        // OK button
        QPushButton *okButton = new QPushButton(tr("OK"));
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        layout->addWidget(okButton);

        setLayout(layout);
    }

    QString point1Name() const { return point1ComboBox->currentText(); }
    QString point2Name() const { return point2ComboBox->currentText(); }
    double value() const { return valueLineEdit->text().toDouble(); }

private:
    QComboBox *point1ComboBox;
    QComboBox *point2ComboBox;
    QLineEdit *valueLineEdit;
};
class GraphWidget : public QWidget
{
    Q_OBJECT

public:

GraphWidget(QWidget *parent = nullptr) : QWidget(parent)
{
    QVBoxLayout *sidebarLayout = new QVBoxLayout;

    QString buttonStyle = "QPushButton {"
                      "background-color: #4CAF50;"
                      "border: none;"
                      "color: white;"
                      "padding: 15px 32px;"
                      "text-align: center;"
                      "text-decoration: none;"
                      "font-size: 16px;"
                      "margin: 4px 2px;"
                      "border-radius: 12px;}"
                      "QPushButton:hover {background-color: #45a049;}";


    QPushButton *addButton = new QPushButton(tr("Add City"), this);
    addButton->setStyleSheet(buttonStyle);
    connect(addButton, &QPushButton::clicked, this, &GraphWidget::addPoint);
    sidebarLayout->addWidget(addButton);

    QPushButton *clearButton = new QPushButton(tr("Clear All Cities"), this);
    clearButton->setStyleSheet(buttonStyle);
    connect(clearButton, &QPushButton::clicked, this, &GraphWidget::clearPoints);
    sidebarLayout->addWidget(clearButton);

    QPushButton *connectButton = new QPushButton(tr("Connect Cities"), this);
    connectButton->setStyleSheet(buttonStyle);
    connect(connectButton, &QPushButton::clicked, this, &GraphWidget::connectPoints);
    sidebarLayout->addWidget(connectButton);

    QPushButton *removePointButton = new QPushButton(tr("Remove City"), this);
    removePointButton->setStyleSheet(buttonStyle);
    connect(removePointButton, &QPushButton::clicked, this, &GraphWidget::removeSpecificPoint);
    sidebarLayout->addWidget(removePointButton);

    QPushButton *removeLineButton = new QPushButton(tr("Remove Edge"), this);
    removeLineButton->setStyleSheet(buttonStyle);
    connect(removeLineButton, &QPushButton::clicked, this, &GraphWidget::removeSpecificLine);
    sidebarLayout->addWidget(removeLineButton);

    QPushButton *editLineButton = new QPushButton(tr("Edit Edge"), this);
    editLineButton->setStyleSheet(buttonStyle);
    connect(editLineButton, &QPushButton::clicked, this, &GraphWidget::editLine);
    sidebarLayout->addWidget(editLineButton);

    QPushButton *displayDistanceButton = new QPushButton(tr("Display Total Distance"), this);
    displayDistanceButton->setStyleSheet(buttonStyle);
    connect(displayDistanceButton, &QPushButton::clicked, this, &GraphWidget::displayDistance);
    sidebarLayout->addWidget(displayDistanceButton);

    QPushButton *displayAlgorithmsButton = new QPushButton(tr("Display Algorithms"), this);
    displayAlgorithmsButton->setStyleSheet(buttonStyle);
    connect(displayAlgorithmsButton, &QPushButton::clicked, this, &GraphWidget::displayAlgorithms);
    sidebarLayout->addWidget(displayAlgorithmsButton);

    QPushButton *resetButton = new QPushButton(tr("Reset Graph"), this);
    resetButton->setStyleSheet(buttonStyle);
    connect(resetButton, &QPushButton::clicked, this, &GraphWidget::resetHighlights);
    sidebarLayout->addWidget(resetButton);


    QPushButton *exitButton = new QPushButton(tr("Exit to Main Menu"), this);
    exitButton->setStyleSheet(buttonStyle);
    connect(exitButton, &QPushButton::clicked, this, &GraphWidget::exitToMainMenu);
    sidebarLayout->addWidget(exitButton);

    QWidget *sidebar = new QWidget;
    sidebar->setLayout(sidebarLayout);
    sidebar->setStyleSheet("background-color: #555555;"
                           "border-radius: 15px;");

    graph = new GraphGUI;

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(graph, 1);

    setLayout(mainLayout);

}

    void Save_GUI_Data(const QString &filename) {
    graph->saveToFileGUI(filename);
}

    signals:
        void exitToMainMenu();
public slots:
    void addPoint()
    {
        bool ok;
        QString name = QInputDialog::getText(this, tr("Point Name"),
                                             tr("Enter name for point:"), QLineEdit::Normal,
                                             QString(), &ok);
        string sname = name.toStdString();
        if (ok && !name.isEmpty())
        {
            //Generating A Random Point
            int x = QRandomGenerator::global()->bounded(graph->width());
            int y = QRandomGenerator::global()->bounded(graph->height());
            QPoint newPoint(x, y);
            //Checking The Point In The Backend
            return_value = UI.AddCity(sname,Graph);
            //Success
            if (return_value == 0) {
            graph->points.push_back(newPoint);
            graph->pointNames[newPoint] = name;
            QPointF relativePoint((qreal)x / graph->width(), (qreal)y / graph->height());
            graph->relativePoints.push_back(qMakePair(relativePoint, name));
            graph->update();
                QMessageBox::information(this, tr("Success"), tr("City Added Successfully."));
            }
            //Fail
            else if (return_value == 1) {
                QMessageBox::warning(this, tr("Error"), tr("Adding The City Failed."));
            }
            //City Already Exists
            else if (return_value == 2) {
                QMessageBox::warning(this, tr("Error"), tr("City Already Exists."));
            }
        }
    }

    void clearPoints()
    {
        //Checking The Graph In The Backend
        return_value = UI.ClearMap(Graph);
        //Success
        if (return_value == 0) {
            graph->points.clear();
            graph->pointNames.clear();
            graph->lines.clear();
            graph->relativePoints.clear();
            QMessageBox::information(this, tr("Success"), tr("Graph Cleared Successfuly."));
            cout<< Graph.citycount<<endl;
        }
        //Fail
        else if (return_value == 1) {
            QMessageBox::warning(this, tr("Error"), tr("Graph Is Empty."));
        }
    }

    void connectPoints()
    {
        //Getting List Of All Names In The Graph
        QStringList pointNamesList;
        for (const auto &pair : graph->pointNames) {
            pointNamesList << pair.second;
        }

        //Getting Input From User
        ConnectPointsDialog dialog(pointNamesList, this);
        if (dialog.exec() == QDialog::Accepted) {
            QString point1Name = dialog.point1Name();
            QString point2Name = dialog.point2Name();
            double value = dialog.value();
            //Searching For The Two Points Selected
            QPoint point1, point2;
            for (const auto &pair : graph->pointNames) {
                if (pair.second == point1Name) point1 = pair.first;
                if (pair.second == point2Name) point2 = pair.first;
            }
            //Checking The Two Points In The Backend
            return_value = UI.AddEdge(point1Name.toStdString(),point2Name.toStdString(),value,Graph);
            //Success
            if (return_value == 0 ) {
            graph->lines.push_back(qMakePair(qMakePair(point1, point2), value));
            graph->update();
            QMessageBox::information(this, tr("Success"), tr("Connection Added Successfully."));
            }
            //Fail
            else if (return_value == 1){
                QMessageBox::warning(this, tr("Error"), tr("Adding The Connection Failed."));
            }
            //One Of The Cities Does Not Exist
            else if (return_value == 2){
                QMessageBox::warning(this, tr("Error"), tr("One Of The Cities Doesn't Exist."));
            }
            //Connection Already Exists
            else if (return_value == 3){
                QMessageBox::warning(this, tr("Error"), tr("Connection Already Exists."));
            }
            else if (return_value == 4){
                QMessageBox::warning(this, tr("Error"), tr("Distance Is Empty Or Wrong."));
            }
            else if (return_value == 5){
                QMessageBox::warning(this, tr("Error"), tr("Can't Connect A City To Itself."));
            }
            //TODO If value == 0
        }
    }

    void editLine()
{
    //Getting The List Of Connected City Names
    QStringList lineNamesList;
    for (const auto &line : graph->lines) {
        lineNamesList << (graph->pointNames[line.first.first] + "-" + graph->pointNames[line.first.second]);
    }

    bool ok;
    QString name = QInputDialog::getItem(this, tr("Edit Line"),
                                         tr("Select line to edit:"), lineNamesList, 0, false, &ok);
    if (ok && !name.isEmpty())
    {
        for (auto &line : graph->lines) {
            //Searching For The Chosen Connection
            if ((graph->pointNames[line.first.first] + "-" + graph->pointNames[line.first.second]) == name) {
                bool ok;
                double value = QInputDialog::getDouble(this, tr("Edit Line Value"),
                                                       tr("New value:"), line.second, -10000, 10000, 2, &ok);
                if (ok) {
                    //Checking For The Connection In The Backend
                    return_value = UI.EditEdge(graph->pointNames[line.first.first].toStdString(),graph->pointNames[line.first.second].toStdString(),value,Graph);
                    //Success
                    if (return_value == 0) {
                        line.second = value;
                        QMessageBox::information(this, tr("Success"), tr("Connection Edited Successfully."));
                    }
                    //Fail
                    else {
                        QMessageBox::warning(this, tr("Error"), tr("Editing The Connection Failed."));
                    }
                    graph->update();
                }
                break;
            }
        }
    }
}

    void removeSpecificPoint()
{
    //Getting List Of Cities
    QStringList pointNamesList;
    for (const auto &pair : graph->pointNames) {
        pointNamesList << pair.second;
    }

    bool ok;
    QString name = QInputDialog::getItem(this, tr("Remove Point"),
                                         tr("Select point to remove:"), pointNamesList, 0, false, &ok);
    string sname = name.toStdString();
    if (ok && !name.isEmpty())
    {
        //Searching For The Point
        for (int i = 0; i < graph->points.size(); ++i) {
            if (graph->pointNames[graph->points[i]] == name) {
                //Checking The Point In The Backend
                return_value = UI.DeleteCity(sname,Graph);
                //Success
                if (return_value == 0) {
                    QPoint removedPoint = graph->points[i];
                    graph->pointNames.erase(graph->points[i]);
                    graph->points.remove(i);
                    graph->relativePoints.remove(i);
                    // Remove the lines connected to the removed point
                    for (int j = graph->lines.size() - 1; j >= 0; --j) {
                        if (graph->lines[j].first.first == removedPoint || graph->lines[j].first.second == removedPoint) {
                            graph->lines.remove(j);
                        }
                    }
                    QMessageBox::information(this, tr("Success"), tr("City Deleted Successfully."));
                }
                //Fail
                else {
                    QMessageBox::warning(this, tr("Error"), tr("Deleting The City Failed."));
                }
                graph->update();
                break;
            }
        }
    }
}

    void removeSpecificLine()
{
    //Getting List Of Lines
    QStringList lineNamesList;
    for (const auto &line : graph->lines) {
        lineNamesList << (graph->pointNames[line.first.first] + "-" + graph->pointNames[line.first.second]);
    }

    bool ok;
    QString name = QInputDialog::getItem(this, tr("Remove Line"),
                                         tr("Select line to remove:"), lineNamesList, 0, false, &ok);
    if (ok && !name.isEmpty())
    {
        for (int i = 0; i < graph->lines.size(); ++i) {
            //Searching For The Connection
            if ((graph->pointNames[graph->lines[i].first.first] + "-" + graph->pointNames[graph->lines[i].first.second]) == name) {
                //Checking The Connection In The Backend
                return_value = UI.DeleteEdge(graph->pointNames[graph->lines[i].first.first].toStdString(),graph->pointNames[graph->lines[i].first.second].toStdString(),Graph);
                //Success
                if (return_value == 0) {
                    graph->lines.remove(i);
                    QMessageBox::information(this, tr("Success"), tr("Connection Deleted Successfully."));
                }
                //Fail
                else {
                    QMessageBox::warning(this, tr("Error"), tr("Deleting The Connection Failed."));
                }
                graph->update();
                break;
            }
        }
    }
}

    void displayAlgorithms() {
    QStringList algorithmNamesList;
    // Add your algorithm names here
    algorithmNamesList << "Breadth First Search" << "Depth First Search" << "Dijkestra's Algorithm" << "Prim's Algorithm";

    QStringList pointNamesList;
    for (const auto &pair : graph->pointNames) {
        pointNamesList << pair.second;
    }

    bool ok;
    QString algorithmName = QInputDialog::getItem(this, tr("Select Algorithm"),
                                                  tr("Select an algorithm:"), algorithmNamesList, 0, false, &ok);
    if (ok && algorithmName=="Prim's Algorithm") {
        QStringList cityPairs;
        for (const auto &str : UI.Prims(Graph).second) {
            cityPairs << QString::fromStdString(str);
        }
        graph->highlightLines(cityPairs);
        QString MST = QString::fromStdString("Total Weight Of MST: "+UI.Prims(Graph).first);
        QMessageBox::information(this, tr("Prim's Algorithm"), MST);
        cout<<"Prim's Algorithm\n";
    }
    else if (ok && !algorithmName.isEmpty() ) {
        QString pointName = QInputDialog::getItem(this, tr("Select Starting Point"),
                                                  tr("Select a starting point:"), pointNamesList, 0, false, &ok);
        if ((ok && !pointName.isEmpty() )) {
            if (algorithmName=="Breadth First Search") {
                QString city = QString::fromStdString(UI.TraverseBfs(pointName.toStdString(),Graph));
                QMessageBox::information(this, tr("BFS"), city);
                cout<<"Breadth First Search\n";
            }
            else if (algorithmName=="Depth First Search") {
                QString city = QString::fromStdString(UI.TraverseDfs(pointName.toStdString(),Graph));
                QMessageBox::information(this, tr("DFS"), city);
                cout<<"Depth First Search\n";
            }
            else if (algorithmName=="Dijkestra's Algorithm") {
                QStringList cityPairs;
                for (const auto &str : UI.Prims(Graph).second) {
                    cityPairs << QString::fromStdString(str);
                }
                graph->highlightLines(cityPairs);
                QString city ="Shortest Distances From "+pointName+"\n"+QString::fromStdString(UI.Dijkstra(pointName.toStdString(),Graph));
                QMessageBox::information(this, tr("Dijkestra"), city);
                cout<<"Dijkestra's Algorithm\n";
            }
        }
    }
}

    void resetHighlights() {
    graph->highlightedLines.clear();
    graph->update();
}
    void displayDistance() {
    QString TotalDistance = QString::fromStdString("Total Distance Of The Graph: "+UI.DisplayTotalDistance(Graph));
    QMessageBox::information(this, tr("Total Distance"), TotalDistance);
}

    void Save_Graph_Data(const string &filename) {
    ofstream file(filename);
    if (!file.is_open())
        return;

    // Save cities
    for (const auto &city : Graph.mymap) {
        file << city.first << '\n';
    }
    file << "EDGES\n";
    // Save edges
    for (const auto &city : Graph.mymap) {
        for (const auto &edge : city.second) {
            if (city.first < edge.first) {
                file << city.first << ' ' << edge.first << ' ' << edge.second << '\n';
            }
        }
    }
}

    void load_Graph_Data(const string &filename ) {
    ifstream file(filename);
    if (!file.is_open())
        return;

    Graph.mymap.clear();
    Graph.citycount = 0;
    string line;
    // Load cities
    while (getline(file, line)) {
        if (line == "EDGES") break;
        Graph.addcity(line);
    }
    // Load edges
    string city1, city2;
    int km;
    while (file >> city1 >> city2 >> km) {
        Graph.addedge(city1, city2, km);
    }
}

    void showEvent(QShowEvent *event) override {
    cout<<"Fetching Data\n";
    graph->Load_GUI_Data("../Data/GUIgraph_data.txt");
    load_Graph_Data("../Data/graph_data.txt");
    Graph.printadjcentlist();
    QWidget::showEvent(event);
}


private:
    GraphGUI *graph;
};