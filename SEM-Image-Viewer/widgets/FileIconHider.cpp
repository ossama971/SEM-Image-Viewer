#include "FileIconHider.h"

FileIconHider::FileIconHider() : QAbstractFileIconProvider() {
}

FileIconHider::~FileIconHider() {
}

QIcon FileIconHider::icon(IconType) const {
    return QIcon();
}

QIcon FileIconHider::icon(const QFileInfo &) const {
    return QIcon();
}
