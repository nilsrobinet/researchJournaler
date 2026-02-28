#include "mainwindow.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QColor>
#include <QComboBox>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QRegularExpression>
#include <QSettings>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("resJourn");
    resize(800, 500);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_tabWidget = new QTabWidget(centralWidget);

    QWidget *genAITab = new QWidget(this);
    QVBoxLayout *genAILayout = new QVBoxLayout(genAITab);

    m_genAIPrompt = new QTextEdit(genAITab);
    m_genAIPrompt->setPlaceholderText("prompt");

    m_genAIResult = new QTextEdit(genAITab);
    m_genAIResult->setPlaceholderText("result");

    m_genAINotes = new QTextEdit(genAITab);
    m_genAINotes->setPlaceholderText("notes");

    QPushButton *genAIButton = new QPushButton("Submit", genAITab);
    connect(genAIButton, &QPushButton::clicked, this, &MainWindow::onGenAISubmit);

    genAILayout->addWidget(m_genAIPrompt);
    genAILayout->addWidget(m_genAIResult);
    genAILayout->addWidget(m_genAINotes);
    genAILayout->addWidget(genAIButton);
    genAILayout->addStretch();

    m_tabWidget->addTab(genAITab, "Gen-AI");

    QWidget *refTab = new QWidget(this);
    QVBoxLayout *refLayout = new QVBoxLayout(refTab);
    QFormLayout *refFormLayout = new QFormLayout();

    // Entry Type Radio Buttons
    QHBoxLayout *typeLayout = new QHBoxLayout();
    m_typeArticle = new QRadioButton("Article", refTab);
    m_typeBook = new QRadioButton("Book", refTab);
    m_typeInproceedings = new QRadioButton("Inproceedings", refTab);
    m_typeMisc = new QRadioButton("Misc", refTab);
    m_typeManual = new QRadioButton("Manual", refTab);
    m_typePatent = new QRadioButton("Patent", refTab);
    m_typeArticle->setChecked(true);
    
    QButtonGroup *typeGroup = new QButtonGroup(refTab);
    typeGroup->addButton(m_typeArticle);
    typeGroup->addButton(m_typeBook);
    typeGroup->addButton(m_typeInproceedings);
    typeGroup->addButton(m_typeMisc);
    typeGroup->addButton(m_typeManual);
    typeGroup->addButton(m_typePatent);
    
    typeLayout->addWidget(m_typeArticle);
    typeLayout->addWidget(m_typeBook);
    typeLayout->addWidget(m_typeInproceedings);
    typeLayout->addWidget(m_typeMisc);
    typeLayout->addWidget(m_typeManual);
    typeLayout->addWidget(m_typePatent);
    typeLayout->addStretch();
    
    connect(m_typeArticle, &QRadioButton::toggled, this, &MainWindow::onEntryTypeChanged);
    connect(m_typeBook, &QRadioButton::toggled, this, &MainWindow::onEntryTypeChanged);
    connect(m_typeInproceedings, &QRadioButton::toggled, this, &MainWindow::onEntryTypeChanged);
    connect(m_typeMisc, &QRadioButton::toggled, this, &MainWindow::onEntryTypeChanged);
    connect(m_typeManual, &QRadioButton::toggled, this, &MainWindow::onEntryTypeChanged);
    connect(m_typePatent, &QRadioButton::toggled, this, &MainWindow::onEntryTypeChanged);

    // Citation Key (read-only, auto-generated)
    m_citationKey = new QLineEdit(refTab);
    m_citationKey->setReadOnly(true);
    m_citationKey->setPlaceholderText("Auto-generated");

    // Common fields
    m_author = new QLineEdit(refTab);
    connect(m_author, &QLineEdit::textChanged, this, &MainWindow::onCitationFieldsChanged);
    
    m_title = new QLineEdit(refTab);
    connect(m_title, &QLineEdit::textChanged, this, &MainWindow::onCitationFieldsChanged);
    
    m_year = new QLineEdit(refTab);
    connect(m_year, &QLineEdit::textChanged, this, &MainWindow::onCitationFieldsChanged);
    
    // Article-specific fields
    m_journalWidget = new QWidget(refTab);
    QHBoxLayout *journalLayout = new QHBoxLayout(m_journalWidget);
    journalLayout->setContentsMargins(0, 0, 0, 0);
    m_journal = new QLineEdit(m_journalWidget);
    journalLayout->addWidget(m_journal);
    
    // Inproceedings-specific fields
    m_booktitleWidget = new QWidget(refTab);
    QHBoxLayout *booktitleLayout = new QHBoxLayout(m_booktitleWidget);
    booktitleLayout->setContentsMargins(0, 0, 0, 0);
    m_booktitle = new QLineEdit(m_booktitleWidget);
    booktitleLayout->addWidget(m_booktitle);
    
    // Book-specific fields
    m_publisherWidget = new QWidget(refTab);
    QHBoxLayout *publisherLayout = new QHBoxLayout(m_publisherWidget);
    publisherLayout->setContentsMargins(0, 0, 0, 0);
    m_publisher = new QLineEdit(m_publisherWidget);
    publisherLayout->addWidget(m_publisher);
    
    // Misc-specific fields
    m_howpublishedWidget = new QWidget(refTab);
    QHBoxLayout *howpublishedLayout = new QHBoxLayout(m_howpublishedWidget);
    howpublishedLayout->setContentsMargins(0, 0, 0, 0);
    m_howpublished = new QLineEdit(m_howpublishedWidget);
    howpublishedLayout->addWidget(m_howpublished);
    
    // Manual-specific fields
    m_organizationWidget = new QWidget(refTab);
    QHBoxLayout *organizationLayout = new QHBoxLayout(m_organizationWidget);
    organizationLayout->setContentsMargins(0, 0, 0, 0);
    m_organization = new QLineEdit(m_organizationWidget);
    organizationLayout->addWidget(m_organization);
    
    m_addressWidget = new QWidget(refTab);
    QHBoxLayout *addressLayout = new QHBoxLayout(m_addressWidget);
    addressLayout->setContentsMargins(0, 0, 0, 0);
    m_address = new QLineEdit(m_addressWidget);
    addressLayout->addWidget(m_address);
    
    m_editionWidget = new QWidget(refTab);
    QHBoxLayout *editionLayout = new QHBoxLayout(m_editionWidget);
    editionLayout->setContentsMargins(0, 0, 0, 0);
    m_edition = new QLineEdit(m_editionWidget);
    editionLayout->addWidget(m_edition);
    
    // Patent-specific fields
    m_numberWidget = new QWidget(refTab);
    QHBoxLayout *numberLayout = new QHBoxLayout(m_numberWidget);
    numberLayout->setContentsMargins(0, 0, 0, 0);
    m_number = new QLineEdit(m_numberWidget);
    numberLayout->addWidget(m_number);
    
    m_nationalityWidget = new QWidget(refTab);
    QHBoxLayout *nationalityLayout = new QHBoxLayout(m_nationalityWidget);
    nationalityLayout->setContentsMargins(0, 0, 0, 0);
    m_nationality = new QLineEdit(m_nationalityWidget);
    nationalityLayout->addWidget(m_nationality);
    
    m_volume = new QLineEdit(refTab);
    m_pages = new QLineEdit(refTab);
    m_doi = new QLineEdit(refTab);
    m_url = new QLineEdit(refTab);
    m_date = new QLineEdit(refTab);
    m_date->setPlaceholderText("YYYY-MM-DD");

    m_refNotes = new QTextEdit(refTab);
    m_refNotes->setPlaceholderText("Notes about this entry...");

    QPushButton *createBibtexButton = new QPushButton("Submit", refTab);
    connect(createBibtexButton, &QPushButton::clicked, this, &MainWindow::onRefSubmit);

    refFormLayout->addRow("Entry Type:", typeLayout);
    refFormLayout->addRow("Citation Key:", m_citationKey);
    refFormLayout->addRow("Author:", m_author);
    refFormLayout->addRow("Title:", m_title);
    refFormLayout->addRow("Journal:", m_journalWidget);
    refFormLayout->addRow("Booktitle:", m_booktitleWidget);
    refFormLayout->addRow("Publisher:", m_publisherWidget);
    refFormLayout->addRow("Howpublished:", m_howpublishedWidget);
    refFormLayout->addRow("Organization:", m_organizationWidget);
    refFormLayout->addRow("Address:", m_addressWidget);
    refFormLayout->addRow("Edition:", m_editionWidget);
    refFormLayout->addRow("Number:", m_numberWidget);
    refFormLayout->addRow("Nationality:", m_nationalityWidget);
    refFormLayout->addRow("Year:", m_year);
    refFormLayout->addRow("Volume:", m_volume);
    refFormLayout->addRow("Pages:", m_pages);
    refFormLayout->addRow("DOI:", m_doi);
    refFormLayout->addRow("URL:", m_url);
    refFormLayout->addRow("Date:", m_date);

    refLayout->addLayout(refFormLayout);
    refLayout->addWidget(m_refNotes);
    refLayout->addWidget(createBibtexButton);
    
    // Initialize field visibility
    onEntryTypeChanged();

    m_tabWidget->addTab(refTab, "Ref");

    QWidget *notesTab = new QWidget(this);
    QVBoxLayout *notesTabLayout = new QVBoxLayout(notesTab);
    m_generalNotes = new QTextEdit(notesTab);
    m_generalNotes->setPlaceholderText("General notes...");
    QPushButton *notesSubmitButton = new QPushButton("Submit", notesTab);
    connect(notesSubmitButton, &QPushButton::clicked, this, &MainWindow::onNotesSubmit);
    notesTabLayout->addWidget(m_generalNotes);
    notesTabLayout->addWidget(notesSubmitButton);

    m_tabWidget->addTab(notesTab, "Notes");

    QWidget *listTab = new QWidget(this);
    QVBoxLayout *listLayout = new QVBoxLayout(listTab);

    m_listWidget = new QTableWidget(listTab);
    m_listWidget->setColumnCount(3);
    m_listWidget->setHorizontalHeaderLabels({"Date/Time", "Entry", "Notes"});
    m_listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listWidget->horizontalHeader()->setStretchLastSection(true);
    m_listWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    m_listWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    m_listWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    m_listWidget->verticalHeader()->setVisible(false);
    m_listWidget->verticalHeader()->setDefaultSectionSize(50);  // Height for multiline date

    QHBoxLayout *listButtonLayout = new QHBoxLayout();
    QPushButton *editButton = new QPushButton("Edit", listTab);
    QPushButton *removeButton = new QPushButton("Remove", listTab);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditEntry);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveEntry);

    listButtonLayout->addWidget(editButton);
    listButtonLayout->addWidget(removeButton);
    
    QHBoxLayout *fileButtonLayout = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton("Save", listTab);
    QPushButton *loadButton = new QPushButton("Load", listTab);
    QPushButton *exportButton = new QPushButton("Export BibTeX", listTab);
    QPushButton *exportMdButton = new QPushButton("Export Markdown", listTab);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveEntries);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadEntries);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExportEntries);
    connect(exportMdButton, &QPushButton::clicked, this, &MainWindow::onExportMarkdown);
    
    fileButtonLayout->addWidget(saveButton);
    fileButtonLayout->addWidget(loadButton);
    fileButtonLayout->addWidget(exportButton);
    fileButtonLayout->addWidget(exportMdButton);

    listLayout->addWidget(m_listWidget);
    listLayout->addLayout(listButtonLayout);
    listLayout->addLayout(fileButtonLayout);

    m_tabWidget->addTab(listTab, "Entries");

    QWidget *configTab = new QWidget(this);
    QVBoxLayout *configLayout = new QVBoxLayout(configTab);
    
    m_colorCodeEntries = new QCheckBox("Color-code entries by type", configTab);
    m_colorCodeEntries->setChecked(true);
    connect(m_colorCodeEntries, &QCheckBox::toggled, this, &MainWindow::onConfigChanged);

    configLayout->addWidget(m_colorCodeEntries);
    configLayout->addStretch();

    m_tabWidget->addTab(configTab, "Config");

    layout->addWidget(m_tabWidget);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    
    // Load saved settings
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::onGenAISubmit()
{
    Entry entry;
    entry.type = "Gen-AI";
    entry.timestamp = QDateTime::currentDateTime();
    
    // Use markers to separate prompt, result, and notes, allowing multiline content
    entry.content = QString("===PROMPT===\n%1\n===RESULT===\n%2\n===NOTES===\n%3")
                        .arg(m_genAIPrompt->toPlainText())
                        .arg(m_genAIResult->toPlainText())
                        .arg(m_genAINotes->toPlainText());
    
    m_entries.append(entry);
    addEntryToList(entry);
    
    m_genAIPrompt->clear();
    m_genAIResult->clear();
    m_genAINotes->clear();
}

