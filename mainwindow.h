#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include "usbfiledialog.h"
#include <QTextEdit>


namespace Ui {
class MainWindow;
}




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:

    void handleFilemanagerFinished();

    void onReqCompleted();

    void on_actionAbout_triggered();

    void on_connButton_clicked();

    void systeminfo();

    void on_sideload_Button_clicked();

    void on_disButton_clicked();

    void on_actionQuit_triggered();

    void on_uninstall_Button_clicked();

    bool installAPK(QString filename);

    void on_actionHelp_triggered();

    void on_backupButton_clicked();

    void open_pref_database();


    void on_restoreButton_clicked();
    void dos_shell();

    void  cacheButton_other();
    void  cacheButton_android();



    void on_adbshellButton_clicked();

    void splashButton_android();

  void splashButton_other();


     void finishedCopy1();
    void finishedCopy2();


    void delay(int secs);

   void insertDevice();

   void backupAndroid();


   void busybox_permissions();

   void createTables();



    void editAndroid();



    void editOther();


   QString getadb();

  QString manufacturer();
  QString devicename();

   int getandroid();
   int getperms(QString dir);
   bool isScoped();



   void  finishedLocalBackup();



   void writeBackup (QString dir);

   QString readBackup (QString databasedir);

   QString checkslash (QString qpath);


   void writeInstall (QString dir);

   QString readInstall (QString databasedir);

   void TimerEvent();
   void delayTimer(int rdelay);


   QString RunLongProcess(QString cstring, QString jobname);

   void updateDeviceRecord();

   void usbbuttons(bool isusb);

   void rotate_logfile();

   void logfile2(QString line);

   void rebootDevice(QString reboot);

   bool mount_system(QString mnt);

   bool fileExists(QString path);

   bool is_su();

   bool is_busybox();


     QString strip (QString str);

    void default_device_values();

    void blank_entry_form();

    void kill_server();

   void start_server();

   bool check_Connection();


   bool is_package(QString package);


   int getRows();


    bool find_daddr(QString);

    void updateRoot();

    void androidLog();

    void updateDevice();

   void copyFolder(QString sourceFolder, QString destFolder);

     void kodiFolder(QString sourceFolder, QString destFolder);

    void getRecord(QString descrip);

    void on_actionReboot_triggered();

    void otherLog();

    void deleteRecord(QString descrip);

    bool isConnectedToNetwork();

    bool searchlistDevices(QString mstring);


    void addcolumn1();

    void on_actionRecovery_triggered();

    void device_clicked(QString item);

    void on_donate_clicked();

   void do_versioncheck();




    void on_actionDownload_Kodi_triggered();

    void on_editRecord_clicked();

    void dataentry(int flag);



  //   void move_kodi_data(QString externallocation, int choice);


    void loaddevicebox();




    void on_doConsole_clicked();

    void on_killServer_clicked();

    void on_refreshConnectedDevices_clicked();


    bool record_exists(QString descrip);

    QString getDescription(QString mdaddr);
    QString getDevice(QString mdaddr);
    QString getOSType(QString mdaddr);
    bool getflag1(QString mdaddr);
    QString getWSA(QString mdaddr);

   void on_newRecord_clicked();

  void androidRemote();

  void  otherRemote();

   void on_delRecord_clicked();


   void on_cacheButton_clicked();

 //  void on_splashButton_clicked();

   void on_mvdataButton_clicked();

   void on_actionView_Kodi_Log_triggered();

   void on_actionView_adbLink_Log_triggered();

   void on_screencapButton_clicked();



   void restoreAndroid();



   void backupOther();
   void restoreOther();

  // void on_actionEdit_XML_files_triggered();


   void on_actionPaste_path_triggered();

   void on_actionStop_Application_triggered();

   void on_actionStart_Application_triggered();

   void on_keypadButton_clicked();



   void on_fmButton_clicked();



   void on_actionMount_system_RO_triggered();

   void on_actionMount_system_RW_triggered();

   void on_actionWireless_ADBD_triggered();

   void on_actionKodi_data_usage_triggered();

  void on_deviceBox_currentIndexChanged(const QString &arg1);

  void on_listDevices_clicked(const QModelIndex &index);

 void on_listDevices_doubleClicked(const QModelIndex &index);

  void on_clearButton_clicked();

  void on_actionReboot_bootloader_triggered();

  void on_actionPreferences_triggered();

  void updateckversion(int value);

   void updateMsgboxtype(int value);
   void updatedownloaddir(QString value);

   QString getDaddr(QString descrip);

  bool getadbPref();

   int getmsgboxtype();

   bool check_devices();

  QString getdownloadpath();

  void on_actionDown_SPMC_triggered();

  void on_adhocip_returnPressed();

//  void on_actionGet_package_name_triggered();

  void on_actionReiinstall_Busybox_triggered();

  void on_actionArchitecture_triggered();

  void on_actionSleep_adjust_triggered();


  void on_actiondelthumb_triggered();

  void on_actionTest_adb_connection_triggered();

  void on_actionPush_remote_triggered();


  void on_actionAndroid_11_triggered();



  void on_actionCreate_kodi_data_triggered();



  void on_scpyButton_clicked();



  void on_pushButton_clicked();

  void on_pushTimers_clicked();



  void on_connWSA_clicked();



  void on_actionView_Changelog_triggered();



  void on_editXML_clicked();

  void on_actionSplash_Screen_triggered();

  void on_View_Changelog_triggered();

  void on_Erase_adbLink_database_triggered();

  void on_actionWSA_triggered();

  void on_actionConnect_WSA_triggered();


  void on_actionSet_Kodi_permissions_triggered();

  void on_actionGet_UID_from_APK_file_triggered();

  void on_actionOculus_VR_triggered();

  void on_actionSend_text_triggered();


  private:
    Ui::MainWindow *ui;

 usbfileDialog *fmdialog;

 QStringList RunProcessList;



};

extern int os;
extern bool isConnected;
extern bool serverRunning;
extern bool is_packageInstalled;


extern bool dbexists;
extern bool updatecheck;
extern bool versioncheck;
extern bool isusb;
extern bool disableroot;

extern QString port;
extern QString filename;
extern QString adbdir;
extern QString adb;
extern QString aapt;
extern QString fastboot;
extern QString xmldir;
extern QString splashdir;



extern QString hdir;
extern QString daddr;
extern QString sldir;
extern QString pushdir;
extern QString pulldir;
extern QString xbmcpackage;
extern QString ostype;
extern QString scope;
extern QString data_root;
extern QString buffersize;
extern QString bufferfactor;
extern QString dbstring;
extern QString description;
extern QString filepath;
extern QString busypath;
extern QString adblog;
extern QStringList bufferlist;


extern QString adbstr1;
extern QString adbstr2;
extern QString devstr1;
extern QString devstr2;
extern QString program;
extern QString tempdir;
extern QString logfiledir;
extern QString databasedir;
extern QString downloaddir;

extern int usbcheck;
extern int rootpath;
extern int ftvupdate;
extern int checkversion;
extern int extdata;
extern int buffermode;
extern int tsvalue;
extern int ct;
extern int Id;
extern int deviceboxindex;




#endif // MAINWINDOW_H
