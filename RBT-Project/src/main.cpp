#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    // Initialize standard Qt application
    QApplication app(argc, argv);
    
    // Create and display the main GUI window
    MainWindow window;
    window.show();
    
    // Run the main event loop
    return app.exec();
}
