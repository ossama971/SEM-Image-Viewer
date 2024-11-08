#include "MessageDataModel.h"

int MessageDataModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return filteredLogEntriesIndexes.size();
}

QVariant MessageDataModel::data(const QModelIndex &index, int role) const {
  // XXX: make sure that this one is right
  if (!index.isValid() || index.row() < 0 || index.row() >= filteredLogEntriesIndexes.size()) {
    return QVariant();
  }

  const IMessage &record = *logEntries[filteredLogEntriesIndexes[index.row()]];

  switch (role) {
  case LoggedItemRole:
    return QString::fromStdString(record.asString());
  case Qt::DisplayRole:
    return QString::fromStdString(record.asString());
  default:
    return QVariant();
  }
}

void MessageDataModel::addLogEntry(std::unique_ptr<IMessage> record) {
  beginInsertRows(QModelIndex(), logEntries.size(), logEntries.size());
  logEntries.push_back(std::move(record));
  endInsertRows();
  applyFilter();
}

void MessageDataModel::setFilterText(const QString &text) {
    filterText = text;
    applyFilter();
}

QHash<int, QByteArray> MessageDataModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[LoggedItemRole] = "loggedItem";
  return roles;
}

void MessageDataModel::applyFilter() {
  beginResetModel();

  filteredLogEntriesIndexes.clear();

  for (std::size_t i = 0; i < logEntries.size(); ++i) {
    if (matchesFilter(*logEntries[i])) {
      filteredLogEntriesIndexes.push_back(i);
    }
  }

  endResetModel();
}

bool MessageDataModel::matchesFilter(const IMessage &record) const {
    bool matches = QString::fromStdString(record.asString()).contains(filterText.trimmed(), Qt::CaseInsensitive);
    return matches;
}
