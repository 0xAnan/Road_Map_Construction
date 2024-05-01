#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QPaintEvent>

#include <map>
#include <cmath>

// Comparator for QPoint objects
struct PointComparator {
    bool operator()(const QPoint &a, const QPoint &b) const {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    }
};

// Graph class represents the graph visualization
class Graph : public QWidget
{
    Q_OBJECT

public:
    // Constructor for the Graph class
    Graph(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(300, 300);
    }

    QVector<QPoint> points;  // Stores the points in the graph
    std::map<QPoint, QString, PointComparator> pointNames;  // Stores the names of the points
    QList<QPair<QPair<QPoint, QPoint>, double>> lines;  // Stores the lines in the graph
    QVector<QPair<QPointF, QString>> relativePoints;  // Stores the points relative to the size of the widget

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

            update();
        }
    }

    // Event handler for mouse release events
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        // Stop dragging the point
        draggedPoint = nullptr;
    }

    // Event handler for context menu events
    void contextMenuEvent(QContextMenuEvent *event) override
    {
        // If a point is right-clicked, allow the user to edit its name
        for (auto &point : points) {
            if (QLineF(event->pos(), point).length() < 10.0) {
                bool ok;
                QString text = QInputDialog::getText(this, tr("Edit Point Name"),
                                                     tr("New name:"), QLineEdit::Normal,
                                                     pointNames[point], &ok);
                if (ok && !text.isEmpty()) {
                    // Update the point's name in pointNames
                    pointNames[point] = text;

                    // Update the corresponding point in relativePoints
                    for (auto &pair : relativePoints) {
                        if (pair.first == QPointF((qreal)point.x() / width(), (qreal)point.y() / height())) {
                            pair.second = text;
                            break;
                        }
                    }
                    update();
                }
                break;
            }
        }
    }

    // Event handler for paint events
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QPen pen;  // Declare the QPen object here

        QFont font = painter.font() ;
        font.setPointSize ( 14 ); // Set the size of font
        font.setBold(true); // Make font bold
        painter.setFont(font); // Set the font

        // Draw lines
        for (const auto &line : lines) {
            painter.drawLine(line.first.first, line.first.second);
            QPoint midpoint = (line.first.first + line.first.second) / 2;

            // Set the color of the text to be more visible, e.g., white
            pen.setColor(Qt::white);
            painter.setPen(pen);

            painter.drawText(midpoint, QString::number(line.second));
        }

        // Reset the color of the text for other drawings
        pen.setColor(Qt::white);
        painter.setPen(pen);

        // Draw points
        for (const auto &point : points) {
            painter.drawEllipse(point, 5, 5);
            auto it = pointNames.find(point);
            if (it != pointNames.end()) {
                painter.drawText(point + QPoint(10, -10), it->second);
            }
        }
    }

    // Event handler for show events
    void showEvent(QShowEvent *event) override
    {
        // Add random points if none exist
        if (points.isEmpty()) {
            for (int i = 0; i < 5; ++i) {
                addRandomPoint();
            }
        }
    }

    // Event handler for resize events
    void resizeEvent(QResizeEvent *event) override
    {
        // Update the positions of the points
        updatePoints();
        QWidget::resizeEvent(event);
    }

