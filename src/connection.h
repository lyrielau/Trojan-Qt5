/*
 * Copyright (C) 2015-2016 Symeon Huang <hzwhuang@gmail.com>
 *
 * shadowsocks-qt5 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * shadowsocks-qt5 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libQtShadowsocks; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QHostInfo>
#include <QHostAddress>
#include <memory>
#include "pacserver.h"
#include "privoxythread.h"
#include "servicethread.h"
#include "tun2socksthread.h"
#include "systemproxyhelper.h"
#include "tqprofile.h"

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection(QObject *parent = 0);
    Connection(const TQProfile &_profile, QObject *parent = 0);
    Connection(QString uri, QObject *parent = 0);
    ~Connection();

    Connection(const Connection&) = delete;
    Connection(Connection&&) = default;

    const TQProfile &getProfile() const;
    const QString &getName() const;
    QByteArray getURI() const;
    void setProfile(TQProfile p);
    bool isValid() const;
    const bool &isRunning() const;
    void latencyTest();

signals:
    void stateChanged(bool started);
    void latencyAvailable(const int);
    void newLogAvailable(const QString &);
    void dataUsageChanged(const quint64 &current, const quint64 &total);
    void startFailed();

public slots:
    void start();
    void stop();
    void onStartFailed();

private:
    QString configFile;
    PrivoxyThread *privoxy;
    ServiceThread *service;
    Tun2socksThread *tun2socks;
    TQProfile profile;
    bool running;

    void testAddressLatency(const QHostAddress &addr);

    friend class EditDialog;
    friend class ConfigHelper;
    friend class StatusDialog;
    friend class ConnectionItem;

private slots:
    void onServerAddressLookedUp(const QHostInfo &host);
    void onLatencyAvailable(const int);

};
Q_DECLARE_METATYPE(Connection*)

#endif // CONNECTION_H
