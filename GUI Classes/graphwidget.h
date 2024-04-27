// Include necessary Qt modules
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <map>
#include <cmath>

// Comparator for QPoint
struct PointComparator {
    bool operator()(const QPoint &a, const QPoint &b) const {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    }
};

// Graph class inherits from QWidget and is responsible for drawing the points and lines
class Graph : public QWidget
{
    Q_OBJECT // Enables the use of signals and slots

public:
    // Constructor for the Graph class
    Graph(QWidget *parent = nullptr) : QWidget(parent) {
        // Set a minimum size for the Graph widget
        setMinimumSize(300, 300);
    }

    // Vectors to store the points
    QVector<QPoint> points;
    // Map to store point names
    std::map<QPoint, QString, PointComparator> pointNames;
    // List to store lines between points
    QList<QPair<QPoint, QPoint>> lines;

protected:
    // Overridden paint event to draw the graph
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw lines between points
        for (const auto &line : lines) {
            painter.drawLine(line.first, line.second);
        }

        // Draw points
        for (const auto &point : points) {
            painter.drawEllipse(point, 5, 5);
            // Draw the point name next to the point
            auto it = pointNames.find(point);
            if (it != pointNames.end()) {
                painter.drawText(point + QPoint(10, -10), it->second);
            }
        }
    }

    // Overridden show event to generate initial points
    void showEvent(QShowEvent *event) override
    {
        // Generate 10 random points
        for (int i = 0; i < 10; ++i) {
            int x = QRandomGenerator::global()->bounded(this->width());
            int y = QRandomGenerator::global()->bounded(this->height());
            QPoint point(x, y);
            points.push_back(point);
            pointNames[point] = "Point " + QString::number(i + 1);
        }
    }
};
// GraphWidget class inherits from QWidget and contains the Graph and buttons
class GraphWidget : public QWidget
{
    Q_OBJECT // Enables the use of signals and slots

public:
    // Constructor for the GraphWidget class
    GraphWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Create a layout for the sidebar with buttons
        QVBoxLayout *sidebarLayout = new QVBoxLayout;
        QPushButton *addButton = new QPushButton(tr("Add Point"), this); // Button to add points
        connect(addButton, &QPushButton::clicked, this, &GraphWidget::addPoint); // Connect button click to slot
        sidebarLayout->addWidget(addButton); // Add button to the sidebar layout

        QPushButton *removeButton = new QPushButton(tr("Remove Last Point"), this); // Button to remove last point
        connect(removeButton, &QPushButton::clicked, this, &GraphWidget::removePoint); // Connect button click to slot
        sidebarLayout->addWidget(removeButton); // Add button to the sidebar layout

        QPushButton *clearButton = new QPushButton(tr("Clear All Points"), this); // Button to clear all points
        connect(clearButton, &QPushButton::clicked, this, &GraphWidget::clearPoints); // Connect button click to slot
        sidebarLayout->addWidget(clearButton); // Add button to the sidebar layout

        QPushButton *connectButton = new QPushButton(tr("Connect Points"), this); // Button to connect points
        connect(connectButton, &QPushButton::clicked, this, &GraphWidget::connectPoints); // Connect button click to slot
        sidebarLayout->addWidget(connectButton); // Add button to the sidebar layout

        // Create the sidebar widget and set its layout
        QWidget *sidebar = new QWidget;
        sidebar->setLayout(sidebarLayout);
        sidebar->setStyleSheet("background-color: green;"); // Set the sidebar background color to green

        // Create the Graph object
        graph = new Graph;

        // Create the main layout and add the sidebar and graph to it
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(sidebar); // Add sidebar to the main layout
        mainLayout->addWidget(graph, 1); // Add graph to the main layout and allow it to expand

        // Set the main layout for the GraphWidget
        setLayout(mainLayout);
    }

// Slots for handling button clicks
public slots:
    void addPoint()
    {
        bool ok;
        // Prompt the user to enter a name for the point
        QString name = QInputDialog::getText(this, tr("Point Name"),
                                             tr("Enter name for point:"), QLineEdit::Normal,
                                             QString(), &ok);
        if (ok && !name.isEmpty())
        {
            // Get random coordinates for the new point
            int x = QRandomGenerator::global()->bounded(graph->width());
            int y = QRandomGenerator::global()->bounded(graph->height());
            // Add the new point to the graph
            QPoint newPoint(x, y);
            graph->points.push_back(newPoint);
            graph->pointNames[newPoint] = name;
            // Update the graph to redraw with the new point
            graph->update();
        }
    }


    void removePoint()
    {
        // Check if there are any points to remove
        if (!graph->points.isEmpty())
        {
            // Remove the last point and name from the graph
            QPoint lastPoint = graph->points.last();
            graph->points.pop_back();
            graph->pointNames.erase(lastPoint);
            // Update the graph to redraw without the removed point
            graph->update();
        }
    }

    void clearPoints()
    {
        // Clear all points, names, and lines from the graph
        graph->points.clear();
        graph->pointNames.clear();
        graph->lines.clear();
        // Update the graph to redraw without any points or lines
        graph->update();
    }


    void connectPoints()
    {
        bool ok;
        // Prompt the user to select two points to connect
        QStringList pointNamesList;
        for (const auto &pair : graph->pointNames) {
            pointNamesList << pair.second;
        }
        QString point1Name = QInputDialog::getItem(this, tr("Select Point 1"),
                                                   tr("Choose the first point to connect:"), pointNamesList, 0, false, &ok);
        if (!ok || point1Name.isEmpty()) return;

        QString point2Name = QInputDialog::getItem(this, tr("Select Point 2"),
                                                   tr("Choose the second point to connect:"), pointNamesList, 0, false, &ok);
        if (!ok || point2Name.isEmpty()) return;

        // Find the points by name
        QPoint point1, point2;
        for (const auto &pair : graph->pointNames) {
            if (pair.second == point1Name) point1 = pair.first;
            if (pair.second == point2Name) point2 = pair.first;
        }

        // Add the line to the list of lines
        graph->lines.push_back(qMakePair(point1, point2));
        // Update the graph to redraw with the new line
        graph->update();
    }

private:
    Graph *graph; // Pointer to the Graph object
};
