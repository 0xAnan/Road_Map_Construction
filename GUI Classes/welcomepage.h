#include <QApplication>  // For the application object
#include <QLabel>  // For the welcome label
#include <QWidget>  // Base class for all user interface objects
#include <QPushButton>  // For the start and exit buttons
#include <QVBoxLayout>  // For the vertical layout
#include <QDialog>  // Base class for dialogs
#include <QPixmap>  // For the background image
#include <QResizeEvent>  // For the resize event

class WelcomePage : public QDialog
{
    Q_OBJECT  // Enables signals and slots for this class

public:
    explicit WelcomePage(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        // Set window flags to allow maximizing, minimizing and closing the window
        this->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

        // Create a new vertical layout with no spacing between widgets
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(0);

        // Create a welcome label with custom font and color, and add it to the layout
        QLabel *welcomeLabel = new QLabel("Welcome to the Graph Application!", this);
        welcomeLabel->setAlignment(Qt::AlignCenter);
        QFont font = welcomeLabel->font();
        font.setPointSize(24);
        font.setBold(true);
        welcomeLabel->setFont(font);
        welcomeLabel->setStyleSheet("QLabel { color : #008000; }");
        layout->addWidget(welcomeLabel, 0, Qt::AlignCenter);

        // Add a stretchable space before the buttons
        layout->addStretch();

        // Create a start button with custom style, and add it to the layout
        QPushButton *startButton = new QPushButton("Start App", this);
        startButton->setFixedWidth(200);
        startButton->setStyleSheet(
            "QPushButton {"
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
            "QPushButton:hover {background-color: #45a049;}"
        );
        // Connect the button's clicked signal to the dialog's accept slot
        connect(startButton, &QPushButton::clicked, this, &QDialog::accept);
        layout->addWidget(startButton, 0, Qt::AlignCenter);

        // Create an exit button with custom style, and add it to the layout
        QPushButton *exitButton = new QPushButton("Exit", this);
        exitButton->setFixedWidth(200);
        exitButton->setStyleSheet(
            "QPushButton {"
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
            "QPushButton:hover {background-color: #45a049;}"
        );
        // Connect the button's clicked signal to the dialog's reject slot
        connect(exitButton, &QPushButton::clicked, this, &QDialog::reject);
        layout->addWidget(exitButton, 0, Qt::AlignCenter);

        // Add a stretchable space after the buttons
        layout->addStretch();

        // Set the layout for the dialog
        setLayout(layout);

        // Load the background image
        backgroundImage.load("../Resources/Background.jpeg");
    }

protected:
    // Override the resize event to scale the background image
    void resizeEvent(QResizeEvent *event) override
    {
        // Scale the image to the size of the widget
        QPixmap scaledImage = backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(scaledImage));
        this->setPalette(palette);

        QDialog::resizeEvent(event);
    }

private:
    QPixmap backgroundImage;  // The background image
};