void MainWindow::onRefSubmit()
{
    QString entryType = m_typeArticle->isChecked() ? "article" : 
                       m_typeBook->isChecked() ? "book" : 
                       m_typeInproceedings->isChecked() ? "inproceedings" :
                       m_typeMisc->isChecked() ? "misc" :
                       m_typeManual->isChecked() ? "manual" : "patent";
    
    Entry entry;
    entry.type = "Reference";
    entry.timestamp = QDateTime::currentDateTime();
    
    // Store all fields with labels for easy parsing
    QStringList contentParts;
    contentParts << QString("EntryType: %1").arg(entryType);
    contentParts << QString("CitationKey: %1").arg(m_citationKey->text());
    contentParts << QString("Author: %1").arg(m_author->text());
    contentParts << QString("Title: %1").arg(m_title->text());
    
    if (!m_journal->text().isEmpty())
        contentParts << QString("Journal: %1").arg(m_journal->text());
    if (!m_booktitle->text().isEmpty())
        contentParts << QString("Booktitle: %1").arg(m_booktitle->text());
    if (!m_publisher->text().isEmpty())
        contentParts << QString("Publisher: %1").arg(m_publisher->text());
    if (!m_howpublished->text().isEmpty())
        contentParts << QString("Howpublished: %1").arg(m_howpublished->text());
    if (!m_organization->text().isEmpty())
        contentParts << QString("Organization: %1").arg(m_organization->text());
    if (!m_address->text().isEmpty())
        contentParts << QString("Address: %1").arg(m_address->text());
    if (!m_edition->text().isEmpty())
        contentParts << QString("Edition: %1").arg(m_edition->text());
    if (!m_number->text().isEmpty())
        contentParts << QString("Number: %1").arg(m_number->text());
    if (!m_nationality->text().isEmpty())
        contentParts << QString("Nationality: %1").arg(m_nationality->text());
    if (!m_year->text().isEmpty())
        contentParts << QString("Year: %1").arg(m_year->text());
    if (!m_volume->text().isEmpty())
        contentParts << QString("Volume: %1").arg(m_volume->text());
    if (!m_pages->text().isEmpty())
        contentParts << QString("Pages: %1").arg(m_pages->text());
    if (!m_doi->text().isEmpty())
        contentParts << QString("DOI: %1").arg(m_doi->text());
    if (!m_url->text().isEmpty())
        contentParts << QString("URL: %1").arg(m_url->text());
    if (!m_date->text().isEmpty())
        contentParts << QString("Date: %1").arg(m_date->text());
    if (!m_refNotes->toPlainText().isEmpty())
        contentParts << QString("Notes: %1").arg(m_refNotes->toPlainText());
    
    entry.content = contentParts.join("\n");
    
    m_entries.append(entry);
    addEntryToList(entry);
    
    m_typeArticle->setChecked(true);
    m_citationKey->clear();
    m_author->clear();
    m_title->clear();
    m_journal->clear();
    m_booktitle->clear();
    m_publisher->clear();
    m_howpublished->clear();
    m_organization->clear();
    m_address->clear();
    m_edition->clear();
    m_number->clear();
    m_nationality->clear();
    m_year->clear();
    m_volume->clear();
    m_pages->clear();
    m_doi->clear();
    m_url->clear();
    m_date->clear();
    m_refNotes->clear();
}

