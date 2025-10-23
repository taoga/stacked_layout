#include "ctbtspin.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QGuiApplication>

// CTbtSpinLE implementation
CTbtSpinLE::CTbtSpinLE(const QString &contents, QWidget *parent ): QLineEdit( contents, parent )
{
    DefInit();
}

CTbtSpinLE::CTbtSpinLE(QWidget *parent ): QLineEdit( parent )
{
    DefInit();
}

void CTbtSpinLE::DefInit()
{
#ifndef Q_OS_WIN
    m_pOnboard = new Onboard(this);
#else
    m_bMoved = false;
    m_nLastDiff = 0;
    m_pParent = 0;

//    setAttribute(Qt::WA_InputMethodEnabled, true);
//    setInputMethodHints(inputMethodHints() | Qt::InputMethodHint::ImhFormattedNumbersOnly);
//    m_pParent = parentWidget();
//    while( m_pParent && m_pParent->parentWidget() )
//        m_pParent = m_pParent->parentWidget();
#endif
}

#ifndef Q_OS_WIN
void CTbtSpinLE::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    if( m_pOnboard )
        QTimer::singleShot( 300, m_pOnboard, SLOT(show()) );
}
void CTbtSpinLE::focusOutEvent(QFocusEvent *event)
{
    if( m_pOnboard ) m_pOnboard->hide();
    QLineEdit::focusOutEvent(event);
}
#else
void CTbtSpinLE::Move4Visible()
{
    if( m_pParent )
    {
        QRectF  rectKbd = QGuiApplication::inputMethod()->keyboardRectangle();
        bool    bKbdVisible = QGuiApplication::inputMethod()->isVisible();
        int     nBottomY = mapToGlobal( rect().bottomLeft() ).y();
        int     nKbdTopY = rectKbd.topLeft().y();
        int     nOrgY = nKbdTopY - nBottomY;

        // move main widget
        if( bKbdVisible && !m_bMoved && nOrgY < 0 && hasFocus() )
        {
            m_bMoved = true;
            m_nLastDiff = nOrgY;
            QRect rectGeometry = m_pParent->frameGeometry();
            m_pParent->move( rectGeometry.x(), rectGeometry.y() - qAbs(m_nLastDiff));
        }
        // roll back
        if( !bKbdVisible && m_bMoved )
        {
            m_bMoved = false;
            QRect rectGeometry = m_pParent->frameGeometry();
            m_pParent->move( rectGeometry.x(), rectGeometry.y() + qAbs(m_nLastDiff));
        }
    }
}
bool CTbtSpinLE::event(QEvent *event)
{
    Move4Visible();
    return QLineEdit::event( event );
}
#endif

void CTbtSpinLE::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if( !numDegrees.isNull() )
    {
        QPoint numSteps = numDegrees / 15;
        emit sigWheelEvent( numSteps.y() );
    }
    event->accept();
}

// CTbtSpinBtn implemetation
CTbtSpinBtn::CTbtSpinBtn(QWidget *parent) : QPushButton(parent)
{
}

CTbtSpinBtn::CTbtSpinBtn(const QString &text, QWidget *parent) : QPushButton( text, parent )
{
}

void CTbtSpinBtn::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit sigMousePressEvent();
}

void CTbtSpinBtn::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit sigMouseReleaseEvent();
}

// CTbtSpin implementation
CTbtSpin::CTbtSpin(QWidget *parent) :
    QWidget(parent)//,
{
    m_lfMin = 0.0;
    m_lfMax = 100.0;
    m_lfStep = 1.0;
    m_nPrecision = 0;
    m_nKeyCode = -1;
    m_nTimer = -1;
    m_nStopTimer = 1;
    m_nTimeOut = 150;

    QString         tmpStr = "<";
    QFontMetrics    qfmTemp( font() );

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    m_pDecBtn = new CTbtSpinBtn( tmpStr, this);
    #if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    m_pDecBtn->setMaximumWidth( qfmTemp.width(tmpStr) * 4);
    #else
    m_pDecBtn->setMaximumWidth( qfmTemp.horizontalAdvance(tmpStr) * 4);
    #endif
    tmpStr = ">";
    m_pIncBtn = new CTbtSpinBtn( tmpStr, this);
    #if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    m_pIncBtn->setMaximumWidth( qfmTemp.width(tmpStr) * 4);
    #else
    m_pIncBtn->setMaximumWidth( qfmTemp.horizontalAdvance(tmpStr) * 4);
    #endif
    m_pLineEdit = new CTbtSpinLE(this);

    pLayout->addWidget( m_pDecBtn );
    pLayout->addWidget( m_pLineEdit );
    pLayout->addWidget( m_pIncBtn );
    pLayout->setMargin( 1 );
    pLayout->setSpacing( 1 );

    setLayout( pLayout );

    connect( m_pDecBtn, SIGNAL( sigMousePressEvent() ), this, SLOT( onPbDownPressed() ) );
    connect( m_pDecBtn, SIGNAL( sigMouseReleaseEvent() ), this, SLOT( onPbDownReleased() ) );

    connect( m_pIncBtn, SIGNAL( sigMousePressEvent() ), this, SLOT( onPbUpPressed() ) );
    connect( m_pIncBtn, SIGNAL( sigMouseReleaseEvent() ), this, SLOT( onPbUpReleased() ) );

    connect( m_pLineEdit, SIGNAL( sigWheelEvent(int) ), this, SLOT( onWheelEvent(int)) );
    connect( m_pLineEdit, SIGNAL( editingFinished() ), this, SLOT( onLeTextChanged() ) );

    set_value( m_lfMin );
}

