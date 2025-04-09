#include "usbfiledialog.h"
#include "pushfiledialog.h"
#include "ui_usbfiledialog.h"
#include "editordialog.h"
#include "dragdialog.h"
#include "logfile.h"
#include "getadbdata.h"

#include <QProcess>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
#include <QInputDialog>
#include <QTreeView>
#include <QFileDialog>
#include <QFileInfo>
#include <QDirModel>
#include <QDebug>
#include <QTimer>
#include <QStatusBar>
#include <QFileSystemModel>


#ifdef Q_OS_LINUX
 int ost1=0;
#elif defined(Q_OS_WIN)
  int ost1=1;
#elif defined(Q_OS_MAC)
int ost1=2;
#endif

bool hasfocus=false;
QString ufdlogfiledir="";
bool noroot;
QString quote1="\"'";
QString quote2="'\"";
bool do_oldfm=false;

///////////////////////////////////////////////////////
usbfileDialog::usbfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::usbfileDialog)
{

    ui->setupUi(this);




  if (ost1==1)
  {   ufdlogfiledir= QDir::homePath()+"/AppData/Roaming/.jocala/";
      tmpdir1= QDir::homePath()+"/AppData/Roaming/.jocala/scripts/";
  }

  else
  {
      ufdlogfiledir = QDir::homePath()+"/.jocala/";
      tmpdir1= QDir::homePath()+"/.jocala/scripts/";
  }

QString programName = QCoreApplication::applicationName();

  logfile(programName);

  ui->kodiDirs->clear();

  ui->kodiDirs->addItem("sdcard");
  ui->kodiDirs->addItem("storage");
  ui->kodiDirs->addItem("data/local/tmp");

  if (programName=="adblink"  ||   programName=="adbLink"   )
  {
    ui->kodiDirs->addItem("kodi");
    ui->kodiDirs->addItem("userdata");
    ui->kodiDirs->addItem("addons");
  }




   ui->usbprogressBar->setHidden(true);
   ui->usbprogressBar->setValue(0);

   connect(ui->usblistWidget1, SIGNAL(itemClicked(QListWidgetItem*)), SLOT( assignWindow1()   ));
   connect(ui->usblistWidget2, SIGNAL(itemClicked(QListWidgetItem*)), SLOT( assignWindow2()   ));



}

usbfileDialog::~usbfileDialog()
{
    delete ui;
}


/////////////////////////////////////

bool usbfileDialog::checkRoot() {

  if (noroot)
      return false;





    QString cstring =  adb21 + " shell /data/local/tmp/adblink/which su";
    QString command=getadbOutput(cstring);

    if (command.contains("su"))

    return true;

    else

    return false;

}


///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setMode(const int &modenum) {

    if (modenum==0)
    ui->usblistWidget1->setSelectionMode(QAbstractItemView::ExtendedSelection);
    else
    ui->usblistWidget1->setSelectionMode(QAbstractItemView::SingleSelection);
}


///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setData(const QString &labelText) {

 this->setWindowTitle(labelText);

}



///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setPulldir(const QString &pullstr) {
 pulldir_ufd=pullstr;

}



///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setuProgram(const QString &fprogram) {
 fmprogram=fprogram;

}




////////////////////////////////////////////////////////

void usbfileDialog::setAdbdir(const QString &adbstr) {
 adbdir_ufd=adbstr;

}




////////////////////////////////////////////////////////

void usbfileDialog::setoldfm(const bool &oldfm) {
 do_oldfm=oldfm;

}



////////////////////////////////////////////////////////

void usbfileDialog::setdisableroot(const bool &disableroot) {
 noroot=disableroot;

}




///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setADB(const QString &adbdata) {

    adb21=adbdata;

   // qDebug() << adb21;

}


///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setkodiPath(const QString &kodipath) {

    mcpath=kodipath;

}




void usbfileDialog::on_pushfilesButton_clicked()
{


    if (do_oldfm)
    oldPush();
    else
    newPush();
}


/////////////////////////////////////////////

void usbfileDialog::newPush()
{

QStringList filePaths;


bool doroot=false;
QString xpath;



    if (hasfocus)
         xpath=current_directory1;
    else
         xpath=current_directory2;


 dragDialog dialog;

 dialog.setWindowTitle(xpath);


if (dialog.exec() == QDialog::Accepted)
     filePaths   = dialog.getFilePaths();





    if (filePaths.count()<=0)
        return;


/*

    cstring =  adb21 + " shell ls "+xpath;
    QString command=getadbOutput(cstring);

    if (command.contains("Permission denied"))
             doroot=true;

     if (xpath.contains("/system/"))
         doroot=true;


     if(doroot)
         rootpush(filePaths);
       else
        userpush(filePaths);
*/

 if (checkRoot())
        rootpush(filePaths);
       else
        userpush(filePaths);


}


/////////////////////////////////////////////

void usbfileDialog::oldPush()
{


    QStringList mstringlist;
    bool doroot=false;
    QString xpath;

    pushfileDialog dialog;
    dialog.setModal(true);

    if(dialog.exec() == QDialog::Accepted)
        mstringlist = dialog.rstringlist();

  if (mstringlist.count()<=0)
      return;



    if (hasfocus)
     xpath=current_directory1;
    else
     xpath=current_directory2;

    cstring =  adb21 + " shell ls "+xpath;
    QString command=getadbOutput(cstring);

    if (command.contains("Permission denied"))
             doroot=true;

     if (xpath.contains("/system/"))
         doroot=true;


     if(doroot)
         rootpush(mstringlist);
       else
        userpush(mstringlist);



}


/////////////////////////////////////////

