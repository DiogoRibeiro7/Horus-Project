#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QFontDatabase>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "attitudeindicator.h"

class PFDMainWindow : public QMainWindow {
    Q_OBJECT

public:
    PFDMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Horus Project - UAV Primary Flight Display");
        resize(1920, 1000);
        // Central widget
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // Top info bar
        QHBoxLayout *topBar = new QHBoxLayout();
        altLabel = new QLabel("ALT: 0 m", this);
        speedLabel = new QLabel("SPD: 0 m/s", this);
        headingLabel = new QLabel("HDG: 0°", this);

        // Use custom font for labels
        QFont labelFont(nimbusMono, 16);
        altLabel->setFont(labelFont);
        speedLabel->setFont(labelFont);
        headingLabel->setFont(labelFont);

        topBar->addWidget(altLabel);
        topBar->addStretch();
        topBar->addWidget(speedLabel);
        topBar->addStretch();
        topBar->addWidget(headingLabel);

        mainLayout->addLayout(topBar);

        // Attitude indicator (main display)
        attitudeIndicator = new AttitudeIndicator(this);
        attitudeIndicator->setMinimumSize(1000, 1000);

        mainLayout->addWidget(attitudeIndicator, 1, Qt::AlignCenter);

        simTime = 0.02;

        // Initialize default values
        pitch = 0.0f;
        roll = 0.0f;
        altitude = 8500.0f;
        speed = 70.0f;
        heading = 0.0f;
        temperature = 25.0f;

        // Set fonts to AttitudeIndicator
        attitudeIndicator->setCustomFonts(PFDMainWindow::customFontFamily, nimbusMono);

        // Try to connect to ESP32
        setupSerialPort();
    }

    // Static member to hold font family name
    static QString customFontFamily;
    static QString nimbusMono;

private slots:
    void setupSerialPort() {
        serialPort = new QSerialPort(this);

        // Configure for your ESP32 port
        serialPort->setPortName("/dev/cu.usbserial-0001");
        serialPort->setBaudRate(QSerialPort::Baud115200);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if (serialPort->open(QIODevice::ReadOnly)) {

            // Connect signal for incoming data
            connect(serialPort, &QSerialPort::readyRead, this, &PFDMainWindow::readSerialData);

            // Start update timer for other data
            simTimer = new QTimer(this);
            connect(simTimer, &QTimer::timeout, this, &PFDMainWindow::updateDisplay);
            simTimer->start(20); // 50 Hz update rate
        } else {
            startSimulation();
        }
    }

    void readSerialData() {
    // Read all available data
    QByteArray data = serialPort->readAll();

    serialBuffer += data;

    // Process complete lines
    while (serialBuffer.contains('\n')) {
        int newlineIndex = serialBuffer.indexOf('\n');
        QString line = serialBuffer.left(newlineIndex).trimmed();
        serialBuffer.remove(0, newlineIndex + 1);

        if (!line.isEmpty()) {
            parseSerialLine(line);
        }
    }
}

    void parseSerialLine(const QString &line) {

    QStringList parts = line.split(',');

    if (parts.size() >= 2) {
        bool ok1, ok2;
        float newPitch = parts[0].toFloat(&ok1);  // REAL pitch from MPU6050
        float newRoll = parts[1].toFloat(&ok2);   // REAL roll from MPU6050

        if (ok1 && ok2) {
            // Update with REAL sensor data
            pitch = newPitch;
            roll = newRoll;

            // Debug output
            qDebug() << "MPU6050 - Pitch:" << pitch << "Roll:" << roll;

            static int frameCount = 0;
            frameCount++;
        } else {
            qWarning() << "Failed to parse pitch/roll:" << line;
        }
    } else {
        qWarning() << "Invalid data format:" << line;
    }
}

