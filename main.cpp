
#include "replicator.h"
#include "sqlitestorage.h"

#include <QApplication>
#include <QUrl>

using namespace Couchette;

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    //Monitor m;
    //
    //m.start(QUrl("http://localhost:5984/test/_changes?feed=continuous"));
    //
    
    SqliteStorage storage;

    Replicator r(&storage);
    r.replicate(QUrl("http://localhost:5984/test"));

    a.exec();
}
