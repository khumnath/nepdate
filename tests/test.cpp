#include "../src/panchangacalculator.h"
#include <QCoreApplication>
#include <QDate>
#include <QDebug>
#include <QString>
#include <QVariantMap>
#include <iostream>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  PanchangaCalculator calc;
  QDate today = QDate(2024, 6, 14);
  bool generateMode = false;

  for (int i = 1; i < argc; ++i) {
      QString arg(argv[i]);
      if (arg == "--generate") {
          generateMode = true;
      } else {
          QDate parsed = QDate::fromString(arg, "yyyy-MM-dd");
          if (parsed.isValid()) {
              today = parsed;
          } else {
              std::cerr << "Invalid argument or date format. Use yyyy-MM-dd\n";
              return 1;
          }
      }
  }

  double lat = 27.7172;
  double lon = 85.3240;
  double tz = 5.75;

  QVariantMap result = calc.calculate(today, lat, lon, tz);

  QJsonObject jsonObj = QJsonObject::fromVariantMap(result);

  QString testDataPath = QDir("tests").exists() ? "tests/test-data.json" : "../tests/test-data.json";
  QFile file(testDataPath);

  if (generateMode) {
      if (file.open(QIODevice::WriteOnly)) {
          QJsonDocument doc(jsonObj);
          file.write(doc.toJson(QJsonDocument::Indented));
          file.close();
          std::cout << "Successfully generated baseline data for " << today.toString("yyyy-MM-dd").toStdString() 
                    << " to " << testDataPath.toStdString() << "\n";
          return 0;
      } else {
          std::cerr << "Failed to open " << testDataPath.toStdString() << " for writing\n";
          return 1;
      }
  }

  if (!file.open(QIODevice::ReadOnly)) {
      std::cerr << "Failed to open " << testDataPath.toStdString() << " for reading\n";
      return 1;
  }
  
  QByteArray expectedData = file.readAll();
  file.close();
  
  QJsonDocument expectedDoc = QJsonDocument::fromJson(expectedData);
  QJsonObject expectedObj = expectedDoc.object();
  
  if (jsonObj == expectedObj) {
      std::cout << "Test passed: Computed Panchanga matches expected output for " << today.toString("yyyy-MM-dd").toStdString() << ".\n";
      return 0;
  } else {
      std::cerr << "Test failed: Computed output differs from expected output!\n";
      QJsonDocument actualDoc(jsonObj);
      std::cerr << "Expected:\n" << expectedDoc.toJson(QJsonDocument::Indented).toStdString() << "\n";
      std::cerr << "Actual:\n" << actualDoc.toJson(QJsonDocument::Indented).toStdString() << "\n";
      return 1;
  }
}