void usbfileDialog::on_pullButton_clicked()
{

   //    qDebug() << pulldir_ufd;

    if (!QDir(pulldir_ufd).exists())
     {   QMessageBox::critical(this,"","Pull destination does not exist");
         return;
     }


    // stat -c "%a" test.gz | grep 7 get permissions


   QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "","Pull file(s)?",
         QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::No)
       return;


   QStringList mstringlist;
   QString fileName;
   QString fname;
   QString command;
   QString cstring;
   bool dtest;
   QString dirname;
   int error=0;



if (hasfocus)
{
   if( ui->usblistWidget1->selectedItems().count() >= 1 )
     {
       foreach( QListWidgetItem *item, ui->usblistWidget1->selectedItems() )
       {
            if (item->text() == "..")
            return;
            else
           mstringlist << item->text();
       }

      }
}

else

{
    if( ui->usblistWidget2->selectedItems().count() >= 1 )
    {
       foreach( QListWidgetItem *item, ui->usblistWidget2->selectedItems() )
       {
            if (item->text() == "..")
            return;
            else
           mstringlist << item->text();
       }

       }
}




if (mstringlist.count() < 1)
   return;




logfile("Pull Files");
logfile("----------");

for (QStringList::iterator it = mstringlist.begin();
      it != mstringlist.end(); ++it)
{
    fileName = *it;


    int r = fileName.lastIndexOf("/");
    fname=fileName.mid(r+1,fileName.length());



    //cstring=adb21+rootShell+" stat -c %a "+fileName;
    //command=getadbOutput(cstring); // permissions

   cstring = adb21+rootShell+ " test -d "+ fileName +" && echo true || echo false";

   command=RunLongProcess_ufd(cstring);  // is directory


    if (command.contains("true"))
    {
        int r = fileName.lastIndexOf("/");
        dirname = fileName.mid(r,fileName.count());
        dirname=fix_directory(dirname);
        dtest=true;
    }

    else dtest=false;






    if (dtest)
         cstring = adb21 + " pull "+'"'+fileName+'"'+" "+'"'+pulldir_ufd+dirname+'"';
        else
        cstring = adb21 + " pull "+'"'+fileName+'"'+" "+'"'+pulldir_ufd+'"';

         command=RunLongProcess_ufd(cstring);


        //if (!command.contains("bytes"))

       if (!command.contains("pulled"))
        {

           // copy fileName to /data/local/tmp then re-attempt pull

           cstring=adb21+rootShell+" cp -R " +fileName+ " /data/local/tmp";
           command=RunLongProcess_ufd(cstring);

          if (!command.isEmpty())
          {
              error=error+1;
              logfile("Error: "+command);
              QMessageBox::critical(this,"","Error(s). See log");
              return;
          }


           fname="/data/local/tmp/"+fname;


           cstring=adb21+rootShell+" chmod -R 755 " +fname;
           command=getadbOutput(cstring);



           if (dtest)
                cstring = adb21 + " pull "+'"'+fname+'"'+" "+'"'+pulldir_ufd+dirname+'"';
               else
               cstring = adb21 + " pull "+'"'+fname+'"'+" "+'"'+pulldir_ufd+'"';

           QString cmd=RunLongProcess_ufd(cstring);

  //   if (!cmd.contains("bytes"))

           if (!cmd.contains("pulled"))
                 {
                    logfile("pull failed:"+ fileName);
                    logfile(cstring);
                    logfile(cmd);
                    error=error+1;

                 }

               cstring=adb21+rootShell+" rm -r " +fname;
               command=getadbOutput(cstring);
       }

    else logfile("pull succeeded:"+ fileName + " "+command);

 }

if (error > 0)
QMessageBox::critical(this,"","Error(s). See log");
else
QMessageBox::information(this,"","Pull(s) complete. See log.");


//qDebug() << ufdlogfiledir+"adblink.log";
//qDebug() << pulldir_ufd;


ui->usblistWidget1->clearSelection();
ui->usblistWidget2->clearSelection();

setPath1(current_directory1);
setPath2(current_directory2);

}









/////////////////////////////////////////

