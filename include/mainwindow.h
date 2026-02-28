#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QVector>

class QTextEdit;
class QLineEdit;
class QTableWidget;
class QTabWidget;
class QRadioButton;
class QLabel;
class QWidget;
class QCheckBox;
class QComboBox;

struct Entry {
    QString type;
    QDateTime timestamp;
    QString content;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGenAISubmit();
    void onRefSubmit();
    void onNotesSubmit();
    void onEditEntry();
    void onRemoveEntry();
    void onEntryTypeChanged();
    void onCitationFieldsChanged();
    void onConfigChanged();
    void onSaveEntries();
    void onLoadEntries();
    void onExportEntries();
    void onExportMarkdown();

private:
    void addEntryToList(const Entry &entry);
    QColor getColorForType(const QString &type) const;
    void loadSettings();
    void saveSettings();

    QVector<Entry> m_entries;
    QTableWidget *m_listWidget;
    QTabWidget *m_tabWidget;

    QTextEdit *m_genAIPrompt;
    QTextEdit *m_genAIResult;
    QTextEdit *m_genAINotes;

    QRadioButton *m_typeArticle;
    QRadioButton *m_typeBook;
    QRadioButton *m_typeInproceedings;
    QLineEdit *m_citationKey;
    QLineEdit *m_author;
    QLineEdit *m_title;
    
    QWidget *m_journalWidget;
    QLineEdit *m_journal;
    QWidget *m_booktitleWidget;
    QLineEdit *m_booktitle;
    QWidget *m_publisherWidget;
    QLineEdit *m_publisher;
    
    QLineEdit *m_year;
    QLineEdit *m_volume;
    QLineEdit *m_pages;
    QLineEdit *m_doi;
    QLineEdit *m_url;
    QLineEdit *m_date;
    QTextEdit *m_refNotes;

    QTextEdit *m_generalNotes;
    
    // Config widgets
    QCheckBox *m_colorCodeEntries;
};

#endif // MAINWINDOW_H
