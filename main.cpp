/****************************************************************************
 **
 ** Copyright (C) 2012 Mathieu Moriceau
 ** All rights reserved.
 ** Contact: Watcheur (watcheur@orange.fr)
 **
 ****************************************************************************/

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();

    return app.exec();
}