void usbfileDialog::on_delButton_clicked()
{


    QStringList mstringlist;
    QString fileName;
    QString command;
    QString cstring;
    QString workingdir;
    int error=0;



    if (ui->usblistWidget1->selectedItems().count() >= 1 && ui->usblistWidget2->selectedItems().count() >= 1 )
       {
     QMessageBox::critical(this,"","select from one pane only");
     return;

    }




    if (hasfocus)
    {
        if( ui->usblistWidget1->selectedItems().count() >= 1 )
        {
        foreach( QListWidgetItem *item, ui->usblistWidget1->selectedItems() )
        {
             if (item->text() == "..")
             return;
             else
            mstringlist << item->text();
        }

       current_directory1 = mstringlist.at(0);
       current_directory1= current_directory1.left( current_directory1.lastIndexOf("/"));

        if(! current_directory1.endsWith("/"))
            current_directory1.append("/") ;

          workingdir=current_directory1;

    }
}

else

    if( ui->usblistWidget2->selectedItems().count() >= 1 )
     {
          foreach( QListWidgetItem *item, ui->usblistWidget2->selectedItems() )
          {
               if (item->text() == "..")
               return;
               else
              mstringlist << item->text();
          }

        current_directory2 = mstringlist.at(0);
        current_directory2= current_directory2.left( current_directory2.lastIndexOf("/"));

         if(!current_directory2.endsWith("/"))
             current_directory2.append("/") ;

         workingdir=current_directory2;

    }


  QMessageBox::StandardButton reply3;
      reply3 = QMessageBox::question(this, "", "Delete files(s)?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply3 == QMessageBox::No)
          return;

logfile("Delete Files");
logfile("----------");



        for (QStringList::iterator it = mstringlist.begin();
            it != mstringlist.end(); ++it)
         {
            fileName = *it;

            if (checkRoot())
              {
                if (fileName.contains(" "))
                 {
                    QMessageBox::critical(this,"","Embedded spaces. See File Manager help");
                    return;
                }

            }


            cstring = adb21 + rootShell + " rm -r  " +quote1+fileName+quote2;

            command=RunLongProcess_ufd(cstring);



           if (command.contains("exist"))
              {
                 logfile(command);
                 logfile( "Deletion failed");
                 QMessageBox::critical(this,"",fileName+":deletion failed");
                 error=error+1;
               }

                 else
                  {
                    logfile(command);
                    logfile(fileName+":deletion succeeded");
                    }


}




   //     if (workingdir==current_directory1)
   //        setPath1(workingdir);
    //    if (workingdir==current_directory2)
   //        setPath2(workingdir);

      //  if (listfocus)
        //    setPath1(workingdir);
      //    else
       //     setPath2(workingdir);

        setPath1(current_directory1);
        setPath2(current_directory2);



        if (error > 0)
            QMessageBox::critical(this,"","Delete(s) failed. See log.");



}

/////////////////////////////////////////

void usbfileDialog::on_editButton_clicked()
{

    QStringList mstringlist;
    QString curdir;
    QString fileName;

    //QString xpath;

    if (hasfocus)
    {
      if( ui->usblistWidget1->selectedItems().count() == 1 )
       {
            foreach( QListWidgetItem *item, ui->usblistWidget1->selectedItems() )
            {
                 if (item->text() == "..")
                 return;
                 else
                mstringlist << item->text();
            }

           fileName=mstringlist.at(0);
           curdir=current_directory1;
       }

   }

    else
    {
           if( ui->usblistWidget2->selectedItems().count() == 1 )
            {
              foreach( QListWidgetItem *item, ui->usblistWidget2->selectedItems() )
              {
                   if (item->text() == "..")
                   return;
                   else
                  mstringlist << item->text();
              }


              fileName=mstringlist.at(0);
              curdir=current_directory2;


          }
    }



    if (checkRoot())
      {
        if (fileName.contains(" "))
         {
            QMessageBox::critical(this,"","Embedded spaces. See File Manager help");
            return;
        }

    }



    if (fileName.isEmpty()) {
        return;
    }


    editfile(fileName, curdir);
    setPath1(curdir);


}

////////////////////////////////////////////////

void usbfileDialog::on_resetButton_clicked()
{

setPath1("/sdcard/");
setPath2("/sdcard/");

ui->customdir->setText("");

}






//////////////////////////////////////////////


void usbfileDialog::on_goButton_clicked()
{


 //   QMessageBox::critical(this,"",fmprogram);
//    return;

 QString xpath;
 QString kpath;
 QString kpath1;
 QString kpath2;
 QString kpath3;
 QString dirpath = ui->customdir->text();
 QString command;
 QString cstring;





 if(!is_directory(mcpath))
  {
     kpath1="/sdcard/";
     kpath2="/sdcard/";
     kpath3="/sdcard/";


 }



 else

  {
      kpath1 = mcpath;
      kpath2 = mcpath+"/userdata";
      kpath3 = mcpath+"/addons";
  }

// qDebug() << mcpath;
// qDebug() << kpath1;
// qDebug() << kpath2;
// qDebug() << kpath3;




/*
 if (!isKodi)
 {

   kpath1="/sdcard/";
   kpath2="/sdcard/";
   kpath3="/sdcard/";

 }


else

 {
     kpath1 = mcpath;
     kpath2 = mcpath+"/userdata";
     kpath3 = mcpath+"/addons";
 }


*/


   if (!dirpath.isEmpty())
   xpath=dirpath;

   else

   {
       switch( ui->kodiDirs->currentIndex() ){


       case 0:
       xpath = "/sdcard/";
       break;

       case 1:
       xpath = "/storage/";
       break;

       case 2:
       xpath = "/data/local/tmp/";
       break;

       case 3:
       xpath = kpath1;
       break;

       case 4:
       xpath = kpath2;
       break;

       case 5:
       xpath = kpath3;
       break;

       default:
       xpath = "/sdcard/";
       break;
       }

   }


   if(!is_directory(xpath))
    {
     QMessageBox::critical(this,"",xpath+" :Directory not found.");
     return;
     }





    if (ui->usblistWidget2->hasFocus())
        setPath2(xpath);
      else
        setPath1(xpath);



}


///////////////////////////////////////////////

void usbfileDialog::on_mkdirButton_clicked()
{

       bool ok;
       QString xpath;
       QString newdir = QInputDialog::getText(this, "","New Directory", QLineEdit::Normal,"", &ok);

       if (hasfocus)
          xpath=current_directory1;
           else
          xpath=current_directory2;

      if (newdir.isEmpty())
          return;



      if (checkRoot())
        {
          if (newdir.contains(" "))
           {
              QMessageBox::critical(this,"","Embedded spaces. See File Manager help");
              return;
          }

      }

        newdir=xpath+newdir;
        QString cstring =  adb21 + rootShell +" mkdir -p "+ quote1+newdir+quote2;



       // QString cstring =  adb21 + rootShell +" mkdir -p "+xpath + newdir;

       QString command=getadbOutput(cstring);

       setPath1(current_directory1);
       setPath2(current_directory2);



}

///////////////////////////////////////////////

void usbfileDialog::on_renameButton_clicked()
{



    QStringList mstringlist;
    int selected_items=0;
    bool ok;
    QString xpath;
    QString oldname;



    if (hasfocus )
      { xpath=current_directory1;
        selected_items = ui->usblistWidget1->selectedItems().count();
        foreach( QListWidgetItem *item, ui->usblistWidget1->selectedItems() )
        {
             if (item->text() == "..")
             return;
             else
            mstringlist << item->text();
        }

    }
        else
      {
        xpath=current_directory2;
        selected_items = ui->usblistWidget2->selectedItems().count();
        foreach( QListWidgetItem *item, ui->usblistWidget2->selectedItems() )
        {
             if (item->text() == "..")
             return;
             else
            mstringlist << item->text();
        }

       }

        if (selected_items < 1 || selected_items > 1)
         {
           
             QMessageBox::critical(this,"","Must select 1 item only");
            return;
           }


        QString newname = QInputDialog::getText(this, "","New name", QLineEdit::Normal,"", &ok);

        if (newname.isEmpty())
           return;









        oldname = mstringlist.at(0);



         newname=xpath+newname;

         if (checkRoot())
           {
             if ((newname.contains(" ")) || (oldname.contains(" ")))
                {
                  QMessageBox::critical(this,"","Embedded spaces. See File Manager help");
                  return;
                  }
            }


          // QString cstring =  adb21 + rootShell + "mv "+  +'"'+oldname+'"' + " " +'"'+newname+'"';

          QString cstring = adb21 + rootShell + " mv "+quote1+oldname+quote2 + " "+quote1+newname+quote2;



          QString command=getadbOutput(cstring);
          logfile(cstring);
          logfile(command);


          setPath1(current_directory1);
          setPath2(current_directory2);


      }



void usbfileDialog::on_customdir_returnPressed()
{
    on_goButton_clicked();
}


////////////////////////////////////////////////

void usbfileDialog::on_clearButton_clicked()
{
  ui->customdir->setText("");
}



/////////////////////////////////////////////////////

QString usbfileDialog::fix_directory(QString dirname)
{

if(!dirname.startsWith("/"))
   dirname.prepend("/");

if(!dirname.endsWith("/"))
   dirname.append("/") ;

 return dirname;

}


/////////////////////////////////////////////

void usbfileDialog::rootpush(QStringList filenames)
{


    QString command;
    QString cstring;
    QString xpath;  
    QString tmpdir=" /data/local/tmp/";


  logfile("rootpush started");


  if (checkRoot())
      rootShell = " shell su -c ";
  else
      rootShell = " shell ";

 // qDebug() << rootShell;
//  return;

    if(filenames.count() < 1)
    return;

  bool error = false;

  if (hasfocus)
      xpath=current_directory1;
    else
      xpath=current_directory2;

QString destdir;

  for (int i =0;i<filenames.count();i++)
                {


                    QFileInfo info1(filenames.at(i));


                   QString fname=info1.fileName();
                   QString absolutefname=info1.absoluteFilePath();



                       QString tmpstr=tmpdir+fname;
                       tmpstr=tmpstr.trimmed();

                        tmpstr='"'+tmpstr+'"';


                        if(info1.isDir())
                           {
                             absolutefname=absolutefname+"/";
                             cstring =  adb21 + " shell mkdir -p "+ tmpstr;
                             logfile(cstring);
                             command=getadbOutput(cstring);
                            }


                       absolutefname='"'+absolutefname+'"';

                       cstring =  adb21 + " push "+absolutefname+" "+tmpstr;

                       command=RunLongProcess_ufd(cstring);
                       logfile(cstring);


                       logfile(command);

                       destdir = xpath+fname;
                       destdir='"'+destdir+'"';

                       destdir.replace(" ", "\\ ");
                       destdir.replace("'", "\\'");

                       tmpstr.replace(" ", "\\ ");
                       tmpstr.replace("'", "\\'");



                      cstring =  adb21 + rootShell+ " cp -R "+tmpstr+" "+xpath;
                      logfile(cstring);
                      command=RunLongProcess_ufd(cstring);
                      logfile(command);


                    if (command.isEmpty())
                        {
                          logfile(info1.fileName()+" pushed");


                          cstring =  adb21 + " shell rm -r "+tmpstr;
                          logfile(cstring);
                          command=RunLongProcess_ufd(cstring);
                            if (!command.isEmpty())
                               {
                                 logfile("Error removing "+tmpstr);
                                 logfile(command);
                               }

                          }
                    else

                        { logfile(info1.fileName()+" not pushed");
                          logfile(command);
                          error = true;
                       }

  }




    setPath1(current_directory1);
    setPath2(current_directory2);


   if (error)
     QMessageBox::critical(this,"","File(s) not pushed. See log.");


}



/////////////////////////////////////////////

void usbfileDialog::userpush(QStringList filenames)
{


    QString command;
    QString cstring;
    QString xpath;
    QString tmpdir=" /data/local/tmp/";



   logfile("userpush started");


    if(filenames.count() < 1)
    return;

  bool error = false;

  if (hasfocus)
      xpath=current_directory1;
    else
      xpath=current_directory2;

QString destdir;

  for (int i =0;i<filenames.count();i++)
                {


                  QFileInfo info1(filenames.at(i));


                  QString fname=info1.fileName();
                  QString absolutefname=info1.absoluteFilePath();



                       QString tmpstr=xpath+fname;
                       tmpstr=tmpstr.trimmed();

                        tmpstr='"'+tmpstr+'"';

/*
                        if(info1.isDir())
                           {
                             // absolutefname=absolutefname+"/";
                             absolutefname=absolutefname;
                             cstring =  adb21 + " shell mkdir -p "+ tmpstr;
                             logfile(cstring);
                             command=getadbOutput(cstring);
                            }

*/

                       absolutefname='"'+absolutefname+'"';

                       cstring =  adb21 + " push "+absolutefname+"  "+tmpstr;


                       command=RunLongProcess_ufd(cstring);
                       logfile(cstring);


                       logfile(command);

                       if (command.contains("bytes") )
                               error=false;
                        else
                              error=true;



  }


    setPath1(current_directory1);
    setPath2(current_directory2);


   if (error)
     QMessageBox::critical(this,"","File(s) not pushed. See log.");


}


////////////////////////////////////////////////////////

QString usbfileDialog::RunLongProcess_ufd(QString cstring)
{


    ui->usbprogressBar->setHidden(false);
    ui->usbprogressBar->setValue(0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(usbTimerEvent()));
    timer->start(2000);


  // fmactivityIcon(true);

/*
 QProcess run_command;
 run_command.setProcessChannelMode(QProcess::MergedChannels);
 run_command.start(cstring);

 run_command.waitForStarted();

 while(run_command.state() != QProcess::NotRunning)
     qApp->processEvents();

 QString command=run_command.readAll();
*/

QString command=getadbOutput(cstring);

ui->usbprogressBar->setHidden(true);
ui->usbprogressBar->setValue(0);
 return command;
}





////////////////////////////////////////////////

void usbfileDialog::on_usblistWidget2_doubleClicked(const QModelIndex &index)
{
    currentitem2 = index.data(Qt::DisplayRole).toString();

    if (currentitem2.contains(".."))
    {
         previous_directory2=previous_directory2.left(previous_directory2.lastIndexOf("/"));
         previous_directory2=previous_directory2.left(previous_directory2.lastIndexOf("/"));
         previous_directory2=fix_directory(previous_directory2);
         setPath2(previous_directory2);
         //qDebug() << previous_directory2;

         return;


    }

    // QString cstring = adb21+ " shell if test -d "+'"'+currentitem2+'"'+ "; then echo 'true'; fi";


    if (checkRoot())
       currentitem2.replace(" ", "\\ ");

    QString cstring = adb21+rootShell + " [ -d " + quote1+currentitem2+ quote2 +" ] && echo 'true'";




    QString command=getadbOutput(cstring);

    if (command.contains("true"))
    {
        currentitem2=fix_directory(currentitem2);
        setPath2(currentitem2);
    }
  else

    {



        on_pullButton_clicked();

    }

}


///////////////////////////////////////////////

void usbfileDialog::on_usblistWidget1_doubleClicked(const QModelIndex &index)
{
    currentitem1 = index.data(Qt::DisplayRole).toString();

    if (currentitem1.contains(".."))
    {
         previous_directory1=previous_directory1.left(previous_directory1.lastIndexOf("/"));
         previous_directory1=previous_directory1.left(previous_directory1.lastIndexOf("/"));
         previous_directory1=fix_directory(previous_directory1);
         setPath1(previous_directory1);
         //qDebug() << previous_directory2;

         return;


    }

   // QString cstring = adb21+ rootShell + " if test -d "+'"'+currentitem1+'"'+ "; then echo 'true'; fi";


   //  QString cstring = adb21+ " shell if test -d "+'"'+currentitem1+'"'+ "; then echo 'true'; fi";





    if (checkRoot())
       currentitem1.replace(" ", "\\ ");

    QString cstring = adb21+rootShell + " [ -d " + quote1+currentitem1+ quote2 +" ] && echo 'true'";


    QString command=getadbOutput(cstring);

  // qDebug() << cstring;
  //  qDebug() << command;

    if (command.contains("true"))
    {
        currentitem1=fix_directory(currentitem1);


        setPath1(currentitem1);

    }
  else

    {



      on_pullButton_clicked();


    }

}



/////////////////////

bool usbfileDialog::is_directory(QString fdirectory)

{

QString string1=adb21 + rootShell +"/data/local/tmp/adblink/busybox find "+'"'+fdirectory+'"'+" -type d -maxdepth 0";
QString command=getadbOutput(string1);


// qDebug() << command;

if (command.contains("No such file or directory"))
    return false;
else
    return true;


}





///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setPath1(QString currentdir) {



   if (checkRoot())
       rootShell = " shell su -c ";
   else
       rootShell = " shell ";


    current_directory1=currentdir;
    previous_directory1=currentdir.left(currentdir.lastIndexOf("/"));

    if (previous_directory1.isEmpty())
        previous_directory1=current_directory1;

     current_directory1=fix_directory(current_directory1);
     previous_directory1=fix_directory(previous_directory1);

     currentdir.replace(" ", "\\ ");
     currentdir.replace("'", "\\'");


     QString string1=adb21+ rootShell +"/data/local/tmp/adblink/busybox find "+'"'+currentdir+'"'+" -type d -maxdepth 1";
     QString command=getadbOutput(string1);
     QStringList stringlist1=command.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    stringlist1.sort();

    QString string2=adb21+ rootShell +"/data/local/tmp/adblink/busybox find "+'"'+currentdir+'"'+" -type l -maxdepth 1";
    command=getadbOutput(string2);
    QStringList stringlist2=command.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    stringlist2.sort();

    QString string3=adb21+ rootShell +"/data/local/tmp/adblink/busybox find "+'"'+currentdir+'"'+" -type f -maxdepth 1";
    command=getadbOutput(string3);
    QStringList stringlist3=command.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    stringlist3.sort();

    QStringList stringlist4 = stringlist1 + stringlist2 + stringlist3;

   stringlist4.removeFirst();

   ui->usblistWidget1->clear();


for (QStringList::const_iterator it = stringlist4.begin();
        it != stringlist4.end(); ++it)
       {
        currentitem1 = *it;
        // qDebug()  << current ;

        if(  (!currentitem1.contains("Permission"))
            && (!currentitem1.contains("denied"))
            && (!currentitem1.contains("emulated"))
            && (!currentitem1.contains(":"))
            && (!currentitem1.isEmpty() ) )
            {
             ui->usblistWidget1->addItem(currentitem1);
            }
        }

QListWidgetItem *newItem = new QListWidgetItem;
newItem->setText("..");
ui->usblistWidget1->insertItem(0, newItem);


}


