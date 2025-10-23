#ifndef ONBOARD_H
#define ONBOARD_H

#include <QtCore>
#include <QtDBus>

class Onboard : public QObject
{
	Q_OBJECT

public:
	explicit Onboard(QObject *parent = 0);

	bool runOnboard();

	void hide();	
	bool isVisible();

	bool autoRun();
	void setAutoRun(bool autoRun);

public slots:
    void show();

signals:
	void visibilityUpdated(bool isVisible);

private:
	static const QString DBUS_SERVICE_NAME;
	static const QString DBUS_PATH;

	bool _isVisible;

	bool _autoRun;

	QDBusInterface _dbusInterface;
	QDBusConnectionInterface* _dbusDeamon;

	bool makeConnection();
	void unexpectedMessage(const QDBusMessage &msg);

	void setVisibility(bool isVisible);
};

#endif // ONBOARD_H
