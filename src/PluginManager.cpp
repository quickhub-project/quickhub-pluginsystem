/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * It is part of the QuickHub framework - www.quickhub.org
 * Copyright (C) 2021 by Friedemann Metzger - mail@friedemann-metzger.de */

#include "PluginManager.h"
#include <QPluginLoader>
#include <QDir>
#include <QCoreApplication>
#include <QProcessEnvironment>
#include "IPlugin.h"
#include <QDebug>


Q_GLOBAL_STATIC(PluginManager, pluginManager);

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{

}

PluginManager *PluginManager::getInstance()
{
    return pluginManager;
}

void PluginManager::loadPlugins(QVariantMap parameters)
{
    QDir pluginsDir;
    #if defined(Q_OS_WIN)
    qDebug()<<"(Windows)";
    pluginsDir = QDir(QCoreApplication::instance()->applicationDirPath());
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
    pluginsDir.cd("plugins");
    #elif defined(Q_OS_MAC)
    qDebug()<<"(OS X)";
    pluginsDir = QDir(QCoreApplication::instance()->applicationDirPath());
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cd("plugins");
    }
    #else
    pluginsDir = QDir(QCoreApplication::instance()->applicationDirPath()+"/plugins/");
    #endif

    qDebug()<<"";
    qDebug()<<"";
    qDebug()<< "Search plugins in "<<pluginsDir.absolutePath();
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        qDebug()<<"--> Plugin found:"<<pluginsDir.absoluteFilePath(fileName);
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        IPlugin* iplugin = qobject_cast<IPlugin*>(plugin);
        if (iplugin)
        {
            _plugins.insert(iplugin->getPluginName(), iplugin);
            qDebug()<<"--> Plugin loaded:"<< iplugin->getPluginName();
            qDebug()<<"";
        }
        else
        {
           qDebug()<<loader.errorString();
        }
    }

    QMapIterator<QString, IPlugin*> it(_plugins);
    while(it.hasNext())
    {
        it.next();
        IPlugin* plugin = it.value();
        init(plugin, parameters);
    }
}

void PluginManager::addObject(QObject *object)
{
    _objectPool.append(object);
    Q_EMIT objectAdded(object);
}

void PluginManager::removeObject(QObject *object)
{
    Q_EMIT objectAboutToRemove(object);
    _objectPool.removeAll(object);
}


QList<QObject *> &PluginManager::getObjects()
{
    return pluginManager->_objectPool;
}

bool PluginManager::init(IPlugin *plugin, QVariantMap parameters)
{
    if(plugin->property("initialized").toBool())
        return true;

    QSetIterator<QString> dependencies(plugin->requires());
    while(dependencies.hasNext())
    {
       QString dep = dependencies.next();
       if(!init(_plugins.value(dep), parameters))
           qWarning()<<"Loading dependency failed: "<< dep;
    }

    qInfo()<<"";
    qInfo()<<"";
    qInfo()<<"--------------------- Load "<<plugin->getPluginName()<<" ---------------------";
    qInfo()<<"";
    if(plugin->init(parameters))
    {
        plugin->setProperty("initialized", true);
        return true;
    }

    return false;
}

