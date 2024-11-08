#ifndef MESSAGEDATAMODEL_H
#define MESSAGEDATAMODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QString>

#include <memory>

#include "../core/data/IMessage.h"

class MessageDataModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum LogRoles {
        LoggedItemRole = Qt::UserRole + 1,
    };

    explicit MessageDataModel(QObject *parent = nullptr)
        : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void addLogEntry(std::unique_ptr<IMessage> record);
    void clearLogEntries();
    void setFilterText(const QString &text);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    void applyFilter();
    bool matchesFilter(const IMessage &record) const;

private:
    QString filterText;
    std::vector<std::unique_ptr<IMessage>> logEntries;
    std::vector<std::size_t> filteredLogEntriesIndexes;
};

#endif // MESSAGEDATAMODEL_H
