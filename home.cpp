#include "home.h"
#include "ui_home.h"
#include <QDir>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDate>
#include <QTime>
#include "loadshedding.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

Home::Home(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Home)
{
  ui->setupUi(this);
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  //QMessageBox::information(this,tr("updt"),QString::number(QString("12").split(":").at(0).toInt()),QMessageBox::Ok);
  this->updateUI();
}

void Home::updateUI()
{
  ui->refresh_at->setText("Last updated at "+lh.latest);
  QDate date=QDate::currentDate();
  int wkDay=date.dayOfWeek()+1;
  if(wkDay==8) wkDay=1;
  ui->tableWidget->selectRow(wkDay-1);

  QString dat=this->lh.data;
  ui->comboBox->setCurrentIndex((this->lh.group.toInt()-1));
  QStringList datas=dat.split("\n");
  for(int i=0;i<=6;i++){
      QString dataTemp;
      try{
        dataTemp=datas.at(i+3);
      }catch(...){
        return;
      }
      QStringList dataTemp_SL=dataTemp.split(" ");
      QString dataTemporary=dataTemp_SL.at(1);
      QStringList dataTemporary_SL=dataTemporary.split(",");
      ui->tableWidget->setItem(i,0,new QTableWidgetItem(dataTemporary_SL.at(0)));

      ui->tableWidget->setItem(i,1,new QTableWidgetItem(dataTemporary_SL.at(1)));
      QString timon=dataTemporary_SL.at(0);
      QString timet=dataTemporary_SL.at(1);
      if(wkDay-1==i){
          QTime dtm=QTime::currentTime();
          int hr=dtm.hour();
          QStringList timon_SL=timon.split("-");
          QStringList timet_SL=timet.split("-");
          int startM=QString(timon_SL.at(0)).split(":").at(0).toInt();
          int endM=QString(timon_SL.at(1)).split(":").at(0).toInt();
          int startE=QString(timet_SL.at(0)).split(":").at(0).toInt();
          int endE=QString(timet_SL.at(1)).split(":").at(0).toInt();
          if((hr>=startM)&&(hr<endM)) ui->tableWidget->setCurrentCell(wkDay-1,0);
          if((hr>=startE)&&(hr<endE)) ui->tableWidget->setCurrentCell(wkDay-1,1);

        }
    }

}

void Home::updateData(QByteArray arr)
{QString allS=arr;
  QStringList allS_SL=allS.split("\n");
  QString groupNoa=allS_SL.at(0);
  QStringList groupNo_SL=groupNoa.split("-");
  QString groupp=groupNo_SL.at(1);
  QString latests=allS_SL.at(1);

  QString datast=arr;
  QDir home=QDir::home();
  if(!home.cd(".loadshedding")){
      home.mkdir(".loadshedding");
      //update
      //refresh
    }
  QString datad=QString("dat")+groupp;
  QFile datas(home.filePath(datad));
  QFile latestf(home.filePath("latest"));
  if(!datas.open(QFile::WriteOnly|QFile::Text)) return;
  QTextStream outd(&datas);
  outd<<datast;
  if(!latestf.open(QFile::WriteOnly|QFile::Text)) return;
  QTextStream outl(&latestf);

  outl<<latests;

  if(this->lh.group.compare(groupp)){
      QFile dataa(home.filePath("data"));
      if(!dataa.open(QFile::WriteOnly|QFile::Text)) return;
      QTextStream outdd(&dataa);
      outdd<<datast;
      this->lh.retrieve();
    }

}

void Home::getData(QString groupd)
{
  QString url=QString("https://acpmasquerade-nepal-loadshedding-schedule-by-sparrow-sms.p.mashape.com/schedule.php?format=format=json&group=")+groupd;
  QUrl urls(url);
  QNetworkRequest request(urls);
  //            "X-Mashape-Key": "S5tT684lzzmsh7b9IggZVkjDllGsp1mWOaajsnh5mvzw7fk8uU",
  request.setRawHeader("X-Mashape-Key","S5tT684lzzmsh7b9IggZVkjDllGsp1mWOaajsnh5mvzw7fk8uU");
  request.setRawHeader("Accept","text/plain");
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  QNetworkReply* reply= manager->get(request);
  reply->setProperty("group",groupd);
  connect(manager, &QNetworkAccessManager::finished,
              this, &Home::replyFinished);
}



Home::~Home()
{
  delete ui;
}


void Home::on_comboBox_currentIndexChanged(const QString &arg1)
{

  QString st=arg1;
  QStringList s_SL=arg1.split(" ");
  QString datad=QString("dat")+s_SL.at(1);
  this->lh.group=s_SL.at(1);
  QString dat=this->lh.getData(datad);
  this->lh.data=dat;
  this->lh.saveUpdate();
  this->updateUI();

}

void Home::replyFinished(QNetworkReply *reply)
{

  if(reply->error()){
      if(reply->property("group")=="7"){
          QMessageBox::warning(this,tr("Loadshedding By Amrit"),tr("Sorry!!\nYour network is not helping us much.\n Try after stable connection."),QMessageBox::Ok);
        }return;
    }

  this->updateData(reply->readAll());
  if(reply->property("group")=="7"){
      QMessageBox::information(this,tr("Loadshedding By Amrit"),tr("Updated successfully.\nData up to date now."),QMessageBox::Ok);
    }

  return;
}

void Home::on_pushButton_clicked()
{
  for(int i=1;i<8;i++){
      QString str=QString::number(i);
      getData(str);
    }
}
