#ifndef FUNCTION_ENTRY_DELEGATE_H
#define FUNCTION_ENTRY_DELEGATE_H

#include <QStyledItemDelegate>
#include "line_editor.h"

class FunctionEntryDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

	enum class EditorType{
		Entry,
		DummyLast
	};

    FunctionEntryDelegate(QObject *parent = nullptr);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const override;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;

	QSize sizeHint (const QStyleOptionViewItem&, const QModelIndex&) const  override;

	void setEditorType(EditorType);

public slots:
    void commitInput();

private:
	EditorType editorType;

};
#endif // FUNCTION_ENTRY_DELEGATE_H

