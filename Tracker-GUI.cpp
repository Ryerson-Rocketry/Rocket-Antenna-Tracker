#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <cmath>
#include "qcustomplot.h"  // Install QCustomPlot for real-time graphing

constexpr float EARTH_RADIUS = 6371000.0f;
constexpr float DEG_TO_RAD = M_PI / 180.0f;

class LandingPage : public QWidget {
    Q_OBJECT

public:
    LandingPage(QWidget *parent = nullptr);
    float gsLat, gsLon, gsAlt;

signals:
    void groundStationSet(float lat, float lon, float alt);

private slots:
    void saveCoordinates();

private:
    QLineEdit *latInput, *lonInput, *altInput;
};

LandingPage::LandingPage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Enter Ground Station Coordinates", this);
    latInput = new QLineEdit(this);
    lonInput = new QLineEdit(this);
    altInput = new QLineEdit(this);
    QPushButton *submitButton = new QPushButton("Start Tracking", this);

    layout->addWidget(title);
    layout->addWidget(new QLabel("Ground Station Latitude:"));
    layout->addWidget(latInput);
    layout->addWidget(new QLabel("Ground Station Longitude:"));
    layout->addWidget(lonInput);
    layout->addWidget(new QLabel("Ground Station Altitude (m):"));
    layout->addWidget(altInput);
    layout->addWidget(submitButton);
    
    connect(submitButton, &QPushButton::clicked, this, &LandingPage::saveCoordinates);
}

void LandingPage::saveCoordinates() {
    gsLat = latInput->text().toFloat();
    gsLon = lonInput->text().toFloat();
    gsAlt = altInput->text().toFloat();
    emit groundStationSet(gsLat, gsLon, gsAlt);
}

class RocketTracker : public QMainWindow {
    Q_OBJECT

public:
    RocketTracker(float lat, float lon, float alt, QWidget *parent = nullptr);

private slots:
    void calculateAzimuthElevation();
    void updatePlot();

private:
    float gsLat, gsLon, gsAlt;
    QLineEdit *latInput, *lonInput, *altInput;
    QLabel *azimuthLabel, *elevationLabel;
    QCustomPlot *plot;
    QTimer *timer;
    QVector<double> azimuthData, elevationData, timeData;
    int timeStep;
};

RocketTracker::RocketTracker(float lat, float lon, float alt, QWidget *parent) : QMainWindow(parent), gsLat(lat), gsLon(lon), gsAlt(alt), timeStep(0) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    latInput = new QLineEdit(this);
    lonInput = new QLineEdit(this);
    altInput = new QLineEdit(this);
    QPushButton *calculateButton = new QPushButton("Calculate", this);
    azimuthLabel = new QLabel("Azimuth: -", this);
    elevationLabel = new QLabel("Elevation: -", this);
    plot = new QCustomPlot(this);
    
    layout->addWidget(new QLabel("Rocket Latitude:"));
    layout->addWidget(latInput);
    layout->addWidget(new QLabel("Rocket Longitude:"));
    layout->addWidget(lonInput);
    layout->addWidget(new QLabel("Rocket Altitude (m):"));
    layout->addWidget(altInput);
    layout->addWidget(calculateButton);
    layout->addWidget(azimuthLabel);
    layout->addWidget(elevationLabel);
    layout->addWidget(plot);
    
    setCentralWidget(centralWidget);
    
    connect(calculateButton, &QPushButton::clicked, this, &RocketTracker::calculateAzimuthElevation);
    
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->addGraph();
    plot->graph(1)->setPen(QPen(Qt::red));
    plot->xAxis->setLabel("Time Step");
    plot->yAxis->setLabel("Degrees");
    plot->legend->setVisible(true);
    
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RocketTracker::updatePlot);
    timer->start(1000);
}

void RocketTracker::calculateAzimuthElevation() {
    float rocketLat = latInput->text().toFloat();
    float rocketLon = lonInput->text().toFloat();
    float rocketAlt = altInput->text().toFloat();

    float lat1 = gsLat * DEG_TO_RAD, lon1 = gsLon * DEG_TO_RAD;
    float lat2 = rocketLat * DEG_TO_RAD, lon2 = rocketLon * DEG_TO_RAD;
    float dLon = lon2 - lon1;

    float y = sin(dLon) * cos(lat2);
    float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
    float azimuth = atan2(y, x) * (180.0f / M_PI);
    if (azimuth < 0) azimuth += 360.0f;

    float dLat = lat2 - lat1;
    float a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1) * cos(lat2) * sin(dLon / 2) * sin(dLon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float distance = EARTH_RADIUS * c;
    float elevation = atan2(rocketAlt - gsAlt, distance) * (180.0f / M_PI);

    azimuthLabel->setText(QString("Azimuth: %1°").arg(azimuth));
    elevationLabel->setText(QString("Elevation: %1°").arg(elevation));

    azimuthData.append(azimuth);
    elevationData.append(elevation);
    timeData.append(timeStep++);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QStackedWidget stackedWidget;
    LandingPage landingPage;
    RocketTracker *tracker = nullptr;
    
    QObject::connect(&landingPage, &LandingPage::groundStationSet, [&](float lat, float lon, float alt) {
        tracker = new RocketTracker(lat, lon, alt);
        stackedWidget.addWidget(tracker);
        stackedWidget.setCurrentWidget(tracker);
    });
    
    stackedWidget.addWidget(&landingPage);
    stackedWidget.setCurrentWidget(&landingPage);
    stackedWidget.show();
    
    return app.exec();
}
