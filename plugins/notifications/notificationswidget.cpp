/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "notificationswidget.h"

#include <QSvgRenderer>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QIcon>

#include <DStyle>
#include <DGuiApplicationHelper>

DWIDGET_USE_NAMESPACE;

NotificationsWidget::NotificationsWidget(QWidget *parent)
    : QWidget(parent)
    , m_hover(false)
    , m_pressed(false)
{
    setMouseTracking(true);
    setMinimumSize(PLUGIN_BACKGROUND_MIN_SIZE, PLUGIN_BACKGROUND_MIN_SIZE);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] {
        update();
    });
    m_icon = QIcon::fromTheme(":/icons/resources/icons/notification.svg");
}

QSize NotificationsWidget::sizeHint() const
{
    return QSize(PLUGIN_BACKGROUND_MAX_SIZE, PLUGIN_BACKGROUND_MAX_SIZE);
}

void NotificationsWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPixmap pixmap;
    QString iconName = "notification";
    int iconSize = PLUGIN_ICON_MAX_SIZE;

    QPainter painter(this);

    if (rect().height() > PLUGIN_BACKGROUND_MIN_SIZE) {

        QColor color;
        if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
            color = Qt::black;
            painter.setOpacity(0.5);

            if (m_hover) {
                painter.setOpacity(0.6);
            }

            if (m_pressed) {
                painter.setOpacity(0.3);
            }
        } else {
            color = Qt::white;
            painter.setOpacity(0.1);

            if (m_hover) {
                painter.setOpacity(0.2);
            }

            if (m_pressed) {
                painter.setOpacity(0.05);
            }
        }

        painter.setRenderHint(QPainter::Antialiasing, true);

        DStyleHelper dstyle(style());
        const int radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);

        QPainterPath path;

        int minSize = std::min(width(), height());
        QRect rc(0, 0, minSize, minSize);
        rc.moveTo(rect().center() - rc.center());

        path.addRoundedRect(rc, radius, radius);
        painter.fillPath(path, color);
    } else if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        // 最小尺寸时，不画背景，采用深色图标
        iconName.append(PLUGIN_MIN_ICON_NAME);
    }

    painter.setOpacity(1);

    pixmap = loadSvg(iconName, QSize(iconSize, iconSize));

    const QRectF &rf = QRectF(rect());
    const QRectF &rfp = QRectF(pixmap.rect());
    painter.drawPixmap(rf.center() - rfp.center() / pixmap.devicePixelRatioF(), pixmap);
}

const QPixmap NotificationsWidget::loadSvg(const QString &fileName, const QSize &size) const
{
    const auto ratio = devicePixelRatioF();

    QPixmap pixmap;
    pixmap = QIcon::fromTheme(fileName, m_icon).pixmap(size * ratio);
    pixmap.setDevicePixelRatio(ratio);

    return pixmap;
}

void NotificationsWidget::mousePressEvent(QMouseEvent *event)
{
    m_pressed = true;
    update();

    QWidget::mousePressEvent(event);
}

void NotificationsWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    m_hover = false;
    update();

    QWidget::mouseReleaseEvent(event);
}

void NotificationsWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = true;
    QWidget::mouseMoveEvent(event);
}

void NotificationsWidget::leaveEvent(QEvent *event)
{
    if (!rect().contains(mapFromGlobal(QCursor::pos()))) {
        m_hover = false;
        m_pressed = false;
        update();
    }

    QWidget::leaveEvent(event);
}

void NotificationsWidget::resizeEvent(QResizeEvent *event)
{
    const Dock::Position position = qApp->property(PROP_POSITION).value<Dock::Position>();
    // 保持横纵比
    if (position == Dock::Bottom || position == Dock::Top) {
        setMaximumWidth(height());
        setMaximumHeight(QWIDGETSIZE_MAX);
    } else {
        setMaximumHeight(width());
        setMaximumWidth(QWIDGETSIZE_MAX);
    }

    QWidget::resizeEvent(event);
}