#include "loadshedding.h"
#include "home.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/qnetworkrequest.h>

Loadshedding::Loadshedding()
{
  this->retrieve();
}

void Loadshedding::retrieve()
{
  QDir home=QDir::home();
  if(!home.cd(".loadshedding")){
      home.mkdir(".loadshedding");
      //update
      //refresh
    }
  else{
      QFile groups(home.filePath("group"));
      QFile datas(home.filePath("data"));
      QFile latest(home.filePath("latest"));
      if (!groups.open(QFile::ReadOnly | QFile::Text)) return;
      if (!datas.open(QFile::ReadOnly | QFile::Text)) return;
      if (!latest.open(QFile::ReadOnly | QFile::Text)) return;
      QTextStream s_group(&groups);
      QTextStream s_datas(&datas);
      QTextStream s_latest(&latest);
      this->group=s_group.readAll();
      this->data=s_datas.readAll();
      this->latest=s_latest.readAll();
    }
}

void Loadshedding::saveUpdate()
{
  QDir home=QDir::home();
  if(!home.cd(".loadshedding")){
      home.mkdir(".loadshedding");
      //update
      //refresh
    }else{
      QFile groups(home.filePath("group"));
      QFile datas(home.filePath("data"));
      QFile latest(home.filePath("latest"));
      if(!groups.open(QFile::WriteOnly|QFile::Text)) return;
      QTextStream outg(&groups);
      outg<<this->group;
      if(!datas.open(QFile::WriteOnly|QFile::Text)) return;
      QTextStream outd(&datas);
      outd<<this->data;
      if(!latest.open(QFile::WriteOnly|QFile::Text)) return;
      QTextStream outl(&latest);
      outl<<this->latest;

    }
}

QString Loadshedding::getData(QString groupd)
{
  QDir home=QDir::home();
  if(!home.cd(".loadshedding")){
      home.mkdir(".loadshedding");
      return this->data;
      //update
      //refresh
    }else{
      QFile groups(home.filePath(groupd));
      if(!groups.open(QFile::ReadOnly|QFile::Text)) return "NULL";
      QTextStream s_grps(&groups);
      return s_grps.readAll();
    }

}