///////////////////////////////////////////////////////////////////////////

void usbfileDialog::setPath2(QString currentdir) {



      if (checkRoot())
          rootShell = " shell su -c ";
      else
          rootShell = " shell ";



    current_directory2=currentdir;
    previous_directory2=currentdir.left(currentdir.lastIndexOf("/"));

    if (previous_directory2.isEmpty())
        previous_directory2=current_directory2;

     current_directory2=fix_directory(current_directory2);
     previous_directory2=fix_directory(previous_directory2);

     currentdir.replace(" ", "\\ ");
     currentdir.replace("'", "\\'");

    QString string1=adb21+ rootShell +"/data/local/tmp/adblink/busybox find "+'"'+currentdir+'"'+" -type d -maxdepth 1";
    QString command=getadbOutput(string1);
    QStringList stringlist1=command.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    stringlist1.sort();


    QString string2=adb21+ rootShell +"/data/local/tmp/adblink/busybox find "+'"'+currentdir+'"'+" -type l -maxdepth 1";
    command=getadbOutput(string2);
    QStringList stringlist2=command.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);


    stringlist2.sort();

    QString string3=adb21+ rootShell  +"/data/local/tmp/adblink/busybox find "+'"'+currentdir+'"'+" -type f -maxdepth 1";
    command=getadbOutput(string3);
    QStringList stringlist3=command.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);


    stringlist3.sort();

    QStringList stringlist4 = stringlist1 + stringlist2 + stringlist3;

   stringlist4.removeFirst();

   ui->usblistWidget2->clear();


