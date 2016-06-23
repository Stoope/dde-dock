#include "launcheritem.h"
#include "util/themeappicon.h"

#include <QPainter>
#include <QProcess>
#include <QMouseEvent>

LauncherItem::LauncherItem(QWidget *parent)
    : DockItem(DockItem::Launcher, parent)
{
}

void LauncherItem::paintEvent(QPaintEvent *e)
{
    DockItem::paintEvent(e);

    if (!isVisible())
        return;

    QPainter painter(this);
    if (DockDisplayMode == Fashion)
        painter.drawPixmap(rect().center() - m_largeIcon.rect().center(), m_largeIcon);
    else
        painter.drawPixmap(rect().center() - m_smallIcon.rect().center(), m_smallIcon);
}

void LauncherItem::resizeEvent(QResizeEvent *e)
{
    DockItem::resizeEvent(e);

    const int iconSize = qMin(width(), height());
    m_smallIcon = ThemeAppIcon::getIcon("deepin-launcher", iconSize * 0.6);
    m_largeIcon = ThemeAppIcon::getIcon("deepin-launcher", iconSize * 0.8);
}

void LauncherItem::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton && !perfectIconRect().contains(e->pos()))
        return QWidget::mousePressEvent(e);

    if (e->button() != Qt::LeftButton)
        return;

    QProcess *proc = new QProcess;

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), proc, &QProcess::deleteLater);

    QStringList args = QStringList() << "--print-reply"
                                     << "--dest=com.deepin.dde.Launcher"
                                     << "/com/deepin/dde/Launcher"
                                     << "com.deepin.dde.Launcher.Toggle";

    proc->start("dbus-send", args);
}