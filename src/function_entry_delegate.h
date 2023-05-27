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

    /**
     * @brief createEditor creates the delegate editor for a row in the function list
     * @param parent parent QWidget
     * @param option style option
     * @param index index in the model
     * @return line editor
     */
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;


    /**
     * @brief setEditorData sets editor data from the model
     * @param editor entry line editor
     * @param index index in the model
     */
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    /**
     * @brief setModelData save data to the model
     * @param editor entry line editor
     * @param model function list model
     * @param index index in the model
     */
	void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const override;

    /**
     * @brief updateEditorGeometry
     * @param editor
     * @param option
     * @param index
     */
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;

	QSize sizeHint (const QStyleOptionViewItem&, const QModelIndex&) const  override;

	void setEditorType(EditorType);

private slots:
    void commitInput();

private:
	EditorType editorType;

};
#endif // FUNCTION_ENTRY_DELEGATE_H

