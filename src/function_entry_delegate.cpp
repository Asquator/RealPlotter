#include "line_editor.h"
#include "function_entry_delegate.h"
#include "function_entry.h"
#include "function_list_model.h"


FunctionEntryDelegate::FunctionEntryDelegate(QObject *parent):
    QStyledItemDelegate(parent){}


/**
 * @brief FunctionEntryDelegate::createEditor creates the delegate editor for a row in the function list
 * @param parent parent QWidget
 * @param option style option
 * @param index index in the model
 * @return line editor
 */
QWidget *FunctionEntryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const {
	LineEditor *editor = new LineEditor(parent);
	const FunctionListModel *mdl = static_cast<const FunctionListModel *>(index.model());

	switch(editorType){
		case EditorType::Entry:
			editor->registerAsEntry();
			editor->setModelIndex(index);
			editor->setFocus(Qt::OtherFocusReason);

			connect(editor, SIGNAL(deleteClicked(QPersistentModelIndex)),
					mdl, SLOT(removeLine(QPersistentModelIndex)));

            connect(editor, &LineEditor::textChanged, editor, &LineEditor::saveCursor);
            connect(editor, &LineEditor::textChanged, this, &FunctionEntryDelegate::commitInput);
            connect(editor, &LineEditor::textChanged, editor, &LineEditor::restoreCursor);

			break;

		case EditorType::DummyLast:
			editor->registerAsDummy();
			connect(editor, SIGNAL(editorFocusIn()), mdl, SLOT(addEmptyLine()));

			break;
	}

    return editor;
}


void FunctionEntryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    //casting the editor to a line edit
	LineEditor *edit = static_cast<LineEditor *>(editor);

    //extracting the abstract data
    QVariant data = index.model()->data(index, Qt::EditRole);

    //interpreting the data as a shared pointer
    auto ptr = data.value<QSharedPointer<FunctionEntry>>();
    edit->setText(ptr->getString());
}


void FunctionEntryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	LineEditor *edit = static_cast<LineEditor *>(editor);
    QString input = edit->text();

    model->setData(index, input, Qt::EditRole);
}


void FunctionEntryDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}


void FunctionEntryDelegate::commitInput(){
    emit commitData(qobject_cast<LineEditor *>(sender()));
}


QSize FunctionEntryDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& ind) const {
	return QSize(QStyledItemDelegate::sizeHint(opt, ind).width(), 50);
}


void FunctionEntryDelegate::setEditorType(EditorType type) {
	editorType = type;
}


