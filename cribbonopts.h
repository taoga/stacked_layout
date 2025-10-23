#ifndef CRIBBONOPTS_H
#define CRIBBONOPTS_H

#include <QWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QResizeEvent>
#include <QVBoxLayout>

// Custom QScrollArea
class QScrollAreaExt : public QScrollArea
{
    Q_OBJECT
public:
    explicit QScrollAreaExt(QWidget *parent = nullptr);

signals:
    void sigScrollContentsBy( int dx, int dy);
    // QAbstractScrollArea interface
protected:
    void scrollContentsBy(int dx, int dy) override;
};

class CRibbonOpts : public QWidget
{
    Q_OBJECT
public:
    enum TypeFlags {                                // Position of links in a control
        TYPE_SC_LEFT       = 1,
        TYPE_SC_RIGHT      = 2,
        TYPE_SC_TOP        = 4,
        TYPE_SC_BOTTOM     = 8,
        TYPE_SC_ICONS_ONLY = 16
    };
    static constexpr quint16 DEF_ICON_SIZE = 24;    // Default icon size

    explicit CRibbonOpts(QWidget *parent = nullptr);
    explicit CRibbonOpts( quint8 u8Type = TYPE_SC_LEFT | TYPE_SC_ICONS_ONLY, QWidget *parent = nullptr);
    ~CRibbonOpts();

    // Add a group of options to the ribbon
    void AddOpt(QWidget *pWidget, QString qsLabel, QString csIcon, QString csSelIcon );
    QWidget*    GetContainer() { return m_pContainer; }
    void SetContainer();

signals:
protected:
    quint8              m_u8Type;                                   // Position of links in a control
    QListWidget         *m_pListLinks;                              // Link display element
    QScrollAreaExt      *m_pScrollArea;                             // Scrolling widget
    QWidget             *m_pContainer;                              // The base widget that will contain the parameter widgets
    QVBoxLayout         *m_pContainerLayout;                        // Basic grouping of elements
    QVector<QWidget*>   m_qvecOptsWgts;                             // Parameter containers
    bool                m_bDisableEWV;                              // Disable scrollbar offset when current tab is selected

    void Init();                                                    // Initializing a control
    void SetWidgetBkground(QWidget *pWgt, int nR, int nG, int nB);  // Temporary - set widget background color

    // QWidget interface
    void resizeEvent(QResizeEvent *event) override;

protected slots:
    void onCurrentItemChanged(QListWidgetItem *pCurrent, QListWidgetItem *pPrevious);   // Options widget selection event handler
    void onScrollContentsBy(int dx, int dy);                                            // Widget offset signal handler
};

#endif // CRIBBONOPTS_H