void MainWindow::onEntryTypeChanged()
{
    bool isArticle = m_typeArticle->isChecked();
    bool isBook = m_typeBook->isChecked();
    bool isInproceedings = m_typeInproceedings->isChecked();
    bool isMisc = m_typeMisc->isChecked();
    bool isManual = m_typeManual->isChecked();
    bool isPatent = m_typePatent->isChecked();
    
    // Article fields
    m_journalWidget->setVisible(isArticle);
    
    // Inproceedings fields
    m_booktitleWidget->setVisible(isInproceedings);
    
    // Book fields
    m_publisherWidget->setVisible(isBook);
    
    // Misc fields
    m_howpublishedWidget->setVisible(isMisc);
    
    // Manual fields
    m_organizationWidget->setVisible(isManual);
    m_addressWidget->setVisible(isManual);
    m_editionWidget->setVisible(isManual);
    
    // Patent fields
    m_numberWidget->setVisible(isPatent);
    m_nationalityWidget->setVisible(isPatent);
}

void MainWindow::onCitationFieldsChanged()
{
    QString author = m_author->text().trimmed();
    QString title = m_title->text().trimmed();
    QString year = m_year->text().trimmed();
    
    if (author.isEmpty() && title.isEmpty() && year.isEmpty()) {
        m_citationKey->clear();
        return;
    }
    
    // Extract last name from author
    QString lastName = author;
    if (author.contains(" ")) {
        QStringList parts = author.split(" ");
        lastName = parts.last();
    }
    lastName = lastName.toLower().remove(QRegularExpression("[^a-z0-9]"));
    
    // Extract first significant word from title (skip common words)
    QStringList skipWords = {"a", "an", "the", "of", "in", "on", "at", "to", "for", "with"};
    QString titleWord;
    if (!title.isEmpty()) {
        QStringList titleParts = title.toLower().split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
        for (const QString &word : titleParts) {
            if (word.length() >= 3 && !skipWords.contains(word)) {
                titleWord = word.left(5);  // Take first 5 chars of significant word
                break;
            }
        }
    }
    
    // Generate citation key: lastname+year+titleword
    QString citationKey = lastName;
    if (!year.isEmpty()) {
        citationKey += year;
    }
    if (!titleWord.isEmpty()) {
        citationKey += titleWord;
    }
    
    m_citationKey->setText(citationKey);
}

