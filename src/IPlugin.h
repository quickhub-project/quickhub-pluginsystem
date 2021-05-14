/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * It is part of the QuickHub framework - www.quickhub.org
 * Copyright (C) 2021 by Friedemann Metzger - mail@friedemann-metzger.de */


#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QtPlugin>
#include "QHPluginSystem_global.h"
#include <QObject>
#include <QVariantMap>
#include <QSet>

/*!
    \class IPlugin
    \brief This class is the abstract base class which needs to be implemented to build and load own QuickHub plugins.
    \note  Do not forget to provide the derived class with the macro Q_INTERFACES(IPlugin)
*/

class PLUGINSYSTEMSHARED_EXPORT IPlugin : public QObject
{
    Q_OBJECT

public:
    IPlugin(QObject* parent = 0) : QObject(parent){}
    virtual ~IPlugin(){}

    /*!
        \fn bool IPlugin::init(QVariantMap parameters = QVariantMap()) = 0
        This is the central initialization function. In this function all resources
        and services must be instantiated and registered. The return value indicates
        whether the initialization of the plugin was successful.

        The parameters in the function argument are the command line parameters which
        are passed to the program at startup. If you call ./server -foo 32
        you will get the value 32 by parameters["foo"].
    */
    virtual bool init(QVariantMap parameters = QVariantMap()) = 0;

    /*!
        \fn bool IPlugin::shutdown() = 0
        This function is called to deinitialize plugins at runtime. Deregister your resources and services
        and free the allocated memory. Return true when deinitialization was successfull.
        \note This feature is currently not yet supported, but this functionality has been provided
        for the sake of completeness.
    */
    virtual bool shutdown() = 0;

    /*!
        \fn QSet<QString> IPlugin::requires()
        If there are dependencies to other Plugins which play a role in the initialization order of
        the Plugins, these dependencies can be indicated (Just add the plugin names to a QSet<QString> anf return it).
        The plugin loader will unravel the dependencies and will adapt the initialization sequence accordingly.
    */
    virtual QSet<QString> requires(){return QSet<QString>();};

    /*!
        \fn IPlugin::getPluginName()
        Return a unique and meaningful name.
    */
    virtual QString getPluginName() = 0;
};

#define IPlugin_iid "org.QuickHub.Server.Plugins.IPlugin"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

#endif // IPLUGIN_H
