#include <QCoreApplication>
#include <QDate>
#include <iostream>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  QDate d(-57, 3, 16);
  std::cout << d.toString("dddd, MMMM d, yyyy").toStdString() << "\n";
  return 0;
}
