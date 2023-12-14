#pragma once
#include <QStyledItemDelegate>
#include <QPainter>

/**
 * @brief The CustomItemDelegate class extends QStyledItemDelegate and is used for painting the table which shows the
 *        flights. The table is a QTableView.
 */

class CustomItemDelegate : public QStyledItemDelegate {
public:
    explicit CustomItemDelegate(QObject* parent = nullptr);
    /**
     * @brief Overridden function to paint items with the specified colors.
     * @param painter Pointer to the QPainter object for painting.
     * @param option Style options for the item view.
     * @param index Index of the item to be painted.
     */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
