#include <qdebug.h>
#include "modqsortproxymodel.h"

ModQSortProxyModel::ModQSortProxyModel(QObject *parent):QSortFilterProxyModel(parent)
{


}

ModQSortProxyModel::~ModQSortProxyModel()
{

}

bool ModQSortProxyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
        QSortFilterProxyModel::setData(index, value, role);
        emit modelModified();
        return 0;
}

