#include "desktopfilesortmodel.h"

#include <QDebug>

DesktopFileSortModel::DesktopFileSortModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void DesktopFileSortModel::componentComplete()
{
    setSortRole(DesktopFileModel::NameRole);
    setSortLocaleAware(true);
    setSortCaseSensitivity(Qt::CaseInsensitive);

    setFilterRole(DesktopFileModel::PathRole);
    setFilterCaseSensitivity(Qt::CaseSensitive);

    _fileModel = new DesktopFileModel(this);
    QObject::connect(_fileModel, SIGNAL(dataFillEnd()), this, SIGNAL(dataFillEnd()));
    setSourceModel(_fileModel);

    sort(0);
}

void DesktopFileSortModel::classBegin()
{
}

int DesktopFileSortModel::count()
{
    return rowCount();
}

QStringList DesktopFileSortModel::filterShortcuts() const
{
    return _filterShortcuts;
}

void DesktopFileSortModel::setFilterShortcuts(const QStringList &newFilter)
{
    _filterShortcuts = newFilter;
    setFilterRegExp(_filterShortcuts.join("|"));
    setSortLocaleAware(!isSortLocaleAware());
    sort(0);
}

QString DesktopFileSortModel::filter() const
{
    return _filterString.pattern();
}

void DesktopFileSortModel::setFilter(const QString &newFilter)
{
    _filterString = QRegExp(newFilter, Qt::CaseInsensitive, QRegExp::FixedString);
    setFilterFixedString(newFilter);
}

QVariantMap DesktopFileSortModel::get(int itemIndex)
{
    QModelIndex sourceIndex = mapToSource(index(itemIndex, 0, QModelIndex()));
    QVariantMap data = _fileModel->get(sourceIndex.row());
    return data;
}

void DesktopFileSortModel::fillData()
{
    if (_fileModel) {
        _fileModel->fillData();
    }
}

bool DesktopFileSortModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!_filterShortcuts.isEmpty()) {
        if (!_filterShortcuts.contains(sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), DesktopFileModel::PackageRole).toString())) {
            return false;
        }
    }
    return sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), DesktopFileModel::NameRole).toString().contains(_filterString);
}

bool DesktopFileSortModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!_filterShortcuts.isEmpty()) {
        QString pathLeft = sourceModel()->data(left, DesktopFileModel::PackageRole).toString();
        QString pathRight = sourceModel()->data(right, DesktopFileModel::PackageRole).toString();
        if (_filterShortcuts.contains(pathLeft) && _filterShortcuts.contains(pathRight)) {
            return _filterShortcuts.indexOf(pathLeft) < _filterShortcuts.indexOf(pathRight);
        }
    }
    return sourceModel()->data(left, DesktopFileModel::NameRole).toString().compare(sourceModel()->data(right, DesktopFileModel::NameRole).toString()) < 0;
}
