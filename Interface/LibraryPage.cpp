//
// Created by Fardeen Bablu on 7/13/24.
//

#include "LibraryPage.h"

LibraryPage::LibraryPage(QWidget *parent) :
        QWidget(parent),
        ui(new QVBoxLayout(this)),
        pageLabel(new QLabel("Your Library")),
        createSetButton(new QPushButton("+ Create Set", this)) {

    // Literally just adding things to the UI, pretty intuitive
    ui->addWidget(pageLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    ui->addWidget(createSetButton, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Connecting a "label" and "button" component to THIS page
    connect(createSetButton, &QPushButton::clicked, this, &LibraryPage::createSetClicked);
    setLayout(ui);
}

void LibraryPage::addSetButton(const QString &setName) {
    QPushButton *setButton = new QPushButton(setName, this);
    ui->addWidget(setButton);
    connect(setButton, &QPushButton::clicked, [this, setName]() {
        emit openSetClicked(setName);
    });
}
