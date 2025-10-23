#ifndef CTBTSPIN_H
#define CTBTSPIN_H

#include <QWidget>
#include <QWheelEvent>
#include <QPushButton>
#include <QLineEdit>
#ifndef Q_OS_WIN
#include "onboard.h"
#else
#endif

class CTbtSpinLE : public QLineEdit
{
    Q_OBJECT

public:
    CTbtSpinLE(const QString &contents, QWidget *parent = nullptr);
    CTbtSpinLE(QWidget *parent = nullptr);

signals:
    void sigWheelEvent( int nSteps );

protected:
    void DefInit();
    void wheelEvent(QWheelEvent *event);            // Change the value of an input field using the mouse wheel
#ifndef Q_OS_WIN
    Onboard *m_pOnboard;                            // D-Bus interface with onboard

    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
#else
    bool event(QEvent *event) override;
    void Move4Visible();
    bool    m_bMoved;
    int     m_nLastDiff;
    QWidget *m_pParent;
#endif
};

class CTbtSpinBtn : public QPushButton
{
    Q_OBJECT

public:
    explicit CTbtSpinBtn(QWidget *parent = nullptr);
    explicit CTbtSpinBtn(const QString &text, QWidget *parent = nullptr);

signals:
    void sigMousePressEvent();
    void sigMouseReleaseEvent();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

class CTbtSpin : public QWidget
{
    Q_OBJECT

public:
    explicit CTbtSpin(QWidget *parent = nullptr);
    ~CTbtSpin();
    // Minimum
    double GetMin() { return m_lfMin; }
    void SetMin( double lfMin ) { m_lfMin = lfMin; }
    // Maximum
    double GetMax() { return m_lfMax; }
    void SetMax( double lfMax );
    // Step
    double GetStep() { return m_lfStep; }
    void SetStep( double lfStep ) { m_lfStep = lfStep; }
    // Accuracy
    int GetPrecision() { return m_nPrecision; }
    void SetPrecision( int nPrecision ) { m_nPrecision = nPrecision; }
    // Value
    void SetValue( double lfValue );
    double GetValue();
    // Timeout
    void SetTimeout( int nTimeOut );
    // On/off access
    void SetEnabled( bool bEnable );
    // Return the minimum width of the widget
    int GetMinWidth();
signals:
    void sigChanged( double lfValue );

private slots:
    void onPbUpPressed();
    void onPbUpReleased();
    void onPbDownPressed();
    void onPbDownReleased();
    void onWheelEvent(int nSteps);
    void onLeTextChanged();
private:
    enum{ DEC, INC };

    double          m_lfMax, m_lfMin, m_lfStep;
    int             m_nPrecision;
    int             m_nKeyCode, m_nTimer, m_nStopTimer, m_nTimeOut;
    CTbtSpinBtn     *m_pDecBtn;
    CTbtSpinBtn     *m_pIncBtn;
    CTbtSpinLE      *m_pLineEdit;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
    bool CheckValue(double &lfValue);
    void set_value(double lfValue, bool bEmitSig = false);
};

#endif // CTBTSPIN_H
