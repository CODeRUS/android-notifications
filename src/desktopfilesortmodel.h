#ifndef DESKTOPFILESORTMODEL_H
#define DESKTOPFILESORTMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlParserStatus>
#include <QStringList>
#include "desktopfilemodel.h"

class DesktopFileSortModel : public QSortFilterProxyModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit DesktopFileSortModel(QObject *parent = 0);

    void componentComplete();
    void classBegin();

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    int count();

    Q_PROPERTY(QStringList filterShortcuts READ filterShortcuts WRITE setFilterShortcuts NOTIFY filterShortcutsChanged)
    QStringList filterShortcuts() const;
    void setFilterShortcuts(const QStringList &newFilter);

    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    QString filter() const;
    void setFilter(const QString &newFilter);

    Q_INVOKABLE QVariantMap get(int itemIndex);

    Q_INVOKABLE void fillData();

private:
    DesktopFileModel *_fileModel;
    QStringList _filterShortcuts;
    QRegExp _filterString;

signals:
    void filterShortcutsChanged();
    void filterChanged();
    void dataFillEnd();

    void countChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

};

#endif // DESKTOPFILESORTMODEL_H
