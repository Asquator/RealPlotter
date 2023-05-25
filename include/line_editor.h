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

	QString text() const;

	void setText(const QString &);
	void setModelIndex(const QModelIndex &index);

	bool eventFilter(QObject *object, QEvent *event);

public slots:
	void registerAsDummy();
	void registerAsEntry();

	void showButton() const;
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