void MainWindow::onNotesSubmit()
{
    Entry entry;
    entry.type = "Note";
    entry.timestamp = QDateTime::currentDateTime();
    entry.content = m_generalNotes->toPlainText();
    
    m_entries.append(entry);
    addEntryToList(entry);
    
    m_generalNotes->clear();
}

void MainWindow::addEntryToList(const Entry &entry)
{
    QString entryInfo, notes;
    
    // Create entry info and extract notes based on entry type
    if (entry.type == "Reference") {
        // Extract key fields for entry column
        QString citationKey, author, title;
        QStringList lines = entry.content.split("\n");
        for (const QString &line : lines) {
            if (line.startsWith("CitationKey: ")) citationKey = line.mid(13);
            else if (line.startsWith("Author: ")) author = line.mid(8);
            else if (line.startsWith("Title: ")) title = line.mid(7);
            else if (line.startsWith("Notes: ")) {
                notes = entry.content.mid(entry.content.indexOf("Notes: ") + 7).trimmed();
            }
        }
        entryInfo = QString("[Ref] %1").arg(citationKey);
        if (!author.isEmpty()) {
            entryInfo += QString(" - %1").arg(author.left(25));
            if (author.length() > 25) entryInfo += "...";
        }
        if (!title.isEmpty()) {
            entryInfo += QString(": %1").arg(title.left(40));
            if (title.length() > 40) entryInfo += "...";
        }
    } else if (entry.type == "Gen-AI") {
        // Extract first line of prompt for entry column and notes
        int promptStart = entry.content.indexOf("===PROMPT===");
        int resultStart = entry.content.indexOf("===RESULT===");
        int notesStart = entry.content.indexOf("===NOTES===");
        if (promptStart != -1 && resultStart != -1) {
            promptStart += QString("===PROMPT===\n").length();
            QString prompt = entry.content.mid(promptStart, resultStart - promptStart).trimmed();
            QString firstLine = prompt.split("\n").first();
            
            // Extract notes if present
            if (notesStart != -1) {
                notesStart += QString("===NOTES===\n").length();
                notes = entry.content.mid(notesStart).trimmed();
            }
            entryInfo = QString("[AI] %1").arg(firstLine.left(60));
            if (firstLine.length() > 60 || prompt.contains("\n")) {
                entryInfo += "...";
            }
        } else {
            entryInfo = QString("[AI] %1").arg(entry.content.left(60));
        }
    } else if (entry.type == "Note") {
        // For notes, put first 60 chars in entry, leave notes column empty
        entryInfo = QString("[N] %1").arg(entry.content.left(60));
        if (entry.content.length() > 60) entryInfo += "...";
        notes = "";  // Already in entry column
    }
    
    // Truncate notes if too long
    if (notes.length() > 50) {
        notes = notes.left(50) + "...";
    }
    
    int row = m_listWidget->rowCount();
    m_listWidget->insertRow(row);
    
    // Date column
    QTableWidgetItem *dateItem = new QTableWidgetItem(entry.timestamp.toString("yyyy-MM-dd\nHH:mm"));
    dateItem->setTextAlignment(Qt::AlignCenter);
    
    // Entry column
    QTableWidgetItem *entryItem = new QTableWidgetItem(entryInfo);
    
    // Notes column
    QTableWidgetItem *notesItem = new QTableWidgetItem(notes);
    notesItem->setForeground(QColor(100, 100, 100));  // Gray text for notes
    
    // Apply color-coding if enabled
    if (m_colorCodeEntries->isChecked()) {
        QColor bgColor = getColorForType(entry.type);
        dateItem->setBackground(bgColor);
        entryItem->setBackground(bgColor);
        notesItem->setBackground(bgColor);
    }
    
    m_listWidget->setItem(row, 0, dateItem);
    m_listWidget->setItem(row, 1, entryItem);
    m_listWidget->setItem(row, 2, notesItem);
}