for (QStringList::const_iterator it = stringlist4.begin();
        it != stringlist4.end(); ++it)
       {
        currentitem2 = *it;
        // qDebug()  << current ;

        if(  (!currentitem2.contains("Permission"))
            && (!currentitem2.contains("denied"))
            && (!currentitem2.contains("emulated"))
            && (!currentitem2.contains(":"))
            && (!currentitem2.isEmpty() ) )
            {
             ui->usblistWidget2->addItem(currentitem2);
            }




}

QListWidgetItem *newItem = new QListWidgetItem;
newItem->setText("..");
ui->usblistWidget2->insertItem(0, newItem);


}



/*

////////////////////////////////////////////////////////
void usbfileDialog::editfile(QString fileName, QString xpath)
{


    QString tempfile1;
    QString tempfile2;
    QString rootfile;
    QString backfile;
    QString cstring;
    QString command;
    bool doroot=false;

QString tmpdir=" /data/local/tmp/";


   cstring = adb21+rootShell+" if test -d "+fileName+ "; then echo 'true'; fi";

   command=getadbOutput(cstring);

    if (command.contains("true"))
    {
        
          QMessageBox::critical(this,"","Can't edit directory "+fileName);
         return;
    }

     QString filename(fileName.mid(fileName.lastIndexOf("/")+1,fileName.length()));




             cstring = adb21 + " pull "+'"'+fileName+'"'+" "+'"'+tmpdir1+'"'+"/"+filename;



              command=getadbOutput(cstring);




             if (!command.contains("bytes"))
              {
                 logfile("edit failed");
                 logfile(command);
                 QMessageBox::critical(
                             this,
                            "",
                             "Edit failed "+command);
                 return;
             }



             QFile file1(tmpdir1+filename);

             if(!file1.open(QIODevice::ReadOnly | QIODevice::Text))
                 return;

             QString xmlfile = file1.readAll();
             editorDialog dialog;
             dialog.seteditor(xmlfile);
             dialog.setfilename(filename);

             rootfile=filename;
             backfile=filename+".bak";

             tempfile1 = tmpdir1+filename;
             tempfile2 = tmpdir1+filename+".bak";




              dialog.setModal(true);

              if(dialog.exec() == QDialog::Accepted)
                {


                  QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Save","Save "+ fileName+"?",
                        QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::No)
                      return;




 if (checkRoot())
    doroot=true;

                    xmlfile = dialog.xmlfile();


                   QFile::copy(tempfile1, tempfile2);
                   QFile caFile( tempfile1);
                   caFile.open(QIODevice::WriteOnly | QIODevice::Text);
                   QTextStream outStream(&caFile);
                   outStream << xmlfile;
                   caFile.close();

                   if(!doroot)
                   {
                       cstring = adb21 + " push "+'"'+tempfile1+'"'+ " "+xpath;
                       logfile(cstring);
                       command=getadbOutput(cstring);
                       logfile(command);

                       cstring = adb21 + " push "+'"'+tempfile2+'"'+ " "+xpath;
                       logfile(cstring);
                       command=getadbOutput(cstring);
                       logfile(command);

                   }

                   else

                   {
                        cstring =  adb21 + " push "+tempfile1+" "+tmpdir;
                       logfile(cstring);
                        command=getadbOutput(cstring);
                        logfile(command);

                        cstring =  adb21 + " push "+tempfile2+" "+tmpdir;
                        logfile(cstring);
                        command=getadbOutput(cstring);
                        logfile(command);

                    //     cstring =  adb21 + rootShell+ " cp -r "+tmpdir+backfile+" "+xpath;


                        file1.close();

                        QFile file2 (tempfile1 );
                        file2.remove();

                        QFile file3 (tempfile2);
                        file3.remove();


                       //


                        // qDebug() << cstring << "\n" << command; return;



                        if (command.contains("bytes"))
                         {
                             cstring =  adb21 + rootShell+ " chmod 0644 "+tmpdir+backfile;
                             logfile(cstring);
                             command=getadbOutput(cstring);
                           //  qDebug() << cstring << "\n" << command;
                            cstring =  adb21 + rootShell+ " cp -r "+tmpdir+backfile+" "+xpath;
                            logfile(cstring);
                            command=getadbOutput(cstring);
                          qDebug() << cstring << "\n" << command;
                            logfile(command);

                            if (command.isEmpty())
                            {
                            cstring =  adb21 + rootShell+ " rm -r "+tmpdir+backfile;
                           logfile(cstring);
                            command=getadbOutput(cstring);
                            logfile(command);
                            }

                        }

                   }

  }




                else
                {
                    cstring =  adb21 + rootShell + " push "+tempfile2+" "+tmpdir;
                    command=getadbOutput(cstring);
                    logfile(command);

                    if (command.contains("bytes"))
                     {
                        cstring =  adb21 + rootShell+ " chmod 0644 "+tmpdir+rootfile;
                        logfile(cstring);
                        command=getadbOutput(cstring);
                        cstring =  adb21 + rootShell+ " cp -r "+tmpdir+rootfile+" "+xpath;
                        logfile(cstring);
                        command=getadbOutput(cstring);
                        logfile(command);

                        if (command.isEmpty())
                        {
                        cstring =  adb21 + rootShell+ " rm -r "+tmpdir+rootfile;
                        logfile(cstring);
                        command=getadbOutput(cstring);
                        logfile(command);
                        }


                    }




  }







 }


*/

