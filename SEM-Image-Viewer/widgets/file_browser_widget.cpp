#include "file_browser_widget.h"
#include "file_icon_hider.h"
#include "image_filter_proxy_model.h"
#include "image_dialog.h"
#include "../core/engines/workspace.h"
#include <QVBoxLayout>
#include <QPushButton>

FileBrowserWidget::FileBrowserWidget(QWidget *parent)
    : QWidget{parent}, _rootDir(""), _imageRepo(&Workspace::Instance()->getActiveSession().getImageRepo())
{
    imageDialog = new ImageDialog(this);

    Initialize();

    connect(_imageRepo, &ImageRepository::onDirectoryChanged, this, &FileBrowserWidget::onDirectoryChanged);
}

FileBrowserWidget::~FileBrowserWidget()
{

}

void FileBrowserWidget::Initialize() {
    QPushButton* newFolderBtn = new QPushButton();
    newFolderBtn->setObjectName("newFolderBtn");
    newFolderBtn->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    border-style: inset;"
        "}"
        );
    newFolderBtn->setIconSize(QSize(21, 21));
    newFolderBtn->resize(21, 21);
    newFolderBtn->setToolTip("Open Folder");

    QPushButton* newFileBtn = new QPushButton();
    newFileBtn->setObjectName("newFileBtn");
    newFileBtn->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    border-style: inset;"
        "}"
        );
    newFileBtn->setIconSize(QSize(21, 21));
    newFileBtn->resize(21, 21);
    newFileBtn->setToolTip("Open File");

    QPushButton* collapseBtn = new QPushButton();
    collapseBtn->setObjectName("collapseBtn");
    collapseBtn->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    border-style: inset;"
        "}"
        );
    collapseBtn->setIconSize(QSize(21, 21));
    collapseBtn->resize(21, 21);

    connect(newFolderBtn, &QPushButton::clicked, this, &FileBrowserWidget::onOpenFolder);
    connect(newFileBtn, &QPushButton::clicked, this, &FileBrowserWidget::onOpenFile);
    connect(collapseBtn, &QPushButton::clicked, this, &FileBrowserWidget::onCollapse);


    QWidget *searchBoxHolder = new QWidget();
    searchBoxHolder->setObjectName("searchBoxHolder");

    searchBoxHolder->move(9, 47);
    searchBoxHolder->resize(239, 35);

    searchBoxHolder->setFixedHeight(40);
    searchBox = new QLineEdit(searchBoxHolder);
    searchBox->setPlaceholderText("File Name");

    QPushButton* searchButton = new QPushButton(searchBoxHolder);
    searchButton->setToolTip("Search");
    searchButton->setObjectName("searchButton");
    searchButton->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    border-style: inset;"
        "}"
        );
    //searchButton->setIconSize(QSize(22, 22));

    connect(searchButton, &QPushButton::clicked, this, &FileBrowserWidget::onSearch);


    treeView = new QTreeView();
    model = new QFileSystemModel;
    iconProvider = new FileIconHider();

    model->setIconProvider(iconProvider);

    proxyModel = new ImageFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    treeView->setHeaderHidden(true);
    treeView->setModel(proxyModel);

    treeView->setColumnHidden(1, true);
    treeView->setColumnHidden(2, true);
    treeView->setColumnHidden(3, true);

    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onTreeClick(QModelIndex)));


    QFrame* separator = new QFrame(this);
    separator->resize(geometry().width(), 5);
    separator->setFixedHeight(5);
    separator->move(0, 30);
    separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    separator->setObjectName("fileBrowserSeparator");

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    //buttonsLayout->addSpacing(50);
    buttonsLayout->addWidget(newFolderBtn);
    buttonsLayout->addSpacing(5);
    buttonsLayout->addWidget(newFileBtn);
    buttonsLayout->addSpacing(5);
    buttonsLayout->addWidget(collapseBtn);
    //buttonsLayout->addSpacing(50);
    buttonsLayout->setSpacing(0);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    //searchLayout->addWidget(searchBoxHolder);
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(searchButton);
    searchLayout->setSpacing(0);

    searchBoxHolder->setLayout(searchLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(searchBoxHolder);
    mainLayout->addWidget(treeView);

    mainLayout->setContentsMargins(9, 3, 9, 9);
    mainLayout->setAlignment(Qt::AlignTop);

    setLayout(mainLayout);
}

void FileBrowserWidget::setRoot(const QString &path) {
    _rootDir = path;

    model->setRootPath(_rootDir.path());
    proxyModel->setRootDir(_rootDir);

    std::filesystem::path baseDir(_rootDir.path().toStdString());
    treeView->setRootIndex(proxyModel->mapFromSource(model->index(QString(baseDir.parent_path().string().c_str()))));
}

void FileBrowserWidget::onOpenFolder() {
    imageDialog->openFolder(_imageRepo, this);
     _imageRepo->selectImage("");
}

void FileBrowserWidget::onOpenFile() {
    imageDialog->openFile(_imageRepo, this);
}

void FileBrowserWidget::onCollapse() {
    emit collapse();
}

void FileBrowserWidget::onSearch() {
    proxyModel->setSearchText(searchBox->text());
}

void FileBrowserWidget::onTreeClick(const QModelIndex &index) {
    QString qFileName = index.data(QFileSystemModel::FilePathRole).toString();
    std::filesystem::path fileName(qFileName.toStdString());

    if (!fileName.has_filename() || !fileName.has_extension())
        return;

    _imageRepo->selectImage(fileName.string());
}

void FileBrowserWidget::onDirectoryChanged(const std::string newDir, std::vector<Image*> newImages, bool image_load) {
    setRoot(QString(newDir.c_str()));

    if (!image_load)
        proxyModel->setImageFilter("");
    else{
        proxyModel->setImageFilter(!newImages.empty() ? QString::fromStdString(newImages.front()->getPath().string()) : "");

    }
}