private:
    QPoint *draggedPoint = nullptr;  // The point currently being dragged

    // Add a random point to the graph
    void addRandomPoint() {
        qreal margin = 0.1;
        qreal x = QRandomGenerator::global()->generateDouble() * (1.0 - 2*margin) + margin;
        qreal y = QRandomGenerator::global()->generateDouble() * (1.0 - 2*margin) + margin;
        QPointF point(x, y);
        relativePoints.push_back(qMakePair(point, "Point " + QString::number(relativePoints.size() + 1)));
        updatePoints();
    }

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
                          "display: inline-block;"
                          "font-size: 16px;"
                          "margin: 4px 2px;"
                          "cursor: pointer;"
                          "border-radius: 12px;}"
                          "QPushButton:hover {background-color: #45a049;}";

    QPushButton *addButton = new QPushButton(tr("Add Point"), this);
    addButton->setStyleSheet(buttonStyle);
    connect(addButton, &QPushButton::clicked, this, &GraphWidget::addPoint);
    sidebarLayout->addWidget(addButton);

    QPushButton *clearButton = new QPushButton(tr("Clear All Points"), this);
    clearButton->setStyleSheet(buttonStyle);
    connect(clearButton, &QPushButton::clicked, this, &GraphWidget::clearPoints);
    sidebarLayout->addWidget(clearButton);

    QPushButton *connectButton = new QPushButton(tr("Connect Points"), this);
    connectButton->setStyleSheet(buttonStyle);
    connect(connectButton, &QPushButton::clicked, this, &GraphWidget::connectPoints);
    sidebarLayout->addWidget(connectButton);

    QPushButton *removePointButton = new QPushButton(tr("Remove Point"), this);
    removePointButton->setStyleSheet(buttonStyle);
    connect(removePointButton, &QPushButton::clicked, this, &GraphWidget::removeSpecificPoint);
    sidebarLayout->addWidget(removePointButton);

    QPushButton *removeLineButton = new QPushButton(tr("Remove Line"), this);
    removeLineButton->setStyleSheet(buttonStyle);
    connect(removeLineButton, &QPushButton::clicked, this, &GraphWidget::removeSpecificLine);
    sidebarLayout->addWidget(removeLineButton);

    QPushButton *editLineButton = new QPushButton(tr("Edit Line"), this);
    editLineButton->setStyleSheet(buttonStyle);
    connect(editLineButton, &QPushButton::clicked, this, &GraphWidget::editLine);
    sidebarLayout->addWidget(editLineButton);

    QPushButton *exitButton = new QPushButton(tr("Exit to Main Menu"), this);
    exitButton->setStyleSheet(buttonStyle);
    connect(exitButton, &QPushButton::clicked, this, &GraphWidget::exitToMainMenu);
    sidebarLayout->addWidget(exitButton);

    QWidget *sidebar = new QWidget;
    sidebar->setLayout(sidebarLayout);
    sidebar->setStyleSheet("background-color: #555555;"
                           "border-radius: 15px;");

    graph = new Graph;

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(graph, 1);

    setLayout(mainLayout);
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
        if (ok && !name.isEmpty())
        {
            int x = QRandomGenerator::global()->bounded(graph->width());
            int y = QRandomGenerator::global()->bounded(graph->height());
            QPoint newPoint(x, y);
            graph->points.push_back(newPoint);
            graph->pointNames[newPoint] = name;
            QPointF relativePoint((qreal)x / graph->width(), (qreal)y / graph->height());
            graph->relativePoints.push_back(qMakePair(relativePoint, name));
            graph->update();
        }
    }

    void removePoint()
    {
        if (!graph->points.isEmpty())
        {
            QPoint lastPoint = graph->points.last();
            graph->points.pop_back();
            graph->pointNames.erase(lastPoint);
            graph->relativePoints.pop_back();
            graph->update();
        }
    }

    void clearPoints()
    {
        graph->points.clear();
        graph->pointNames.clear();
        graph->lines.clear();
        graph->relativePoints.clear();
        graph->update();
    }

    void connectPoints()
    {
        QStringList pointNamesList;
        for (const auto &pair : graph->pointNames) {
            pointNamesList << pair.second;
        }
        ConnectPointsDialog dialog(pointNamesList, this);
        if (dialog.exec() == QDialog::Accepted) {
            QString point1Name = dialog.point1Name();
            QString point2Name = dialog.point2Name();
            double value = dialog.value();

            QPoint point1, point2;
            for (const auto &pair : graph->pointNames) {
                if (pair.second == point1Name) point1 = pair.first;
                if (pair.second == point2Name) point2 = pair.first;
            }

            graph->lines.push_back(qMakePair(qMakePair(point1, point2), value));
            graph->update();
        }
    }
    void editLine()
{
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
            if ((graph->pointNames[line.first.first] + "-" + graph->pointNames[line.first.second]) == name) {
                bool ok;
                double value = QInputDialog::getDouble(this, tr("Edit Line Value"),
                                                       tr("New value:"), line.second, -10000, 10000, 2, &ok);
                if (ok) {
                    line.second = value;
                    graph->update();
                }
                break;
            }
        }
    }
}
    void removeSpecificPoint()
{
    QStringList pointNamesList;
    for (const auto &pair : graph->pointNames) {
        pointNamesList << pair.second;
    }

    bool ok;
    QString name = QInputDialog::getItem(this, tr("Remove Point"),
                                         tr("Select point to remove:"), pointNamesList, 0, false, &ok);
    if (ok && !name.isEmpty())
    {
        for (int i = 0; i < graph->points.size(); ++i) {
            if (graph->pointNames[graph->points[i]] == name) {
                graph->pointNames.erase(graph->points[i]);
                graph->points.remove(i);
                graph->relativePoints.remove(i);
                graph->update();
                break;
            }
        }
    }
}
    void removeSpecificLine()
{
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
            if ((graph->pointNames[graph->lines[i].first.first] + "-" + graph->pointNames[graph->lines[i].first.second]) == name) {
                graph->lines.remove(i);
                graph->update();
                break;
            }
        }
    }
}



private:
    Graph *graph;
};