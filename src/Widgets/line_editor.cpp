#include <QIcon>

#include "line_editor.h"
#include <iostream>
LineEditor::LineEditor(QWidget *parent)
	: QWidget{parent},
	layout(new QHBoxLayout(this)),
	inputField(new QLineEdit(this)),
	deleteButton(new QPushButton(this))
{

	//focus
	setFocusProxy(inputField);

	deleteButton->setFocusPolicy(Qt::NoFocus);

	//view and layout

    deleteButton->setFixedSize(50, 50);
    QIcon deleteIcon(DELETE_ICON_LOCATION);
    std::cout << deleteIcon.isNull() << deleteIcon.isNull() << deleteIcon.isNull() << deleteIcon.isNull() << std::endl;
    deleteButton->setIcon(QIcon(DELETE_ICON_LOCATION));
    deleteButton->setIconSize(deleteButton->size());

    deleteButton->setAutoFillBackground(false);

    inputField->setFont(QFont("Monospace", 18));

    setFixedHeight(50);

    inputField->setFixedHeight(50);

	layout->addWidget(inputField);
	layout->addWidget(deleteButton,0, Qt::AlignLeft);

	inputField->setContentsMargins(0,0,0,0);
	deleteButton->setContentsMargins(0,0,0,0);
	layout->setContentsMargins(0,0,0,0);

    connect(inputField, &QLineEdit::textEdited, this, &LineEditor::textChanged);

}

QString LineEditor::text() const {
	return inputField->text();
}

void LineEditor::setText(const QString &text){
	inputField->setText(text);
}

void LineEditor::setModelIndex(const QModelIndex &newIndex){
	index = newIndex;
}

void LineEditor::hideButton() const{
	deleteButton->hide();
}

void LineEditor::showButton() const{
	deleteButton->show();
}

void LineEditor::saveCursor(){
    lastSavedCursor = inputField->cursorPosition();
}

void LineEditor::restoreCursor(){
    inputField->setCursorPosition(lastSavedCursor);
}


void LineEditor::registerAsEntry(){
	deleteButton->installEventFilter(this);
	showButton();
}

void LineEditor::registerAsDummy(){
	inputField->installEventFilter(this);
	hideButton();
}


bool LineEditor::eventFilter(QObject *obj, QEvent *ev){
    if(obj == inputField && ev->type() == QEvent::FocusIn){
        emit editorFocusIn();
        return true;
    }

    else if(obj == deleteButton && ev->type() == QEvent::MouseButtonPress){
		emit deleteClicked(index);
		return true;
	}

	return false;

}