CTbtSpin::~CTbtSpin()
{
}

void CTbtSpin::SetMax(double lfMax)
{
    m_lfMax = lfMax;

    QString qsTemp, qsFormat;

    qsFormat = QString::asprintf("%%0.%dlf", m_nPrecision );
    qsTemp = QString::asprintf( qsFormat.toStdString().data(), m_lfMax );

    QFontMetrics tmpFontMetrics( font() );

    #if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    m_pLineEdit->setMinimumWidth( tmpFontMetrics.width( qsTemp ) );
    #else
    m_pLineEdit->setMinimumWidth( tmpFontMetrics.horizontalAdvance( qsTemp ) );
    #endif
}

void CTbtSpin::onPbUpPressed()
{
    m_nKeyCode = CTbtSpin::INC;
    m_nTimer = startTimer( m_nTimeOut );
    m_nStopTimer = 0;
}


void CTbtSpin::onPbUpReleased()
{
    m_nStopTimer = 1;
}

void CTbtSpin::onPbDownPressed()
{
    m_nKeyCode = CTbtSpin::DEC;
    m_nTimer = startTimer( m_nTimeOut );
    m_nStopTimer = 0;
}

void CTbtSpin::onPbDownReleased()
{
    m_nStopTimer = 1;
}

void CTbtSpin::SetValue( double lfValue )
{
    set_value( lfValue );
}

double CTbtSpin::GetValue()
{
    return m_pLineEdit->text().toDouble();
}

void CTbtSpin::SetTimeout(int nTimeOut)
{
    if( nTimeOut > 0 )
        m_nTimeOut = nTimeOut;
}

void CTbtSpin::SetEnabled(bool bEnable)
{
    m_pDecBtn->setEnabled( bEnable );
    m_pIncBtn->setEnabled( bEnable );
    m_pLineEdit->setEnabled( bEnable );
}

void CTbtSpin::timerEvent(QTimerEvent *event)
{
    if( m_nTimer == event->timerId() )  // Наш таймер
    {
        double  lfValue = 0;

        switch( m_nKeyCode )
        {
            case CTbtSpin::DEC:
                lfValue = m_pLineEdit->text().toDouble();
                lfValue -= m_lfStep;
                set_value( lfValue, true );
            break;

            case CTbtSpin::INC:
                lfValue = m_pLineEdit->text().toDouble();
                lfValue += m_lfStep;
                set_value( lfValue, true );
            break;
        }

        if( m_nStopTimer )
        {
            killTimer( m_nTimer );
            m_nTimer = -1;
            // Code of the pressed button, min. and max. values
            m_nKeyCode = -1;
        }
    }
}

bool CTbtSpin::CheckValue(double &lfValue)
{
    bool bRet = true;

    if( lfValue < m_lfMin ) {lfValue = m_lfMin; bRet = false;}
    if( lfValue > m_lfMax ) {lfValue = m_lfMax; bRet = false;}

    return bRet;
}

void CTbtSpin::onLeTextChanged()
{
    double  lfValue = m_pLineEdit->text().toDouble();

    if( !CheckValue( lfValue ) )
        set_value( lfValue, true );
    else
        emit sigChanged( lfValue );
}

void CTbtSpin::set_value( double lfValue, bool bEmitSig )
{
    QString qsTemp, qsFormat;

    CheckValue( lfValue );

    qsFormat = QString::asprintf("%%0.%dlf", m_nPrecision );

    qsTemp = QString::asprintf( qsFormat.toStdString().data(), lfValue );

    m_pLineEdit->setText( qsTemp );

    if( bEmitSig )
        emit sigChanged( lfValue );
}

void CTbtSpin::onWheelEvent( int nSteps )
{
    double lfValue = GetValue();

    lfValue += m_lfStep*(double)(nSteps);
    set_value( lfValue, true );
}
// Return the minimum width of the widget
int CTbtSpin::GetMinWidth()
{
    int nResult = 0;

    nResult += m_pDecBtn->width();
    nResult += m_pIncBtn->width();
    nResult += m_pLineEdit->width();

    return nResult;
}
