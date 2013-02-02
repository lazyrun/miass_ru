#include "MainWindow.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   MainWindow mainWindow;
   mainWindow.show();
   
   a.setQuitOnLastWindowClosed(true);
   return a.exec();
}

