#include "widget/ipedit.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>

#include <QEvent>
#include <QKeyEvent>

namespace ltoufi {

IPv4Edit::IPv4Edit(QHostAddress hint, QWidget *parent)
    : IPEdit(parent), mLineEdits()
{
    QHBoxLayout* lay = new QHBoxLayout(this);
    setLayout(lay);
    lay->setContentsMargins(1, 1, 1, 1);
    lay->setSpacing(0);

    for (size_t i = 0 ; i < mLineEdits.size() ; ++i) {
        mLineEdits[i] = new QLineEdit(this);
        lay->addWidget(mLineEdits[i]);

        mLineEdits[i]->setFrame(false);
        mLineEdits[i]->setAlignment(Qt::AlignHCenter | Qt::AlignBaseline);

        QFont font { mLineEdits[i]->font() };
        font.setStyleHint(QFont::Monospace);
        font.setFixedPitch(true);
        mLineEdits[i]->setFont(font);

        mLineEdits[i]->installEventFilter(this);
        mLineEdits[i]->setValidator(new QIntValidator(0, 255, mLineEdits[i]));

        if (i != mLineEdits.size() - 1) {
            QLabel* dotLbl = new QLabel(".", this);
            //dotLbl->setStyleSheet("background: white");
            dotLbl->setBuddy(mLineEdits[i]);

            lay->addWidget(dotLbl);
            lay->setStretch(lay->count() - 1, 0);
        }

        mLineEdits[i]->setText(QStringLiteral("."));
        connect(mLineEdits[i], &QLineEdit::textEdited, this, &IPv4Edit::onTextEdited);
    }

    setMinimumWidth(30 * mLineEdits.size());
    setFixedHeight(30);

    setAddress(hint);
    mLineEdits[0]->selectAll();
}

QHostAddress IPv4Edit::address() const {
    return QHostAddress(mLineEdits[0]->text() + '.'
            + mLineEdits[1]->text() + '.'
            + mLineEdits[2]->text() + '.'
            + mLineEdits[3]->text());
}

bool IPv4Edit::setAddress(QHostAddress address) {
    bool ok;
    quint32 ipv4 = address.toIPv4Address(&ok);
    if (ok) {
        mLineEdits[0]->setText(QString::number((ipv4 >> 24) & 0xFF));
        mLineEdits[1]->setText(QString::number((ipv4 >> 16) & 0xFF));
        mLineEdits[2]->setText(QString::number((ipv4 >> 8) & 0xFF));
        mLineEdits[3]->setText(QString::number(ipv4 & 0xFF));
    }
    return ok;
}

void IPv4Edit::onTextEdited() {
    QLineEdit* sender = qobject_cast<QLineEdit*>(QObject::sender());

    if (sender) {
        for (size_t i = 0; i < mLineEdits.size() - 1 ; ++i ) {
            if (sender == mLineEdits[i] && sender->text().size() == 3
                    && sender->text().size() == sender->cursorPosition()) {
                // auto-move to next item
                mLineEdits[i+1]->setFocus();
                mLineEdits[i+1]->selectAll();
            }
        }

        emit addressChanged(address());
    }
}

bool IPv4Edit::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent) {
            for (size_t i = 0 ; i != mLineEdits.size() ; ++i) {
                if (mLineEdits[i] == obj) {
                    switch (keyEvent->key()) {
                    case Qt::Key_Left:
                        if (mLineEdits[i]->cursorPosition() == 0) {
                            // user wants to move to previous item
                            if (i > 0) {
                                mLineEdits[i-1]->setFocus();
                                mLineEdits[i-1]->selectAll();
                                return true;
                            }
                        }
                        break;

                    case Qt::Key_Right:
                        if (mLineEdits[i]->text().isEmpty() ||
                                (mLineEdits[i]->text().size() == mLineEdits[i]->cursorPosition())) {
                            // user wants to move to next item
                            if (i+1 < mLineEdits.size()) {
                                mLineEdits[i+1]->setFocus();
                                mLineEdits[i+1]->selectAll();
                                return true;
                            }
                        }
                        break;
                    }

                    break; // exit for-loop
                }
            }
        }
    }

    return IPEdit::eventFilter(obj, event);
}

}