QColor MainWindow::getColorForType(const QString &type) const
{
    if (type == "Gen-AI") {
        return QColor(200, 230, 255);  // Light blue
    } else if (type == "Reference") {
        return QColor(255, 230, 200);  // Light orange
    } else if (type == "Note") {
        return QColor(230, 255, 200);  // Light green
    }
    return QColor(Qt::white);
}

void MainWindow::onEditEntry()
{
    int currentRow = m_listWidget->currentRow();
    if (currentRow < 0 || currentRow >= m_entries.size()) {
        QMessageBox::warning(this, "No Selection", "Please select an entry to edit.");
        return;
    }
    
    const Entry &entry = m_entries[currentRow];
    
    if (entry.type == "Gen-AI") {
        // Parse content back into fields using markers
        QString prompt, result, notes;
        
        int promptStart = entry.content.indexOf("===PROMPT===");
        int resultStart = entry.content.indexOf("===RESULT===");
        int notesStart = entry.content.indexOf("===NOTES===");
        
        if (promptStart != -1 && resultStart != -1) {
            // Extract prompt (between ===PROMPT=== and ===RESULT===)
            promptStart += QString("===PROMPT===\n").length();
            prompt = entry.content.mid(promptStart, resultStart - promptStart).trimmed();
            
            // Extract result (between ===RESULT=== and ===NOTES=== or to end)
            resultStart += QString("===RESULT===\n").length();
            if (notesStart != -1) {
                result = entry.content.mid(resultStart, notesStart - resultStart).trimmed();
                
                // Extract notes (after ===NOTES===)
                notesStart += QString("===NOTES===\n").length();
                notes = entry.content.mid(notesStart).trimmed();
            } else {
                result = entry.content.mid(resultStart).trimmed();
            }
            
            m_genAIPrompt->setPlainText(prompt);
            m_genAIResult->setPlainText(result);
            m_genAINotes->setPlainText(notes);
        }
        m_tabWidget->setCurrentIndex(0);
    } else if (entry.type == "Reference") {
        // Parse reference fields - each field is on its own line with "FieldName: value" format
        QStringList lines = entry.content.split("\n");
        
        // Clear all fields first
        m_typeArticle->setChecked(true);
        m_citationKey->clear();
        m_author->clear();
        m_title->clear();
        m_journal->clear();
        m_booktitle->clear();
        m_publisher->clear();
        m_howpublished->clear();
        m_organization->clear();
        m_address->clear();
        m_edition->clear();
        m_number->clear();
        m_nationality->clear();
        m_year->clear();
        m_volume->clear();
        m_pages->clear();
        m_doi->clear();
        m_url->clear();
        m_date->clear();
        m_refNotes->clear();
        
        for (const QString &line : lines) {
            if (line.startsWith("EntryType: ")) {
                QString entryType = line.mid(11);
                if (entryType == "article") m_typeArticle->setChecked(true);
                else if (entryType == "book") m_typeBook->setChecked(true);
                else if (entryType == "inproceedings") m_typeInproceedings->setChecked(true);
                else if (entryType == "misc") m_typeMisc->setChecked(true);
                else if (entryType == "manual") m_typeManual->setChecked(true);
                else if (entryType == "patent") m_typePatent->setChecked(true);
            }
            else if (line.startsWith("CitationKey: ")) m_citationKey->setText(line.mid(13));
            else if (line.startsWith("Author: ")) m_author->setText(line.mid(8));
            else if (line.startsWith("Title: ")) m_title->setText(line.mid(7));
            else if (line.startsWith("Journal: ")) m_journal->setText(line.mid(9));
            else if (line.startsWith("Booktitle: ")) m_booktitle->setText(line.mid(11));
            else if (line.startsWith("Publisher: ")) m_publisher->setText(line.mid(11));
            else if (line.startsWith("Howpublished: ")) m_howpublished->setText(line.mid(14));
            else if (line.startsWith("Organization: ")) m_organization->setText(line.mid(14));
            else if (line.startsWith("Address: ")) m_address->setText(line.mid(9));
            else if (line.startsWith("Edition: ")) m_edition->setText(line.mid(9));
            else if (line.startsWith("Number: ")) m_number->setText(line.mid(8));
            else if (line.startsWith("Nationality: ")) m_nationality->setText(line.mid(13));
            else if (line.startsWith("Year: ")) m_year->setText(line.mid(6));
            else if (line.startsWith("Volume: ")) m_volume->setText(line.mid(8));
            else if (line.startsWith("Pages: ")) m_pages->setText(line.mid(7));
            else if (line.startsWith("DOI: ")) m_doi->setText(line.mid(5));
            else if (line.startsWith("URL: ")) m_url->setText(line.mid(5));
            else if (line.startsWith("Date: ")) m_date->setText(line.mid(6));
            else if (line.startsWith("Notes: ")) {
                // Notes might be multi-line, so get everything after "Notes: "
                int notesStart = entry.content.indexOf("Notes: ");
                if (notesStart != -1) {
                    m_refNotes->setPlainText(entry.content.mid(notesStart + 7));
                    break;  // Notes is the last field
                }
            }
        }
        m_tabWidget->setCurrentIndex(1);
    } else if (entry.type == "Note") {
        m_generalNotes->setPlainText(entry.content);
        m_tabWidget->setCurrentIndex(2);
    }
    
    // Remove the old entry
    m_entries.removeAt(currentRow);
    m_listWidget->removeRow(currentRow);
}

