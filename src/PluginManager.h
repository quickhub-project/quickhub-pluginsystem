/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * It is part of the QuickHub framework - www.quickhub.org
 * Copyright (C) 2021 by Friedemann Metzger - mail@friedemann-metzger.de */


#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include "QHPluginSystem_global.h"

class IPlugin;
class PLUGINSYSTEMSHARED_EXPORT PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = nullptr);
    static PluginManager* getInstance();
    void loadPlugins(QVariantMap parameters);

    /**
        \brief  Method for adding objects to the object pool.

                Plugins could provide functionality to other plugins by adding
                objects to the object pool.
    */
    void addObject(QObject *object);

    /**
        \brief  Method for removing objects from the object pool.

                Plugins could provide functionality to other plugins by adding
                objects to the object pool.
    */
    void removeObject(QObject *object);

    /**
        \brief  Returns all objects from the object pool
    */
    static QList<QObject*>& getObjects();

    /**
        \brief  Retuns all objects with the given type from the object pool.
    */
    template <typename T> QList<T *> static getObjects()
    {
        QList<QObject *> all = getObjects();
        QList<T *> result;
        foreach (QObject *obj, all)
        {
            T* t = qobject_cast<T*>(obj);
            if (t != 0)
            {
                result.append(t);
            }
        }
        return result;
    }

private:
    QList<QObject*> _objectPool;
    QMap<QString, IPlugin*> _plugins;
    bool init(IPlugin* plugin, QVariantMap parameters);


signals:
    void objectAdded(QObject *object);
    void objectAboutToRemove(QObject *object);

public slots:
};

#endif // PLUGINMANAGER_H
