/*
 * Copyright (C) 2022 ~ 2022 Deepin Technology Co., Ltd.
 *
 * Author:     donghualin <donghualin@uniontech.com>
 *
 * Maintainer:  donghualin <donghualin@uniontech.com>
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
#pragma once

#include <QWidget>
#include <QTimer>
#include <QPair>

class QDBusMessage;
class BaseTrayWidget : public QWidget
{
    Q_OBJECT

public:
    enum TrayType {
        ApplicationTray,
        SystemTray,
    };

    explicit BaseTrayWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~BaseTrayWidget() override;

    virtual QString itemKeyForConfig() = 0;
    virtual void updateIcon() = 0;
    virtual void sendClick(uint8_t mouseButton, int x, int y) = 0;
    virtual inline TrayType trayType() const { return TrayType::ApplicationTray; } // default is ApplicationTray
    virtual bool isValid() {return true;}
    uint getOwnerPID();
    virtual bool needShow();
    virtual void setNeedShow(bool needShow);
    virtual QPixmap icon() = 0;

Q_SIGNALS:
    void iconChanged();
    void clicked();
    void needAttention();
    void requestWindowAutoHide(const bool autoHide);
    void requestRefershWindowVisible();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void handleMouseRelease();
    const QRect perfectIconRect() const;
    void resizeEvent(QResizeEvent *event) override;
    void setOwnerPID(uint PID);

private:
    QTimer *m_handleMouseReleaseTimer;

    QPair<QPoint, Qt::MouseButton> m_lastMouseReleaseData;

    uint m_ownerPID;
    bool m_needShow;
};