void MainWindow::onRemoveEntry()
{
    int currentRow = m_listWidget->currentRow();
    if (currentRow < 0 || currentRow >= m_entries.size()) {
        QMessageBox::warning(this, "No Selection", "Please select an entry to remove.");
        return;
    }
    
    m_entries.removeAt(currentRow);
    m_listWidget->removeRow(currentRow);
}

void MainWindow::loadSettings()
{
    QSettings settings("resJourn", "resJourn");
    
    m_colorCodeEntries->setChecked(settings.value("colorCodeEntries", true).toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings("resJourn", "resJourn");
    
    settings.setValue("colorCodeEntries", m_colorCodeEntries->isChecked());
}

void MainWindow::onConfigChanged()
{
    saveSettings();
    
    // Refresh list display if color-coding setting changed
    m_listWidget->setRowCount(0);
    for (const Entry &entry : m_entries) {
        addEntryToList(entry);
    }
}

void MainWindow::onSaveEntries()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                     "Save Entries",
                                                     "entries.json",
                                                     "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }
    
    QJsonArray entriesArray;
    for (const Entry &entry : m_entries) {
        QJsonObject entryObj;
        entryObj["type"] = entry.type;
        entryObj["timestamp"] = entry.timestamp.toString(Qt::ISODate);
        entryObj["content"] = entry.content;
        entriesArray.append(entryObj);
    }
    
    QJsonDocument doc(entriesArray);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return;
    }
    
    file.write(doc.toJson());
    file.close();
    QMessageBox::information(this, "Success", QString("Saved %1 entries.").arg(m_entries.size()));
}

