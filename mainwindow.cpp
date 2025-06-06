#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
// QProcess is already included via mainwindow.h

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ... (constructor code from previous step, including UI setup, styling, config loading, signal/slot connections) ...
    // Window Properties
    setWindowTitle("野战模组化人机协同手术机器人系统");
    resize(1200, 800);

    // Central Widget and Main Layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Title Label
    titleLabel = new QLabel("野战模组化人机协同手术机器人系统", this);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Buttons
    moduleButton1 = new QPushButton("颅内血肿穿刺减压机器人模组", this);
    moduleButton2 = new QPushButton("介入止血手术机器人模组", this);

    // Button Layout
    buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(moduleButton1);
    buttonLayout->addSpacing(40);
    buttonLayout->addWidget(moduleButton2);
    buttonLayout->addStretch();

    // Add title and button layout to the main layout
    mainLayout->addStretch(1);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(60);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch(2);

    mainLayout->setContentsMargins(50, 30, 50, 50);
    centralWidget->setLayout(mainLayout);

    // Apply Stylesheet
    QString styleSheet = R"(
        QMainWindow {
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(200, 255, 220, 255), stop:1 rgba(150, 230, 190, 255));
        }
        QLabel#TitleLabel {
            font-size: 32px; font-weight: bold; color: #005030; padding-bottom: 20px;
        }
        QPushButton {
            background-color: rgba(255, 255, 255, 0.85); border: 1px solid #B0BEC5;
            border-radius: 15px; padding: 25px 35px; font-size: 18px; color: #263238;
            min-width: 350px; min-height: 100px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 1); border: 1px solid #00796B;
        }
        QPushButton:pressed {
            background-color: rgba(230, 245, 235, 0.85);
        }
    )";
    this->setStyleSheet(styleSheet);

    process = new QProcess(this); // Initialize QProcess

    // Connect QProcess signals for error handling and state changes (optional but good)
    connect(process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error){
        Q_UNUSED(error); // error variable might not be used explicitly in this simple handler
        QMessageBox::critical(this, "Process Error", QString("Failed to start process: %1").arg(process->errorString()));
    });

    loadConfig(); // Load configuration

    // Connect button signals to slots
    connect(moduleButton1, &QPushButton::clicked, this, &MainWindow::onModuleButton1Clicked);
    connect(moduleButton2, &QPushButton::clicked, this, &MainWindow::onModuleButton2Clicked);
}

MainWindow::~MainWindow()
{
    // process is parented, will be deleted automatically.
    // If process is running, you might want to ensure it's killed.
    if (process->state() == QProcess::Running) {
        process->terminate(); // or kill()
        process->waitForFinished(1000); // Wait for 1 sec
    }
}

void MainWindow::loadConfig()
{
    QString configFilePath = QDir::currentPath() + "/config.ini";
    QSettings settings(configFilePath, QSettings::IniFormat);

    if (!QFile::exists(configFilePath)) {
        qWarning() << "Configuration file not found at:" << configFilePath;
        QMessageBox::warning(this, "Configuration Error",
                             QString("Configuration file (config.ini) not found in %1. Please create it.").arg(QDir::currentPath()));
        module1ExePath = "";
        module2ExePath = "";
        return;
    }

    settings.beginGroup("Paths");
    module1ExePath = settings.value("Module1Exe", "").toString();
    module2ExePath = settings.value("Module2Exe", "").toString();
    settings.endGroup();

    if (module1ExePath.isEmpty()) {
        qWarning() << "Module1Exe path is missing in config.ini";
        QMessageBox::warning(this, "Configuration Warning", "Module1Exe path is missing or empty in config.ini.");
    } else {
        qDebug() << "Config loaded: Module1Exe =" << module1ExePath;
    }

    if (module2ExePath.isEmpty()) {
        qWarning() << "Module2Exe path is missing in config.ini";
        QMessageBox::warning(this, "Configuration Warning", "Module2Exe path is missing or empty in config.ini.");
    } else {
        qDebug() << "Config loaded: Module2Exe =" << module2ExePath;
    }
}

void MainWindow::onModuleButton1Clicked()
{
    qDebug() << "Module 1 button clicked. Attempting to launch:" << module1ExePath;
    if (module1ExePath.isEmpty()) {
        QMessageBox::warning(this, "Execution Error", "Path for Module 1 is not configured.");
        return;
    }

    // For .bat or .sh files, or executables needing a specific working directory,
    // it's often better to use QProcess::startDetached or setWorkingDirectory.
    // For simple .exe, start() is fine.
    // Using startDetached also prevents the GUI from freezing if the launched process is long-running.
    if (!QProcess::startDetached(module1ExePath)) {
        // Note: QProcess::errorOccurred signal should handle detailed errors if connected.
        // This is a fallback message if startDetached itself returns false immediately.
        QMessageBox::critical(this, "Execution Error", QString("Failed to start Module 1: %1").arg(module1ExePath));
    }
}

void MainWindow::onModuleButton2Clicked()
{
    qDebug() << "Module 2 button clicked. Attempting to launch:" << module2ExePath;
    if (module2ExePath.isEmpty()) {
        QMessageBox::warning(this, "Execution Error", "Path for Module 2 is not configured.");
        return;
    }

    if (!QProcess::startDetached(module2ExePath)) {
        QMessageBox::critical(this, "Execution Error", QString("Failed to start Module 2: %1").arg(module2ExePath));
    }
}
