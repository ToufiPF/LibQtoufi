#pragma once

#include <QWidget>
#include <QHostAddress>

#include <QLineEdit>
#include <array>

#include "libqtoufi_global.hpp"

namespace ltoufi {

class LIBQTOUFI_EXPORT IPEdit : public QWidget {
    Q_OBJECT
public:
    inline explicit IPEdit(QWidget* parent = nullptr) : QWidget(parent) {}

    virtual QHostAddress address() const = 0;
    virtual bool setAddress(QHostAddress) = 0;

signals:
    void addressChanged(QHostAddress newAddress);
};

class LIBQTOUFI_EXPORT IPv4Edit final : public IPEdit {
    Q_OBJECT
public:
    explicit IPv4Edit(QHostAddress hint = QHostAddress::LocalHost, QWidget* parent = nullptr);

    virtual QHostAddress address() const override;
    virtual bool setAddress(QHostAddress) override;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onTextEdited();

private:
    std::array<QLineEdit*, 4> mLineEdits;
};

}
