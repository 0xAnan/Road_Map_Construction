#include <QApplication>
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
#include <map>
#include <cmath>

struct PointComparator {
    bool operator()(const QPoint &a, const QPoint &b) const {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    }
};

class Graph : public QWidget
{
    Q_OBJECT

public:
    // Constructor for the Graph class
    Graph(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(300, 300);
    }

    QVector<QPoint> points;
    std::map<QPoint, QString, PointComparator> pointNames;
    QList<QPair<QPair<QPoint, QPoint>, double>> lines;

    void addRelativePoint(const QPointF& point) {
        relativePoints.push_back(point);
    }

    void removeRelativePoint() {
        if (!relativePoints.isEmpty()) {
            relativePoints.pop_back();
        }
    }
    void clearRelativePoints() {
        relativePoints.clear();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        for (const auto &line : lines) {
            painter.drawLine(line.first.first, line.first.second);
            QPoint midpoint = (line.first.first + line.first.second) / 2;
            painter.drawText(midpoint, QString::number(line.second));
        }

        for (const auto &point : points) {
            painter.drawEllipse(point, 5, 5);
            auto it = pointNames.find(point);
            if (it != pointNames.end()) {
                painter.drawText(point + QPoint(10, -10), it->second);
            }
        }
    }

    void showEvent(QShowEvent *event) override
    {
        for (int i = 0; i < 5; ++i) {
            addRandomPoint();
        }
    }

    void resizeEvent(QResizeEvent *event) override
    {
        updatePoints();
        QWidget::resizeEvent(event);
    }

private:
    QVector<QPointF> relativePoints;  // Stores points as percentages

    void addRandomPoint() {
        qreal margin = 0.1;  // adjust this value to change the margin
        qreal x = QRandomGenerator::global()->generateDouble() * (1.0 - 2*margin) + margin;
        qreal y = QRandomGenerator::global()->generateDouble() * (1.0 - 2*margin) + margin;
        QPointF point(x, y);
        relativePoints.push_back(point);
        updatePoints();
    }


    void updatePoints() {
        points.clear();
        pointNames.clear();
        for (int i = 0; i < relativePoints.size(); ++i) {
            int x = relativePoints[i].x() * width();
            int y = relativePoints[i].y() * height();
            QPoint point(x, y);
            points.push_back(point);
            pointNames[point] = "Point " + QString::number(i + 1);
        }
    }
};


class ConnectPointsDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectPointsDialog(const QStringList &pointNames, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        point1ComboBox = new QComboBox;
        point1ComboBox->addItems(pointNames);
        layout->addWidget(point1ComboBox);

        point2ComboBox = new QComboBox;
        point2ComboBox->addItems(pointNames);
        layout->addWidget(point2ComboBox);

        valueLineEdit = new QLineEdit;
        layout->addWidget(valueLineEdit);

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
    Q_OBJECT // Enables the use of signals and slots

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
    sidebarLayout->addWidget(addButton); // Add button to the sidebar layout

    QPushButton *removeButton = new QPushButton(tr("Remove Last Point"), this);
    removeButton->setStyleSheet(buttonStyle);
    connect(removeButton, &QPushButton::clicked, this, &GraphWidget::removePoint);
    sidebarLayout->addWidget(removeButton); // Add button to the sidebar layout

    QPushButton *clearButton = new QPushButton(tr("Clear All Points"), this);
    clearButton->setStyleSheet(buttonStyle);
    connect(clearButton, &QPushButton::clicked, this, &GraphWidget::clearPoints);
    sidebarLayout->addWidget(clearButton); // Add button to the sidebar layout

    QPushButton *connectButton = new QPushButton(tr("Connect Points"), this);
    connectButton->setStyleSheet(buttonStyle);
    connect(connectButton, &QPushButton::clicked, this, &GraphWidget::connectPoints);
    sidebarLayout->addWidget(connectButton);

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
        // Add the new point to the graph
        QPoint newPoint(x, y);
        graph->points.push_back(newPoint);
        graph->pointNames[newPoint] = name;
        // Add the new point to relativePoints
        QPointF relativePoint((qreal)x / graph->width(), (qreal)y / graph->height());
        graph->addRelativePoint(relativePoint);
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
        // Remove the last point from relativePoints
        graph->removeRelativePoint();
        graph->update();
    }
}


    void clearPoints()
{
    graph->points.clear();
    graph->pointNames.clear();
    graph->lines.clear();
    // Clear relativePoints as well
    graph->clearRelativePoints();
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

private:
    Graph *graph;
};