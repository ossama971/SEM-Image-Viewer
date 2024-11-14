#include "FileBrowserWidget.h"
#include "FileIconHider.h"
#include "ImageFilterProxyModel.h"
#include "ImageDialog.h"
#include "../core/engines/Workspace.h"
#include <QVBoxLayout>

FileBrowserWidget::FileBrowserWidget(QWidget *parent)
    : QWidget{parent}, _rootDir(""), _imageRepo(&Workspace::Instance().getActiveSession().getImageRepo()), viewController(nullptr)
{
    Initialize();

    connect(_imageRepo, &ImageRepository::onDirectoryChanged, this, &FileBrowserWidget::onDirectoryChanged);
}

FileBrowserWidget::~FileBrowserWidget()
{

}

void FileBrowserWidget::Initialize() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QPushButton* newFolderBtn = new QPushButton();
    newFolderBtn->setIcon(QIcon(":/icons/new-folder-dark.svg"));
    newFolderBtn->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    background-color: #424242;"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #353535;"
        "    border-style: inset;"
        "}"
        );
    newFolderBtn->setIconSize(QSize(24, 24));
    newFolderBtn->resize(24, 24);

    QPushButton* newFileBtn = new QPushButton();
    newFileBtn->setIcon(QIcon(":/icons/new-file-dark.svg"));
    newFileBtn->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    background-color: #424242;"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #353535;"
        "    border-style: inset;"
        "}"
        );
    newFileBtn->setIconSize(QSize(24, 24));
    newFileBtn->resize(24, 24);

    QPushButton* collapseBtn = new QPushButton();
    collapseBtn->setIcon(QIcon(":/icons/nav-arrow-dark.svg"));
    collapseBtn->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    background-color: #424242;"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #353535;"
        "    border-style: inset;"
        "}"
        );
    collapseBtn->setIconSize(QSize(24, 24));
    collapseBtn->resize(24, 24);

    connect(newFolderBtn, &QPushButton::clicked, this, &FileBrowserWidget::onOpenFolder);
    connect(newFileBtn, &QPushButton::clicked, this, &FileBrowserWidget::onOpenFile);
    connect(collapseBtn, &QPushButton::clicked, this, &FileBrowserWidget::onCollapse);


    QWidget *searchBoxHolder = new QWidget();
    searchBoxHolder->setStyleSheet(
        "    border: 2px solid darkgray;"
        "    border-radius: 10px;"
        "    padding: 3px 5px 5px 5px;"
        );
    searchBoxHolder->move(9, 47);
    searchBoxHolder->resize(239, 35);

    searchBox = new QLineEdit(searchBoxHolder);
    searchBox->setPlaceholderText("File Name");
    searchBox->setStyleSheet(
        "QLineEdit {"
        "	 color: #CCCCCC;"
        "    border: 0;"
        "    padding: 3px 5px 5px 5px;"
        "    selection-background-color: darkgray;"
        "}"
        );

    QPushButton* searchButton = new QPushButton(searchBoxHolder);
    searchButton->setIcon(QIcon(":/icons/search-dark.svg"));
    searchButton->setStyleSheet(
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    background-color: #424242;"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #353535;"
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

    treeView->setStyleSheet(
        "QTreeView {"
        "   background-color: #2C2C2C; "
        "   color: white; "
        "   border: 0; "
        "}"
        "QTreeView::branch:has-siblings:!adjoins-item {"
        "    border-image: url(:/icons/vline-dark.png) 0;"
        "}"
        "QTreeView::branch:has-siblings:adjoins-item {"
        "    border-image: url(:/icons/vline-dark.png) 0;"
        "}"
        "QTreeView::branch:!has-children:!has-siblings:adjoins-item {"
        "    border-image: url(:/icons/vline-dark.png) 0;"
        "}"
        "QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings {"
        "    border-image: none;"
        "    image: url(:/icons/arrow-right-dark.svg);"
        "}"
        ""
        "QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings  {"
        "    border-image: none;"
        "    image: url(:/icons/arrow-down-dark.svg);"
        "}"
        );

    treeView->setHeaderHidden(true);
    treeView->setModel(proxyModel);

    treeView->setColumnHidden(1, true);
    treeView->setColumnHidden(2, true);
    treeView->setColumnHidden(3, true);

    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onTreeClick(QModelIndex)));


    QFrame* separator = new QFrame();
    separator->resize(searchBoxHolder->geometry().width() + 20, 1);
    separator->setFixedHeight(1);
    separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //separator->setStyleSheet(QString("background-color: white;"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(newFolderBtn);
    buttonsLayout->addWidget(newFileBtn);
    buttonsLayout->addWidget(collapseBtn);
    buttonsLayout->setContentsMargins(0, 13, 0, 0);
    buttonsLayout->setSpacing(0);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    //searchLayout->addWidget(searchBoxHolder);
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(searchButton);
    searchLayout->setSpacing(0);

    searchBoxHolder->setLayout(searchLayout);

    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(separator);
    mainLayout->addWidget(searchBoxHolder);
    mainLayout->addWidget(treeView);

    setLayout(mainLayout);
}

void FileBrowserWidget::setViewController(WidgetViewController* widgetViewController) {
    viewController = widgetViewController;
}

void FileBrowserWidget::setRoot(QString path) {
    _rootDir = path;

    model->setRootPath(_rootDir.path());
    proxyModel->setRootDir(_rootDir);

    std::filesystem::path baseDir(_rootDir.path().toStdString());
    treeView->setRootIndex(proxyModel->mapFromSource(model->index(QString(baseDir.parent_path().string().c_str()))));
}

void FileBrowserWidget::onOpenFolder() {
    ImageDialog::openFolder(_imageRepo, this);
}

void FileBrowserWidget::onOpenFile() {
    ImageDialog::openFile(_imageRepo, this);
}

void FileBrowserWidget::onCollapse() {
    viewController->showLeftBar(false);
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

void FileBrowserWidget::onDirectoryChanged(const std::string newDir, std::vector<Image>& newImages, bool image_load) {
    setRoot(QString(newDir.c_str()));

    if (!image_load)
        proxyModel->setImageFilter("");
    else
        proxyModel->setImageFilter(!newImages.empty() ? QString::fromStdString(newImages.front().getPath().string()) : "");
}
