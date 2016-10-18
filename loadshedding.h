#ifndef LOADSHEDDING_H
#define LOADSHEDDING_H
#include <QString>

class Loadshedding
{
public:
  QString group;
  QString data;
  QString latest;
  Loadshedding();
  void retrieve();
  void saveUpdate();
  QString getData(QString groupd);

};

#endif // LOADSHEDDING_H
