// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "airplanemodeapplet.h"
#include "constants.h"

#include <DSwitchButton>

#include <QLabel>
#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

AirplaneModeApplet::AirplaneModeApplet(QWidget *parent)
    : QWidget(parent)
    , m_switchBtn(new DSwitchButton(this))
{
    setMinimumWidth(PLUGIN_ITEM_WIDTH - 40);
    setFixedHeight(40);
    QLabel *title = new QLabel(this);
    title->setText(tr("Airplane Mode"));
    QFont font;
    font.setPixelSize(17);
    title->setFont(font);
    title->setForegroundRole(QPalette::BrightText);
    QHBoxLayout *appletlayout = new QHBoxLayout(this);
    appletlayout->setMargin(0);
    appletlayout->setSpacing(0);
    appletlayout->addSpacing(0);
    appletlayout->addWidget(title);
    appletlayout->addStretch();
    appletlayout->addWidget(m_switchBtn);
    appletlayout->addSpacing(0);
    appletlayout->setContentsMargins(20, 0, 6, 0);
    setLayout(appletlayout);
    connect(m_switchBtn, &DSwitchButton::checkedChanged, this, &AirplaneModeApplet::enableChanged);
}

void AirplaneModeApplet::setEnabled(bool enable)
{
    m_switchBtn->blockSignals(true);
    m_switchBtn->setChecked(enable);
    m_switchBtn->blockSignals(false);
}