///////////////////////////////////

void usbfileDialog::editfile(QString fileName, QString xpath)
{
QString tempfile1;
QString tempfile2;
QString rootfile;
QString backfile;
QString cstring;
QString command;
bool doroot = false;

QString tmpdir = " /data/local/tmp/";

cstring = adb21 + rootShell + " if test -d " + fileName + "; then echo 'true'; fi";
command = getadbOutput(cstring);

if (command.contains("true"))
{
            QMessageBox::critical(this, "", "Can't edit directory " + fileName);
            return;
}

QString filename(fileName.mid(fileName.lastIndexOf("/") + 1, fileName.length()));

cstring =  adb21 + rootShell+ " cp "+fileName+" "+ tmpdir;
command = getadbOutput(cstring);

cstring=adb21+rootShell+" chmod 777 " +tmpdir+filename;
command=getadbOutput(cstring);

cstring = adb21 + " pull " +tmpdir+filename + " "+ tmpdir1 + "/" + filename;
command = getadbOutput(cstring);



if (!command.contains("bytes"))
{
            logfile("edit failed");
            logfile(command);
            QMessageBox::critical(this, "", "Exit failed " + command);
            return;
}

QFile file1(tmpdir1 + filename);

if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

QString xmlfile = file1.readAll();
editorDialog dialog;
dialog.seteditor(xmlfile);
dialog.setfilename(filename);

rootfile = filename;
backfile = filename + ".bak";

tempfile1 = tmpdir1 + filename;
tempfile2 = tmpdir1 + filename + ".bak";

dialog.setModal(true);

if (dialog.exec() == QDialog::Accepted)
{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Save", "Save " + fileName + "?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::No)
             return;

            if (checkRoot())
            {  doroot = true;
               rootShell = " shell su -c ";}
            else {
              doroot = false;
              rootShell = " shell ";
            }




            xmlfile = dialog.xmlfile();

            QFile::copy(tempfile1, tempfile2);
            QFile caFile(tempfile1);
            caFile.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream outStream(&caFile);
            outStream << xmlfile;
            caFile.close();

            if (!doroot)
            {
             cstring = adb21 + " push " + tempfile1 +" " + xpath;
             command = getadbOutput(cstring);

             cstring = adb21 + " push " + tempfile2 + " " + xpath;
             command = getadbOutput(cstring);

            }

            else

            {


             cstring = adb21 + " push " + tempfile1 + " " + tmpdir;         
             command = getadbOutput(cstring);

             cstring = adb21 + " push " + tempfile2 + " " + tmpdir;          
             command = getadbOutput(cstring);


             cstring =  adb21 + rootShell+ " cp "+tmpdir+rootfile+" "+xpath;
             command=getadbOutput(cstring);


             cstring =  adb21 + rootShell+ " cp "+tmpdir+backfile+" "+xpath;
             command=getadbOutput(cstring);


             cstring =  adb21 + rootShell+ " rm "+tmpdir+rootfile;
             command=getadbOutput(cstring);

             cstring =  adb21 + rootShell+ " rm "+tmpdir+backfile;
             command=getadbOutput(cstring);




            }

            file1.close();

            QFile file2(tempfile1);
            file2.remove();

            QFile file3(tempfile2);
            file3.remove();
 }
}


