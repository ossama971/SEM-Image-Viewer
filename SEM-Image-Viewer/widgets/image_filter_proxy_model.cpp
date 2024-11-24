#include "image_filter_proxy_model.h"
#include "../core/data/image_repository.h"
#include <QFileSystemModel>
#include <filesystem>
#include <regex>
#include <boost/algorithm/string.hpp>

ImageFilterProxyModel::ImageFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

void ImageFilterProxyModel::setRootDir(QDir root_dir) {
    _rootDir = root_dir;
}

void ImageFilterProxyModel::setImageFilter(QString image_path) {
    _imageFilter = image_path;

    invalidateFilter();
}

void ImageFilterProxyModel::setSearchText(QString search_text) {
    _searchText = search_text;

    invalidateFilter();
}

bool is_subpath(std::filesystem::path fileName, std::filesystem::path base) {
    const auto mismatch_pair = std::mismatch(fileName.begin(), fileName.end(), base.begin(), base.end());
    return mismatch_pair.second == base.end();
}

bool ImageFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString qFileName = sourceModel()->data(index, QFileSystemModel::FilePathRole).toString();
    std::filesystem::path fileName = qFileName.toStdString();
    std::filesystem::path rootPath(_rootDir.path().toStdString());

    // Pass parent pathes
    if (is_subpath(rootPath, fileName))
        return true;

    // Filter children pathes
    if (!is_subpath(fileName, rootPath))
        return false;

    if (fileName.has_filename() && fileName.has_extension())
    {
        QString file(qFileName);

        // get file name only
        qsizetype last_dash = file.lastIndexOf("/");
        if (last_dash == -1)
            last_dash = file.lastIndexOf("\\");
        if (last_dash != -1)
            file = file.remove(0, last_dash + 1);

        const std::regex filter(IMAGE_FILE_REGEX);
        std::smatch what;

        // Filter file type
        std::string filenamestd = file.toStdString();
        boost::to_lower(filenamestd);
        if (!std::regex_search(filenamestd, what, filter))
            return false;

        if (_imageFilter.length() && qFileName.compare(_imageFilter, Qt::CaseInsensitive))
            return false;

        // Filter search text
        if (_searchText.length() && !file.contains(_searchText, Qt::CaseInsensitive))
            return false;
    }

    // Filter out children pathes, on having a specific image only to be shown.
    else if (_imageFilter.length())
    {
        QString qImageFilterPath(_imageFilter);

        // get file name only
        qsizetype last_dash = _imageFilter.lastIndexOf("/");
        if (last_dash == -1)
            last_dash = _imageFilter.lastIndexOf("\\");
        if (last_dash != -1)
            qImageFilterPath.truncate(last_dash);

        std::filesystem::path imageFilterPath(qImageFilterPath.toStdString());
        if (is_subpath(fileName, imageFilterPath))
            return false;
    }

    return true;
}