void MainWindow::onLoadEntries()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                     "Load Entries",
                                                     "",
                                                     "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading.");
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        QMessageBox::warning(this, "Error", "Invalid file format.");
        return;
    }
    
    m_entries.clear();
    m_listWidget->setRowCount(0);
    
    QJsonArray entriesArray = doc.array();
    for (const QJsonValue &value : entriesArray) {
        QJsonObject entryObj = value.toObject();
        Entry entry;
        entry.type = entryObj["type"].toString();
        entry.timestamp = QDateTime::fromString(entryObj["timestamp"].toString(), Qt::ISODate);
        entry.content = entryObj["content"].toString();
        m_entries.append(entry);
        addEntryToList(entry);
    }
    
    QMessageBox::information(this, "Success", QString("Loaded %1 entries.").arg(m_entries.size()));
}

void MainWindow::onExportEntries()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                     "Export References as BibTeX",
                                                     "references.bib",
                                                     "BibTeX Files (*.bib)");
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return;
    }
    
    QTextStream out(&file);
    int exportCount = 0;
    
    for (const Entry &entry : m_entries) {
        if (entry.type != "Reference") {
            continue;  // Only export Reference entries
        }
        
        // Parse reference fields
        QStringList lines = entry.content.split("\n");
        QString entryType, citationKey, author, title, journal, booktitle, publisher;
        QString howpublished, organization, address, edition, number, nationality;
        QString year, volume, pages, doi, url, date, notes;
        
        for (const QString &line : lines) {
            if (line.startsWith("EntryType: ")) entryType = line.mid(11).trimmed();
            else if (line.startsWith("CitationKey: ")) citationKey = line.mid(13).trimmed();
            else if (line.startsWith("Author: ")) author = line.mid(8).trimmed();
            else if (line.startsWith("Title: ")) title = line.mid(7).trimmed();
            else if (line.startsWith("Journal: ")) journal = line.mid(9).trimmed();
            else if (line.startsWith("Booktitle: ")) booktitle = line.mid(11).trimmed();
            else if (line.startsWith("Publisher: ")) publisher = line.mid(11).trimmed();
            else if (line.startsWith("Howpublished: ")) howpublished = line.mid(14).trimmed();
            else if (line.startsWith("Organization: ")) organization = line.mid(14).trimmed();
            else if (line.startsWith("Address: ")) address = line.mid(9).trimmed();
            else if (line.startsWith("Edition: ")) edition = line.mid(9).trimmed();
            else if (line.startsWith("Number: ")) number = line.mid(8).trimmed();
            else if (line.startsWith("Nationality: ")) nationality = line.mid(13).trimmed();
            else if (line.startsWith("Year: ")) year = line.mid(6).trimmed();
            else if (line.startsWith("Volume: ")) volume = line.mid(8).trimmed();
            else if (line.startsWith("Pages: ")) pages = line.mid(7).trimmed();
            else if (line.startsWith("DOI: ")) doi = line.mid(5).trimmed();
            else if (line.startsWith("URL: ")) url = line.mid(5).trimmed();
            else if (line.startsWith("Date: ")) date = line.mid(6).trimmed();
            else if (line.startsWith("Notes: ")) notes = line.mid(7).trimmed();
        }
        
        if (citationKey.isEmpty()) {
            continue;  // Skip entries without citation key
        }
        
        // Write BibTeX entry
        out << "@" << entryType << "{" << citationKey << ",\n";
        if (!author.isEmpty()) out << "  author = {" << author << "},\n";
        if (!title.isEmpty()) out << "  title = {" << title << "},\n";
        if (!journal.isEmpty()) out << "  journal = {" << journal << "},\n";
        if (!booktitle.isEmpty()) out << "  booktitle = {" << booktitle << "},\n";
        if (!publisher.isEmpty()) out << "  publisher = {" << publisher << "},\n";
        if (!howpublished.isEmpty()) out << "  howpublished = {" << howpublished << "},\n";
        if (!organization.isEmpty()) out << "  organization = {" << organization << "},\n";
        if (!address.isEmpty()) out << "  address = {" << address << "},\n";
        if (!edition.isEmpty()) out << "  edition = {" << edition << "},\n";
        if (!number.isEmpty()) out << "  number = {" << number << "},\n";
        if (!nationality.isEmpty()) out << "  nationality = {" << nationality << "},\n";
        if (!year.isEmpty()) out << "  year = {" << year << "},\n";
        if (!volume.isEmpty()) out << "  volume = {" << volume << "},\n";
        if (!pages.isEmpty()) out << "  pages = {" << pages << "},\n";
        if (!doi.isEmpty()) out << "  doi = {" << doi << "},\n";
        if (!url.isEmpty()) out << "  url = {" << url << "},\n";
        if (!date.isEmpty()) out << "  date = {" << date << "},\n";
        if (!notes.isEmpty()) out << "  note = {" << notes << "},\n";
        out << "}\n\n";
        
        exportCount++;
    }
    
    file.close();
    QMessageBox::information(this, "Success", QString("Exported %1 reference(s) to BibTeX.").arg(exportCount));
}

