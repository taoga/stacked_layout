#include "cribbonopts.h"
#include <QHBoxLayout>
#include <QScroller>
#include <QDebug>

// QScrollAreaExt class implementation
QScrollAreaExt::QScrollAreaExt(QWidget *parent) : QScrollArea(parent)
{
}

void QScrollAreaExt::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy( dx, dy);
    emit sigScrollContentsBy( dx, dy);
}

// CRibbonOpts class implementation
CRibbonOpts::CRibbonOpts(QWidget *parent) : QWidget(parent)
{
    // Init position of links in a control
    m_u8Type = TYPE_SC_TOP;// | TYPE_SC_ICONS_ONLY;
    //m_u8Type = TYPE_SC_BOTTOM;// | TYPE_SC_ICONS_ONLY;
    //m_u8Type = TYPE_SC_LEFT | TYPE_SC_ICONS_ONLY;
    //m_u8Type = TYPE_SC_RIGHT | TYPE_SC_ICONS_ONLY;
    Init();
}

CRibbonOpts::CRibbonOpts(quint8 u8Type, QWidget *parent) : QWidget(parent)
{
    m_u8Type = u8Type;
    Init();
}

CRibbonOpts::~CRibbonOpts()
{
}
/////////////////////////////////////////////////////////////////////////////////////////
// Initializing a control
void CRibbonOpts::Init()
{
    m_bDisableEWV = false;

    m_pListLinks = new QListWidget( this );         // Link display element
    m_pScrollArea = new QScrollAreaExt( this );     // Scrolling widget

    if( !m_pListLinks ) return;
    if( !m_pScrollArea ) return;

    if( m_pScrollArea )
    {
        // hide scrollbars
        m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // configure gesture and add rubberband effect
        QScroller::grabGesture(m_pScrollArea, QScroller::LeftMouseButtonGesture);

        m_pScrollArea->setWidgetResizable( true );
    }

    if( m_u8Type & TYPE_SC_ICONS_ONLY ) {
        m_pListLinks->setFocusPolicy( Qt::NoFocus );
        m_pListLinks->setViewMode( QListView::IconMode );
    }

    m_pListLinks->setStyleSheet("QListWidget { border : none; background : palette(window); }" );

    m_pListLinks->setAcceptDrops( false );
    m_pListLinks->setDragDropMode( QAbstractItemView::NoDragDrop );
    m_pListLinks->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pListLinks->setSelectionBehavior(QAbstractItemView::SelectItems);

    m_pContainer = new QWidget( m_pScrollArea );                                  // The base widget that will contain the parameter widgets
    // Basic grouping of elements
    m_pContainerLayout = new QVBoxLayout();
    m_pContainerLayout->setContentsMargins( 0, 0, 0, 0 );

    if( (m_u8Type & TYPE_SC_LEFT) || (m_u8Type & TYPE_SC_RIGHT) )
    { // Horizontal alignment
        QHBoxLayout *pLayout = new QHBoxLayout(this);
        m_pListLinks->setFlow( QListWidget::TopToBottom );
        if( pLayout )
        {
            pLayout->setContentsMargins( 0, 0, 0, 0 );
            if( m_u8Type & TYPE_SC_LEFT )
            {
                pLayout->addWidget( m_pListLinks );
                pLayout->addWidget( m_pScrollArea );
            }
            else
            {
                pLayout->addWidget( m_pScrollArea );
                pLayout->addWidget( m_pListLinks );
            }
        }
    }
    else
    if( (m_u8Type & TYPE_SC_TOP) || (m_u8Type & TYPE_SC_BOTTOM) )
    { // Vertical alignment
        QVBoxLayout *pLayout = new QVBoxLayout(this);
        m_pListLinks->setFlow( QListWidget::LeftToRight );
        if( pLayout )
        {
            pLayout->setContentsMargins( 0, 0, 0, 0 );
            if( m_u8Type & TYPE_SC_TOP )
            {
                pLayout->addWidget( m_pListLinks );
                pLayout->addWidget( m_pScrollArea );
            }
            else
            {
                pLayout->addWidget( m_pScrollArea );
                pLayout->addWidget( m_pListLinks );
            }
        }
    }

    connect( m_pListLinks, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(onCurrentItemChanged(QListWidgetItem*, QListWidgetItem*)) );
    connect( m_pScrollArea, SIGNAL(sigScrollContentsBy(int,int)), this, SLOT(onScrollContentsBy(int,int)) );
}
/////////////////////////////////////////////////////////////////////////////////////////
// Add parameter group
// Parameters:
// pWidget          - pointer to parameter group form object
// csLabel          - Name of the parameter to be displayed on the info panel
// csIcon           - Name of image file from resources
// csIcon           - Name of the image file from the resources for the selected element
void CRibbonOpts::AddOpt(QWidget *pWidget, QString qsLabel, QString csIcon, QString csSelIcon )
{
    if( !pWidget || !m_pContainerLayout ) return;
    // Add the settings widget to the list, you will need it
    int nWgtIdx = m_qvecOptsWgts.size();
    m_qvecOptsWgts.push_back( pWidget );
    // Add a widget to the feed base
    m_pContainerLayout->addWidget( pWidget );

    QIcon   tmpIcon;
    tmpIcon.addPixmap( QPixmap(csIcon), QIcon::Normal );
    tmpIcon.addPixmap( QPixmap(csSelIcon), QIcon::Selected );

    QListWidgetItem *pListWgtItem = new QListWidgetItem( tmpIcon, (m_u8Type & TYPE_SC_ICONS_ONLY) ? NULL : qsLabel, m_pListLinks );

    if( pListWgtItem )
    {
        pListWgtItem->setData( Qt::UserRole, nWgtIdx );     // Store widget index in array
        if( m_u8Type & TYPE_SC_ICONS_ONLY )
            pListWgtItem->setToolTip( qsLabel );            // Set tooltip if only icons are shown
    }
}

