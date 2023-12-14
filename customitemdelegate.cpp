#include "customitemdelegate.h"
#include "Data.h"

/**
 * @brief Constructs a new CustomItemDelegate object.
 *
 * @param parent The parent QObject.
 */

CustomItemDelegate::CustomItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

/**
 * @brief Paints the delegate using the given painter and style option for the item specified by index. So
 *        in our application it paints the rows of the QTableView.
 *
 * @param painter The QPainter object.
 * @param option The QStyleOptionViewItem object.
 * @param index The QModelIndex object.
 */
void CustomItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {

    QStyleOptionViewItem updatedOption = option;
    updatedOption.displayAlignment = Qt::AlignCenter;
    /**
     * Paint every odd row in light blue and every even row in dark blue and
     * set the text color to white
     */

    if (index.row() % 2 == 1) { // Check if it's an odd row
        painter->fillRect(option.rect, color1); // Light blue color
        updatedOption.palette.setColor(QPalette::Text, QColor(Qt::white));
    } else {
        painter->fillRect(option.rect, color2);
        updatedOption.palette.setColor(QPalette::Text, QColor(Qt::white));
    }

    /**
 * @brief Calls the paint method of the base class QStyledItemDelegate.
 *
 * This allows the base class to perform its standard painting operations.
 * It's a common pattern when you're overriding a method but still want the base functionality to be executed.
 *
 * @param painter The QPainter object.
 * @param updatedOption The modified QStyleOptionViewItem object.
 * @param index The QModelIndex object.
 */

    QStyledItemDelegate::paint(painter, updatedOption, index);
}