void MainWindow::onExportMarkdown()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                     "Export Entries as Markdown",
                                                     "entries.md",
                                                     "Markdown Files (*.md)");
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return;
    }
    
    QTextStream out(&file);
    out << "# resJourn Entries\n\n";
    
    for (const Entry &entry : m_entries) {
        out << "## " << entry.timestamp.toString("yyyy-MM-dd HH:mm") << " - " << entry.type << "\n\n";
        
        if (entry.type == "Gen-AI") {
            // Parse Gen-AI content
            QString prompt, result, notes;
            int promptStart = entry.content.indexOf("===PROMPT===");
            int resultStart = entry.content.indexOf("===RESULT===");
            int notesStart = entry.content.indexOf("===NOTES===");
            
            if (promptStart != -1 && resultStart != -1) {
                promptStart += QString("===PROMPT===\n").length();
                prompt = entry.content.mid(promptStart, resultStart - promptStart).trimmed();
                
                resultStart += QString("===RESULT===\n").length();
                if (notesStart != -1) {
                    result = entry.content.mid(resultStart, notesStart - resultStart).trimmed();
                    notesStart += QString("===NOTES===\n").length();
                    notes = entry.content.mid(notesStart).trimmed();
                } else {
                    result = entry.content.mid(resultStart).trimmed();
                }
            }
            
            out << "**Prompt:**\n\n";
            out << prompt.replace("\t", "    ") << "\n\n";
            out << "**Result:**\n\n";
            out << result.replace("\t", "    ") << "\n\n";
            if (!notes.isEmpty()) {
                out << "**Notes:**\n\n";
                QString notesClean = notes;
                out << notesClean.replace("\t", "    ") << "\n\n";
            }
        } else if (entry.type == "Reference") {
            // Parse reference content
            out << "**Reference Entry:**\n\n";
            QStringList lines = entry.content.split("\n");
            for (const QString &line : lines) {
                if (line.trimmed().isEmpty()) continue;
                QString lineClean = line;
                out << "- " << lineClean.replace("\t", "    ") << "\n";
            }
            out << "\n";
        } else if (entry.type == "Note") {
            QString noteContent = entry.content;
            out << noteContent.replace("\t", "    ") << "\n\n";
        }
        
        out << "---\n\n";
    }
    
    file.close();
    QMessageBox::information(this, "Success", QString("Exported %1 entries to Markdown.").arg(m_entries.size()));
}
