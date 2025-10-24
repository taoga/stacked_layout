#include "onboard.h"

const QString Onboard::DBUS_SERVICE_NAME = "org.onboard.Onboard";
const QString Onboard::DBUS_PATH = "/org/onboard/Onboard/Keyboard";

Onboard::Onboard(QObject *parent) :
	QObject(parent),
	_autoRun(true),
    _dbusInterface(DBUS_SERVICE_NAME, DBUS_PATH)
{
    _dbusDeamon = _dbusInterface.connection().interface();

    _isVisible = isVisible();
}

bool Onboard::runOnboard()
{
	if (_dbusDeamon->isServiceRegistered(_dbusInterface.service()))
	{
		setVisibility(true);
		return true;
	}

    QStringList tmpList;
    //bool success = QProcess::startDetached("onboard");
    bool success = QProcess::startDetached("onboard", tmpList );
    if (success)
    {
		setVisibility(true);
    }
    else
    {
        setVisibility(false);
        qCritical("Unable to run Onboard application.");
    }

	return success;
}

void Onboard::show()
{
	if (_dbusDeamon->isServiceRegistered(_dbusInterface.service()))
	{
		QDBusMessage reply = _dbusInterface.call("Show");

		if (reply.type() == QDBusMessage::ReplyMessage)
		{
			setVisibility(true);
		}
		else
		{
			unexpectedMessage(reply);
		}
	}
	else if (_autoRun)
	{
		runOnboard();
	}
	else
	{
		setVisibility(false);
	}
}

void Onboard::hide()
{
	if (_dbusDeamon->isServiceRegistered(_dbusInterface.service()))
	{
		QDBusMessage reply = _dbusInterface.call("Hide");

		if (reply.type() == QDBusMessage::ReplyMessage)
		{
			setVisibility(false);
		}
		else
		{
			unexpectedMessage(reply);
		}
	}
	else
	{
		setVisibility(false);
	}
}

bool Onboard::isVisible()
{
	if (_dbusDeamon->isServiceRegistered(_dbusInterface.service()))
	{
		QDBusMessage reply = _dbusInterface.call("IsVisible");

		if (reply.type() != QDBusMessage::ReplyMessage
			|| reply.arguments().size() != 1)
		{
			unexpectedMessage(reply);
			setVisibility(false);
			return false;
		}

		bool result = reply.arguments().first().toBool();
		setVisibility(result);
		return result;
	}
	else
	{
		setVisibility(false);
		return false;
	}
}

bool Onboard::autoRun()
{
	return _autoRun;
}

void Onboard::setAutoRun(bool autoRun)
{
	_autoRun = autoRun;
}

void Onboard::unexpectedMessage(const QDBusMessage &msg)
{
	qCritical() << "Unexpected D-Bus message:" << msg.errorName() << msg.errorMessage();
}

void Onboard::setVisibility(bool isVisible)
{
	if (_isVisible != isVisible)
	{
		_isVisible = isVisible;
		emit visibilityUpdated(_isVisible);
	}
}
