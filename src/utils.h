#ifndef UTILS_H
#define UTILS_H

#include "app.h"

class Utils
{
public:
    Utils() = default;

    static QString readDesktopFileContent(const QString &fileName);

    static QStringList readDefinition();
    static void writeDefinition(const QList<App *> &apps);
};

#endif // UTILS_H
