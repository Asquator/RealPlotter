#include "line_editor.h"
#include "function_entry_delegate.h"
#include "function_entry.h"
#include "function_list_model.h"


FunctionEntryDelegate::FunctionEntryDelegate(QObject *parent):
    QStyledItemDelegate(parent){}



QWidget *FunctionEntryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const {
	LineEditor *editor = new LineEditor(parent);
	const FunctionListModel *mdl = static_cast<const FunctionListModel *>(index.model());

	switch(editorType){
		case EditorType::Entry:
			editor->registerAsEntry();
			editor->setModelIndex(index);

            //focusing on new entries
			editor->setFocus(Qt::OtherFocusReason);

			connect(editor, SIGNAL(deleteClicked(QPersistentModelIndex)),
                    mdl, SLOT(removeRow(QPersistentModelIndex)));

            //save cursor
            connect(editor, &LineEditor::textChanged, editor, &LineEditor::saveCursor);

            //save to model when text changed
            connect(editor, &LineEditor::textChanged, this, &FunctionEntryDelegate::commitInput);

            //restore cursor to the correct position
            connect(editor, &LineEditor::textChanged, editor, &LineEditor::restoreCursor);

			break;

		case EditorType::DummyLast:
			editor->registerAsDummy();

            //add empty line on click
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

/*
 * commits input to the model
 */
void FunctionEntryDelegate::commitInput(){
    emit commitData(qobject_cast<LineEditor *>(sender()));
}


QSize FunctionEntryDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& ind) const {
	return QSize(QStyledItemDelegate::sizeHint(opt, ind).width(), 50);
}


void FunctionEntryDelegate::setEditorType(EditorType type) {
	editorType = type;
}