void updateDisplay() {
    // This runs at 50Hz and updates the display with REAL pitch/roll from ESP32

    // Simulate other parameters (altitude, speed, etc.)
    simTime += 0.02;

    speed = 70.0f + 30.0f * std::sin(simTime * 0.4);
    heading = std::fmod(simTime * 10.0, 360.0);
    float batteryState = 4.2f + 0.2f * std::sin(simTime * 5);
    int propQuantity = 4;
    float batteryLevel = 0.56f + 0.1f * std::sin(simTime * 0.02);
    int rpm[4] = {
        static_cast<int>(2500 + 500.0f * std::sin(simTime * 0.2)),
        static_cast<int>(2500 + 400.0f * std::sin(simTime * 0.25)),
        static_cast<int>(2500 + 450.0f * std::sin(simTime * 0.27)),
        static_cast<int>(2500 + 480.0f * std::sin(simTime * 0.29))
    };
    float QNH = 29.92f + 0.1f * std::sin(simTime * 0.3);
    altitude = 8500.00f + 100.0f * std::sin(simTime * 0.2);
    float OAT = 15.0f; // Fixed outside air temperature
    std::string flightMode = "MANUAL - MPU6050 Active";

    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::stringstream ss;
    ss << std::put_time(localTime, "%H:%M:%S");
    std::string timeStr = ss.str();

    // *** USE REAL PITCH AND ROLL FROM MPU6050 ***
    attitudeIndicator->setAttitude(pitch, roll, altitude, speed, heading,
                                  QNH, flightMode, timeStr, rpm,
                                  batteryState, batteryLevel, propQuantity, OAT);

    // Update top labels
    altLabel->setText(QString("ALT: %1 ft").arg(altitude, 0, 'f', 1));
    speedLabel->setText(QString("SPD: %1 kts").arg(speed, 0, 'f', 1));
    headingLabel->setText(QString("Pitch:%1° Roll:%2°").arg(pitch, 0, 'f', 1).arg(roll, 0, 'f', 1));
}

    void startSimulation() {
        // Fallback simulation mode if ESP32 not connected
        simTimer = new QTimer(this);
        connect(simTimer, &QTimer::timeout, this, &PFDMainWindow::updateSimulation);
        simTimer->start(20);
    }

    void updateSimulation() {
        // Full simulation mode (when ESP32 not connected)
        simTime += 0.02;

        pitch = 90.0f * std::sin(simTime * 0.2);
        roll = 180.0f * std::sin(simTime * 0.5);
        speed = 70.0f + 230.0f * std::sin(simTime * 0.4);
        heading = std::fmod(simTime * 10.0, 360.0);

        float altitudeRate = 2000.0f;
        float batteryState = 4.2f + 1.0f * std::sin(simTime * 5);
        int propQuantity = 4;
        float batteryLevel = 0.56f + 1.0f * std::sin(simTime * 0.02);
        int rpm[4] = {
            static_cast<int>(2500 + 1560.0f * std::sin(simTime * 0.2)),
            static_cast<int>(2500 + 1210.0f * std::sin(simTime * 0.25)),
            static_cast<int>(2500 + 1543.0f * std::sin(simTime * 0.27)),
            static_cast<int>(2500 + 1673.0f * std::sin(simTime * 0.29))
        };
        float QNH = 29.92f + 1.0f * std::sin(simTime * 0.3);
        altitude = 8500.00f + 1000.0f * std::sin(simTime * 0.2);
        float OAT = 8.0f;
        std::string flightMode = "ATLC Takeoff Active";

        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        std::stringstream ss;
        ss << std::put_time(localTime, "%H:%M:%S");
        std::string timeStr = ss.str();

        attitudeIndicator->setAttitude(pitch, roll, altitude, speed, heading,
                                      QNH, flightMode, timeStr, rpm,
                                      batteryState, batteryLevel, propQuantity, OAT);

        altLabel->setText(QString("ALT: %1 ft").arg(altitude, 0, 'f', 1));
        speedLabel->setText(QString("SPD: %1 kts").arg(speed, 0, 'f', 1));
        headingLabel->setText(QString("Roll: %1°").arg(roll, 0, 'f', 0));
    }

private:
    AttitudeIndicator *attitudeIndicator;
    QLabel *altLabel;
    QLabel *speedLabel;
    QLabel *headingLabel;
    QLabel *statusLabel;
    QTimer *simTimer;
    QSerialPort *serialPort;
    QByteArray serialBuffer;
    double simTime;

    // Real-time sensor data from ESP32
    float pitch;
    float roll;
    float temperature;

    // Simulated data
    float altitude;
    float speed;
    float heading;
};

// Define static member
QString PFDMainWindow::customFontFamily = "Courier";
QString PFDMainWindow::nimbusMono = "Nimbus Mono PS";

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load custom fonts
    int fontId1 = QFontDatabase::addApplicationFont(":/fonts/armarurgt.ttf");
    int fontId2 = QFontDatabase::addApplicationFont(":/fonts/NimbusMono.otf");

    if (fontId1 != -1) {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId1);
        if (!families.isEmpty()) {
            PFDMainWindow::customFontFamily = families.at(0);
            qDebug() << "✅ Loaded armarurgt font:" << PFDMainWindow::customFontFamily;
        }
    } else {
        PFDMainWindow::customFontFamily = "Courier";
    }

    if (fontId2 != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId2);
        if (!fontFamilies.isEmpty()) {
            PFDMainWindow::nimbusMono = fontFamilies.at(0);
            qDebug() << "✅ Loaded NimbusMono font:" << PFDMainWindow::nimbusMono;
        }
    } else {
        PFDMainWindow::nimbusMono = "Nimbus Mono PS";
    }

    PFDMainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"