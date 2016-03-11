#ifndef CONNMANCOMPONENT_H
#define CONNMANCOMPONENT_H

#include <QObject>
#include <QStringList>
#include <ComponentManager.h>

#include "connman-qt5/networkmanager.h"
#include "connman-qt5/useragent.h"

class ConnmanComponent : public ComponentBase
{
  Q_OBJECT
  DEFINE_SINGLETON(ConnmanComponent);

public:

  virtual bool componentExport() { return true; }
  virtual const char* componentName() { return "system.network"; }
  virtual bool componentInitialize();

  // technologies related functions
  Q_INVOKABLE QStringList getTechnologies();
  Q_INVOKABLE bool enableTechnology(QString technology, bool state);
  Q_INVOKABLE bool isTechnologyEnabled(QString technology);

  // services related functions
  Q_INVOKABLE QStringList getServices(QString technology);
  Q_INVOKABLE bool scan(QString technology);
  Q_INVOKABLE bool connectService(QString technology, QString service);
  Q_INVOKABLE bool isServiceConnected(QString technology, QString service);
  Q_INVOKABLE bool setServiceConfig(QString technology, QString service, QVariantMap options);
  Q_INVOKABLE QVariantMap getServiceConfig(QString technology, QString service);
  Q_INVOKABLE bool disconnectService(QString technology, QString service);
  Q_INVOKABLE void provideServicePassword(QString technology, QString service, QString password);

  // utilities functions
  Q_INVOKABLE void logInfo();

Q_SIGNALS:
  void enableStateChanged(QString technology, bool enabled);
  void serviceListChanged(QString technology, QStringList services);
  void connectionStateChanged(QString technology, QString service, bool connected);
  void serviceConfigurationChanged(QString technology, QString service);
  void requestServicePassword(QString technology, QString service);

private:
  ConnmanComponent(QObject* parent = 0);

  NetworkManager* m_networkManager;
  UserAgent* m_userAgent;

  NetworkService *getServiceForTechnology(QString technology, QString service);
  NetworkTechnology *getTechnologyForService(NetworkService *service);
  QString getServiceNameFromPath(QString path);
  NetworkService* getServiceFromPath(QString path);
  void HookServiceEvents(NetworkService *service);

private slots:
  // manager events
  void managerTechnologiesChanged();
  void managerServiceListChanged();

  // technology events
  void technologyPoweredChanged(const bool& powered);
  void technologyScanFinished();
  void technologyConnectedChanged(const bool& connected);


  // service events
  void serviceConnectedChanged(const bool& connected);
  void serviceConfigChanged();

  // agent events
   void agentUserInputRequested(const QString& servicePath, const QVariantMap& fields);
};

#endif // CONNMANCOMPONENT_H