void CRibbonOpts::SetContainer()
{
    if( m_pScrollArea && m_pContainer && m_pContainerLayout )
    {
        m_pContainer->setLayout( m_pContainerLayout );
        m_pScrollArea->setWidget( m_pContainer );

        if( m_pListLinks->count() > 0 )
            m_pListLinks->setCurrentItem( m_pListLinks->item( 0 ) );
        m_pScrollArea->ensureVisible( 0, 0 );
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
// Resizing a widget
void CRibbonOpts::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QSize qszClient = event->size();
    if( qszClient.width() == 0 || qszClient.height() == 0 ) return;

    if( m_pListLinks  )
    {

        QFontMetrics    qfmTemp( m_pListLinks->font() );
        int             nItem = 0, nItems = m_pListLinks->count();
        int             nIconWidth = 0, nIconHeight = 0;
        QString         qsItemText;

        if( nItems > 0 )
        {
            for( nItem = 0; nItem < nItems; nItem++ )
            {
                QListWidgetItem *pTmpItem = m_pListLinks->item( nItem );

                QIcon           tmpIcon = pTmpItem->icon();
                QList<QSize>    tmpList =  tmpIcon.availableSizes();
                QSize           qsIconSize;

                if( tmpList.size() > 0 )
                {
                    qsIconSize = tmpList.at(0);
                    if( nIconWidth < qsIconSize.width() ) nIconWidth = qsIconSize.width();
                    if( nIconHeight < qsIconSize.height() ) nIconHeight = qsIconSize.height();
                }
                // Save the longest line
                if( qsItemText.size() < pTmpItem->text().size() ) qsItemText = pTmpItem->text();
            }
        }
        int nMax = DEF_ICON_SIZE;
        if( (m_u8Type & TYPE_SC_LEFT) || (m_u8Type & TYPE_SC_RIGHT) )
        {// Horizontal position, width
            if( nIconWidth > 0 ) nMax = nIconWidth;

            if( !(m_u8Type & TYPE_SC_ICONS_ONLY) )
                nMax += qfmTemp.width( qsItemText ) + qfmTemp.width( 'O' );
            // Set fixed size of elements
            for( nItem = 0; nItem < nItems; nItem++ )
            {
                QListWidgetItem *pTmpItem = m_pListLinks->item( nItem );
                pTmpItem->setSizeHint( QSize( nMax, nIconHeight ) );
            }
            m_pListLinks->setMaximumWidth( nMax );
        }
        else
        {
            // Vertical position, height
            if( nIconHeight > 0 ) nMax = nIconHeight;
            m_pListLinks->setMaximumHeight( nMax - nMax / 4 );
        }
    }
}
// Options widget selection event handler
void CRibbonOpts::onCurrentItemChanged(QListWidgetItem *pCurrent, [[maybe_unused]]QListWidgetItem *pPrevious )
{
    if( !pCurrent  ) return;

    int nIdxWgt = pCurrent->data( Qt::UserRole ).toInt();
    if( nIdxWgt < 0 || nIdxWgt >= m_qvecOptsWgts.size() ) return;

    if( !m_bDisableEWV )
    {
        QWidget *pCurWgt = m_qvecOptsWgts[nIdxWgt];
        m_pScrollArea->ensureWidgetVisible( pCurWgt );
    }
    m_bDisableEWV = false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Temporary - set widget background color
void CRibbonOpts::SetWidgetBkground( QWidget *pWgt, int nR, int nG, int nB )
{
    if( !pWgt ) return;

    QPalette tmpPal = pWgt->palette();

    tmpPal.setColor( QPalette::Background, QColor( qRgb( nR, nG, nB ) ) );
    pWgt->setAutoFillBackground( true );
    pWgt->setPalette( tmpPal );
}
// Widget offset signal handler
void CRibbonOpts::onScrollContentsBy( [[maybe_unused]]int dx, [[maybe_unused]]int dy )
{
    // Determine the visible widget
    int i = 0, nWgts = m_qvecOptsWgts.size(), nMaxHeight = 0, nVisible = -1;
    for( i = 0; i < nWgts; i++ )
    {
        if( !(m_qvecOptsWgts[i]->visibleRegion().isEmpty()) )
        {
            QRect tmpRect = m_qvecOptsWgts[i]->visibleRegion().boundingRect();
            if( !tmpRect.isNull() )
                if( tmpRect.height() > nMaxHeight ) {nMaxHeight = tmpRect.height(); nVisible = i; }
        }
    }
    // Make it current, no scrolling
    if( nVisible != -1 )
    {
        m_bDisableEWV = (m_pListLinks->currentRow() == nVisible) ? false : true; // Disable scrollbar offset when current tab is selected

        if( m_pListLinks->currentRow() != nVisible )
            m_pListLinks->setCurrentRow( nVisible );
    }
}
