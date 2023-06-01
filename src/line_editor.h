#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QString>
#include <QEvent>
#include <QAbstractListModel>

class LineEditor : public QWidget
{
	Q_OBJECT
public:
	explicit LineEditor(QWidget *parent = nullptr);

    /**
     * @brief text returns text from the line editor
     * @return text
     */
	QString text() const;

    /**
     * @brief setText sets text of the editor
     */
	void setText(const QString &);

    /**
     * @brief setModelIndex remembers a persistent model index
     * @param index index in the model
     */
	void setModelIndex(const QModelIndex &index);

	bool eventFilter(QObject *object, QEvent *event);

public slots:
    /**
     * @brief registerAsDummy registers editor as an empty dummy
     */
	void registerAsDummy();

    /**
     * @brief registerAsEntry registers editor as an expression input entry
     */
	void registerAsEntry();

    /**
     * @brief showButton shows remove button
     */

	void showButton() const;

    /**
     * @brief hideButton hides remove buttom
     */
	void hideButton() const;

    void saveCursor();
    void restoreCursor();

signals:
	void deleteClicked(QPersistentModelIndex);
	void editorFocusIn();
    void textChanged();

private:
    int lastSavedCursor = 0;

	QHBoxLayout *layout;
	QPushButton *deleteButton;
	QLineEdit *inputField;

	QPersistentModelIndex index;

};

#endif // LINE_EDITOR_H