///////////////////////////////////////////////////////////
void usbfileDialog::assignWindow1()

{
     //QMessageBox::critical(this,"","panel1");
     hasfocus=true;
      //this->setWindowTitle("Panel 1");
}


///////////////////////////////////////////////////////////
void usbfileDialog::assignWindow2()

{
     //QMessageBox::critical(this,"","panel2");
     hasfocus=false;
    // this->setWindowTitle("Panel 2");
}


void usbfileDialog::usbTimerEvent()
{
  int value = ui->usbprogressBar->value();

  if (value >= 100)
      {
         value = 0;
         ui->usbprogressBar->reset();
     }

  ui->usbprogressBar->setValue(value+1);




}


///////////////////////////////////////////////

void usbfileDialog::on_copyButton_clicked()

  {

    do_copy(0);

  }


  /////////////////////////////////////////

  void usbfileDialog::on_moveButton_clicked()
  {

       do_copy(1);
  }





  ///////////////////////////////////////////////

  void usbfileDialog::do_copy(int opcode)
  {

      QStringList mstringlist;
      QString fileName;
      QString command;
      QString cstring;
      QString workingdir;
      QString optext;
      int error=0;


    if(opcode==0)
        optext="Copy";
    else
        optext="Move";


  if (hasfocus)
  {

      if( ui->usblistWidget1->selectedItems().count() >= 1 )
       {


          foreach( QListWidgetItem *item, ui->usblistWidget1->selectedItems() )
             {
                  if (item->text() == "..")
                  return;
                  else
                 mstringlist << item->text();
             }




          current_directory1 = mstringlist.at(0);
         current_directory1= current_directory1.left( current_directory1.lastIndexOf("/"));

          if(! current_directory1.endsWith("/"))
              current_directory1.append("/") ;

          workingdir=current_directory2;

      }
  }
  else

      if( ui->usblistWidget2->selectedItems().count() >= 1 )
       {
          foreach( QListWidgetItem *item, ui->usblistWidget2->selectedItems() )
             {

              if (item->text() == "..")
                  return;
              else
             mstringlist << item->text();
         }

          current_directory2 = mstringlist.at(0);
          current_directory2= current_directory2.left( current_directory2.lastIndexOf("/"));

           if(!current_directory2.endsWith("/"))
               current_directory2.append("/") ;

           workingdir=current_directory1;
      }




  if (mstringlist.count() < 1)
      return;


  if (checkRoot())
    {
      if ((fileName.contains(" ")) || (workingdir.contains(" ")))
       {
          QMessageBox::critical(this,"","Embedded spaces. See File Manager help");
          return;
      }

  }



    QMessageBox::StandardButton reply3;
        reply3 = QMessageBox::question(this, "", optext+" item(s)?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply3 == QMessageBox::No)
            return;




  logfile(optext + " files");
  logfile("----------");


          for (QStringList::iterator it = mstringlist.begin();
              it != mstringlist.end(); ++it)
           {
              fileName = *it;




              if (opcode==0)
              cstring = adb21 + rootShell + " cp -R "+quote1+fileName+quote2 + " "+quote1+workingdir+quote2;
              else
              cstring = adb21 + rootShell + " mv "+quote1+fileName+quote2 + " "+quote1+workingdir+quote2;

              logfile(cstring);


       command=RunLongProcess_ufd(cstring);

             if (!command.isEmpty())
                {

                 logfile(command);
                   logfile( optext+" failed");
                   QMessageBox::critical(this,"",fileName+" "+optext+ " failed");
                   error=error+1;
                 }

                   else
                    {
                      logfile(command);
                      logfile(fileName+" "+optext+" succeeded");
                      }


  }



    if (error>0)
      QMessageBox::critical(this,"",optext+ "(s) failed. See log.");


    if (checkRoot())
      {
        current_directory1.replace(" ", "\\ ");
        current_directory2.replace(" ", "\\ ");
     }

    setPath1(current_directory1);
    setPath2(current_directory2);


  }


/*



    if (hasfocus)
     xpath=current_directory1;
    else
     xpath=current_directory2;

    cstring =  adb21 + " shell ls "+xpath;
    QString command=getadbOutput(cstring);

    if (command.contains("Permission denied"))
             doroot=true;

     if (xpath.contains("/system/"))
         doroot=true;


     if(doroot)
         rootpush(filenames);
       else
        userpush(filenames);


*/

void usbfileDialog::gather_push()
{


    //  if (!check_devices() )
   //       return;

    //   bool installer=false;

      // QStringList filenames = QFileDialog::getOpenFileNames(this,tr("APK files"),QDir::currentPath(),tr("APK files (*.apk);;All files (*.*)") );



       bool doroot=false;
       QString xpath;


       if (hasfocus)
        xpath=current_directory1;
       else
        xpath=current_directory2;

       cstring =  adb21 + " shell ls "+xpath;
       QString command=getadbOutput(cstring);

       if (command.contains("Permission denied"))
                doroot=true;

        if (xpath.contains("/system/"))
            doroot=true;



       QFileDialog fileDialog;
       fileDialog.setOption(QFileDialog::DontUseNativeDialog);

        QStringList filenames = fileDialog.getOpenFileNames();

       // QStringList filenames = QFileDialog::getOpenFileNames();


      //  QStringList filenames =  QFileDialog::DontUseNativeDialog();

      if( !filenames.isEmpty() )
      {


        // qDebug() << filenames;
         return;

          QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Push", "Push files?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {


                if(doroot)
                    rootpush(filenames);
                  else
                   userpush(filenames);

            }



  //          if (installer)
  //          {

   //             QMessageBox::information(this,"","Files pushed.\nSee log for details.");


    //        }


      }



}

/*

#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFileDialog w;
    w.setFileMode(QFileDialog::DirectoryOnly);
    w.setOption(QFileDialog::DontUseNativeDialog,true);
    QListView *l = w.findChild<QListView*>("listView");
    if (l) {
         l->setSelectionMode(QAbstractItemView::MultiSelection);
     }
    QTreeView *t = w.findChild<QTreeView*>();
     if (t) {
       t->setSelectionMode(QAbstractItemView::MultiSelection);
   }
   return w.exec();
}

*/

